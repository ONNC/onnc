PP_DEFINE_VISIT(Conv, pOp)
{
  const Tensor* input_X_t       = pOp.getInput(0);
  int32_t       input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t       input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);

  const Tensor* input_W_t       = pOp.getInput(1);
  int32_t       input_W_ndim    = input_W_t->getNumOfDimensions();
  int32_t       input_W_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_W_ndim; ++i)
    input_W_dims[i] = input_W_t->dimension(i);

  const Tensor* const     input_B_t    = (pOp.hasBias() ? pOp.getB() : nullptr);
  const Tensor::Dimension input_B_size = (pOp.hasBias() ? input_B_t->dimension(0) : 1);

  const Tensor* output_Y_t       = pOp.getOutput(0);
  int32_t       output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t       output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);
  NvDlaCubeInfo Y_cube(*this, NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2],
                       output_Y_dims[3]);

  // Prepare attributes
  int32_t number_of_dilations = pOp.getDilations().vector().size();
  int32_t dilations[2]        = {1, 1};
  for (int i = 0; i < number_of_dilations; ++i)
    dilations[i] = pOp.getDilations().at(i);
  const int32_t numGroups              = pOp.getGroup().value();
  int32_t       number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t       kernel_shape[2]        = {1, 1};
  for (int i = 0; i < number_of_kernel_shape; ++i)
    kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pads[4]        = {0, 0, 0, 0};
  for (int i = 0; i < number_of_pads; ++i)
    pads[i] = pOp.getPads().at(i);
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[2]        = {1, 1};
  for (int i = 0; i < number_of_strides; ++i)
    strides[i] = pOp.getStrides().at(i);

  NvDlaCubeInfo X_cube(*this, NVDLA_CUBE_FEATURE, input_X_dims[0],
                       input_X_dims[1], input_X_dims[2], input_X_dims[3], pads[1], pads[3]);

  input_X_dims[1] /= numGroups;
  input_W_dims[0] /= numGroups;

  NvDlaCubeInfo fcube_group(*this, NVDLA_CUBE_FEATURE, input_X_dims[0],
                            input_X_dims[1], input_X_dims[2], input_X_dims[3], pads[1], pads[3]);
  const Tensor::Dimension numInputChannelsPerGroup  = input_X_dims[1];
  const Tensor::Dimension numOutputChannelsPerGroup = input_W_dims[0];
  for (std::int32_t groupIdx = 0; groupIdx < numGroups; ++groupIdx) {
    // output/input channel offsets w/o alignment by atomic k
    const Tensor::Dimension inputChannelOffset        = groupIdx * numInputChannelsPerGroup;
    const Tensor::Dimension alignedInputChannelOffset = ((inputChannelOffset / MAC_ATOMIC_K) * MAC_ATOMIC_K);
    const Tensor::Dimension outputChannelOffset       = groupIdx * numOutputChannelsPerGroup;
    const Tensor::Dimension alignedOutputChannelOffset =
      groupIdx * UNIT_ALIGNMENT(numOutputChannelsPerGroup, MAC_ATOMIC_K);

    const Tensor::Dimension numWeightFrontPaddingChannels = inputChannelOffset - alignedInputChannelOffset;

    NvDlaCubeInfo winfo(*this, NVDLA_CUBE_WEIGHT, input_W_dims[0],
                        numWeightFrontPaddingChannels + numInputChannelsPerGroup, input_W_dims[2], input_W_dims[3]);

    // Weight Memory allocation, repacking by groups
    int W_mid = -1;
    W_mid = packWeight(*input_W_t, NvDlaDims(input_W_dims), numWeightFrontPaddingChannels, outputChannelOffset);

    int W_addr = issueDlaAddr(W_mid, winfo);
    int B_mid  = -1;
    int B_addr = -1;

    NvDlaCubeInfo B_info(*this, NVDLA_CUBE_FEATURE, numOutputChannelsPerGroup, 1, 1, 1);

    if (pOp.hasBias()) {
      B_mid  = packBias(*input_B_t, numOutputChannelsPerGroup, outputChannelOffset);
      B_addr = issueDlaAddr(B_mid, B_info);
    }

    int pad_top       = pads[0];
    int pad_left      = pads[1];
    int pad_bottom    = pads[2];
    int pad_right     = pads[3];
    int kernel_height = input_W_dims[2] + (dilations[0] - 1) * (input_W_dims[2] - 1);
    int stride_y      = strides[0];
    int input_height  = input_X_dims[2];
    int output_height = output_Y_dims[2];
    assert(output_height == ((input_height + pad_top + pad_bottom - kernel_height) / stride_y + 1));

    unsigned min_data_banks_needed = 0;
    bool     is_weight_reuse       = false;
    std::tie(min_data_banks_needed, is_weight_reuse) =
      tryAllocateDataAndWeightsIntoCBuf(fcube_group, winfo, dilations[0]);

    int affordable_conv_height = (CBUF_BANK_DEPTH * (CBUF_BANK_NUM - winfo.banks)) / fcube_group.eps;

    int data_banks_of_first_split_layer = -1;
    int input_h_idx         = -pad_top; // the starting H where the input data of each split convolution comes from.
    int output_h_idx        = 0;        // the starting H where the output data of each split convolution goes to.
    int is_first_split      = true;
    int is_last_split       = false;
    int unused_input_height = (input_height + pad_top) - (kernel_height + stride_y * (output_height - 1));
    if (unused_input_height < 0)
      unused_input_height = 0;

    do { // for each split convolution
      is_last_split = (input_height - unused_input_height - std::max(input_h_idx, 0)) <= affordable_conv_height;

      int input_split_height;
      int output_split_height;
      if (is_first_split && is_last_split) { // No split
        output_split_height = output_height;
        input_split_height  = input_height;
      } else if (is_last_split) {
        input_split_height  = input_height - unused_input_height - std::max(input_h_idx, 0);
        output_split_height = output_height - output_h_idx;
        assert(output_split_height == ((input_split_height + pad_bottom - kernel_height) / stride_y + 1));
      } else if (is_first_split) {
        output_split_height = (affordable_conv_height + pad_top - kernel_height) / stride_y + 1;
        input_split_height  = kernel_height + stride_y * (output_split_height - 1) - pad_top;
      } else { // Split convolution in middle
        output_split_height = (affordable_conv_height - kernel_height) / stride_y + 1;
        input_split_height  = kernel_height + stride_y * (output_split_height - 1);
      }

      int split_pad_top    = (is_first_split) ? pad_top : 0;
      int split_pad_bottom = (is_last_split) ? pad_bottom : 0;

      NvDlaCubeInfo finfo(*this, NVDLA_CUBE_FEATURE, input_X_dims[0],
                          numWeightFrontPaddingChannels + numInputChannelsPerGroup, input_split_height,
                          input_X_dims[3], pads[1], pads[3]);

      if (is_first_split) {
        data_banks_of_first_split_layer = finfo.banks;
      } else {                 // The rest split layers
        if (is_weight_reuse) { // All weights are buffered in the CBUF, so weights are reused among split layers.
          if (!(finfo.banks <= data_banks_of_first_split_layer)) {
            fatal(nvdla_unexpected_num_of_banks) << PP_STRINGIFY(finfo.banks) << finfo.banks;
          }
          // All split layers must have the same data_bank setting.
          finfo.banks = data_banks_of_first_split_layer;
        }
      }

      NvDlaCubeInfo oinfo(*this, NVDLA_CUBE_FEATURE, output_Y_dims[0], numOutputChannelsPerGroup, output_split_height,
                          output_Y_dims[3]);

      NvDlaDlaOperation* const conv_op = new NvDlaDlaOperation();
      conv_op->op_dep.op_type          = DLA_OP_CONV;

      struct dla_conv_op_desc* conv_desc = (struct dla_conv_op_desc*)(&(conv_op->op_desc));
      conv_desc->conv_mode               = CONV_MODE_DIRECT;
      conv_desc->data_reuse              = 0;
      conv_desc->weight_reuse            = (is_weight_reuse && !is_first_split) ? 1 : 0;
      conv_desc->skip_data_rls           = 0;
      conv_desc->skip_weight_rls         = (is_weight_reuse && !is_last_split) ? 1 : 0;
      conv_desc->entry_per_slice         = finfo.eps;
      conv_desc->data_format = FORMAT_FEATURE;
      conv_desc->pixel_mapping = 0;
      conv_desc->fetch_grain   = 1;
      conv_desc->batch         = 1;
      conv_desc->weight_format = WEIGHT_FORMAT_UNCOMPRESSED;
      conv_desc->data_bank     = finfo.banks;
      conv_desc->weight_bank   = winfo.banks;
      conv_desc->batch_stride   = 0;
      conv_desc->post_extension = 0;
      conv_desc->pixel_override = 0;
      conv_desc->release = input_split_height;
      conv_desc->input_width_csc    = finfo.dim_w;
      conv_desc->input_height_csc   = finfo.dim_h;
      conv_desc->input_channel_csc  = finfo.dim_c;
      conv_desc->kernel_channel_csc = winfo.dim_c;
      conv_desc->kernel_width_csc   = winfo.dim_w;
      conv_desc->kernel_height_csc  = winfo.dim_h;
      conv_desc->input_width_cmac   = output_Y_dims[3];
      conv_desc->input_height_cmac  = output_split_height;
      conv_desc->bytes_per_kernel   = winfo.dim_c * winfo.dim_h * winfo.dim_w * ELEMENT_SIZE;
      conv_desc->mean_ry            = 0;
      conv_desc->mean_gu            = 0;
      conv_desc->mean_bv            = 0;
      conv_desc->mean_ax            = 0;
      conv_desc->mean_format        = 0;
      conv_desc->conv_stride_x      = strides[1];
      conv_desc->conv_stride_y      = strides[0];
      conv_desc->pad_x_left         = pad_left;
      conv_desc->pad_x_right        = pad_right;
      conv_desc->pad_y_top          = split_pad_top;
      conv_desc->pad_y_bottom       = split_pad_bottom;
      conv_desc->dilation_x         = dilations[1];
      conv_desc->dilation_y         = dilations[0];
      conv_desc->pra_truncate       = 0;
      conv_desc->in_precision       = DLA_PRECISION;
      conv_desc->out_precision      = DLA_PRECISION;
      conv_desc->out_cvt.scale      = 1;
      conv_desc->out_cvt.enable     = 1;
      conv_desc->pad_val            = 0;

      struct dla_conv_surface_desc* conv_surf = (struct dla_conv_surface_desc*)(&(conv_op->op_surf));
      conv_surf->weight_data.type             = DLA_MEM_MC;
      conv_surf->weight_data.address          = W_addr;
      conv_surf->weight_data.size             = m_pMeta.getMemoryListEntrySize(W_mid);
      conv_surf->weight_data.width            = winfo.dim_w;
      conv_surf->weight_data.height           = winfo.dim_h;
      conv_surf->weight_data.channel          = winfo.dim_c;
      conv_surf->weight_data.line_stride      = 0;
      conv_surf->weight_data.surf_stride      = 0;
      conv_surf->weight_data.plane_stride     = 0;

      conv_surf->wmb_data.type    = DLA_MEM_HW;
      conv_surf->wmb_data.address = -1;

      conv_surf->wgs_data.type    = DLA_MEM_HW;
      conv_surf->wgs_data.address = -1;

      conv_surf->src_data.type = DLA_MEM_MC;
      conv_surf->src_data.address =
        issueDlaAddr(*input_X_t, X_cube, alignedInputChannelOffset, std::max(input_h_idx, 0));
      conv_surf->src_data.size         = finfo.size;
      conv_surf->src_data.width        = finfo.dim_w;
      conv_surf->src_data.height       = finfo.dim_h;
      conv_surf->src_data.channel      = finfo.dim_c;
      conv_surf->src_data.line_stride  = fcube_group.stride_line;
      conv_surf->src_data.surf_stride  = fcube_group.stride_surface;
      conv_surf->src_data.plane_stride = fcube_group.stride_plane;

      conv_surf->dst_data.type         = DLA_MEM_HW;
      conv_surf->dst_data.address      = -1;
      conv_surf->dst_data.size         = oinfo.size;
      conv_surf->dst_data.width        = oinfo.dim_w;
      conv_surf->dst_data.height       = oinfo.dim_h;
      conv_surf->dst_data.channel      = oinfo.dim_c;
      conv_surf->dst_data.line_stride  = Y_cube.stride_line;
      conv_surf->dst_data.surf_stride  = Y_cube.stride_surface;
      conv_surf->dst_data.plane_stride = Y_cube.stride_plane;

      // Bias Add
      NvDlaDlaOperation* const add_op = new NvDlaDlaOperation();
      add_op->op_dep.op_type          = DLA_OP_SDP;

      struct dla_sdp_op_desc* add_desc = (struct dla_sdp_op_desc*)(&(add_op->op_desc));
      add_desc->src_precision          = DLA_PRECISION;
      add_desc->dst_precision          = DLA_PRECISION;
      add_desc->lut_index              = -1;
      add_desc->conv_mode              = 0;
      add_desc->out_cvt.scale          = 1;
      add_desc->out_cvt.truncate       = 0;
      add_desc->out_cvt.enable         = 1;
      add_desc->out_cvt.offset         = 0;
      add_desc->conv_mode              = CONV_MODE_DIRECT;
      add_desc->batch_num              = 1;
      add_desc->batch_stride           = 0;
      if (pOp.hasBias()) {
        add_desc->x1_op.enable               = 1;
        add_desc->x1_op.alu_type             = SDP_ALU_OP_SUM;
        add_desc->x1_op.type                 = SDP_OP_ADD;
        add_desc->x1_op.mode                 = SDP_OP_PER_KERNEL;
        add_desc->x1_op.act                  = ACTIVATION_NONE;
        add_desc->x1_op.shift_value          = 0;
        add_desc->x1_op.truncate             = 0;
        add_desc->x1_op.precision            = DLA_PRECISION;
        add_desc->x1_op.alu_operand          = 0;
        add_desc->x1_op.mul_operand          = 0;
        add_desc->x1_op.cvt.alu_cvt.scale    = 0;
        add_desc->x1_op.cvt.alu_cvt.truncate = 0;
        add_desc->x1_op.cvt.alu_cvt.enable   = 0;
        add_desc->x1_op.cvt.alu_cvt.offset   = 0;
        add_desc->x1_op.cvt.mul_cvt.scale    = 0;
        add_desc->x1_op.cvt.mul_cvt.truncate = 0;
        add_desc->x1_op.cvt.mul_cvt.enable   = 0;
        add_desc->x1_op.cvt.mul_cvt.offset   = 0;
      } else {
        add_desc->x1_op.enable = 0;
      }

      add_desc->x2_op.enable = 0;
      add_desc->y_op.enable  = 0;

      struct dla_sdp_surface_desc* add_surf = (struct dla_sdp_surface_desc*)(&(add_op->op_surf));
      add_surf->src_data.type               = DLA_MEM_HW;
      add_surf->src_data.address            = -1;
      add_surf->src_data.size               = conv_surf->dst_data.size;
      add_surf->src_data.width              = conv_surf->dst_data.width;
      add_surf->src_data.height             = conv_surf->dst_data.height;
      add_surf->src_data.channel            = conv_surf->dst_data.channel;
      add_surf->src_data.line_stride        = 0;
      add_surf->src_data.surf_stride        = 0;
      add_surf->src_data.plane_stride       = 0;

      if (pOp.hasBias()) {
        add_surf->x1_data.type         = DLA_MEM_MC;
        add_surf->x1_data.address      = B_addr;
        add_surf->x1_data.size         = m_pMeta.getMemoryListEntrySize(B_mid);
        add_surf->x1_data.width        = 1;
        add_surf->x1_data.height       = 1;
        add_surf->x1_data.channel      = numOutputChannelsPerGroup;
        add_surf->x1_data.line_stride  = B_info.stride_line;
        add_surf->x1_data.surf_stride  = B_info.stride_surface;
        add_surf->x1_data.plane_stride = B_info.stride_plane;
      }

      add_surf->dst_data.type         = DLA_MEM_MC;
      add_surf->dst_data.address      = issueDlaAddr(*output_Y_t, Y_cube, alignedOutputChannelOffset, output_h_idx);
      add_surf->dst_data.size         = conv_surf->dst_data.size;
      add_surf->dst_data.width        = conv_surf->dst_data.width;
      add_surf->dst_data.height       = conv_surf->dst_data.height;
      add_surf->dst_data.channel      = conv_surf->dst_data.channel;
      add_surf->dst_data.line_stride  = conv_surf->dst_data.line_stride;
      add_surf->dst_data.surf_stride  = conv_surf->dst_data.surf_stride;
      add_surf->dst_data.plane_stride = conv_surf->dst_data.plane_stride;

      NvDlaDlaOperation* prev_op = (is_first_split) ? m_pMeta.m_pPrevOp : NULL;
      issueDlaOp(conv_op, add_op, prev_op);

      output_h_idx = output_h_idx + output_split_height;
      input_h_idx  = stride_y * output_h_idx - pad_top;

      is_first_split = false;
    } while (!is_last_split);
  }
}
