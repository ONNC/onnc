PP_DEFINE_VISIT(AveragePool, pOp)
{
  const Tensor* input_X_t       = pOp.getInput(0);
  int32_t       input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t       input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);
  MemoryListEntryId          X_mid = m_pMeta.getMemoryListEntryId(*input_X_t);
  ILoadable::MemoryListEntry X_mle = m_pMeta.getMemoryListEntry(X_mid);
  NvDlaCubeInfo X_cube(*this, NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);

  // Prepare output
  const Tensor* output_Y_t       = pOp.getOutput(0);
  int32_t       output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t       output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);
  MemoryListEntryId          Y_mid = m_pMeta.getMemoryListEntryId(*output_Y_t);
  ILoadable::MemoryListEntry Y_mle = m_pMeta.getMemoryListEntry(Y_mid);
  NvDlaCubeInfo Y_cube(*this, NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2], output_Y_dims[3]);

  const Tensor* output_Indices_t    = NULL;
  void*         output_Indices      = NULL;
  int32_t       output_Indices_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_Indices_t    = pOp.getOutput(1);
    output_Indices_ndim = output_Indices_t->getNumOfDimensions();
  }
  int32_t output_Indices_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Indices_ndim; ++i)
    output_Indices_dims[i] = output_Indices_t->dimension(i);

  // Prepare attributes
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i)
    kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pad_shapes[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i)
    pad_shapes[i] = pOp.getPads().at(i);
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i)
    strides[i] = pOp.getStrides().at(i);

  NvDlaDlaOperation* avgpool_op = new NvDlaDlaOperation();
  avgpool_op->op_dep.op_type    = DLA_OP_PDP;

  struct dla_pdp_op_desc* avgpool_desc = (struct dla_pdp_op_desc*)(&(avgpool_op->op_desc));
  avgpool_desc->partial_in_width_first = 0;
  avgpool_desc->partial_in_width_mid   = 0;
  avgpool_desc->partial_in_width_last  = 0;
  avgpool_desc->partial_width_first    = 0;
  avgpool_desc->partial_width_mid      = 0;
  avgpool_desc->partial_width_last     = 0;
  avgpool_desc->split_num              = 1;
  avgpool_desc->pool_mode              = POOL_MODE_AVG;
  avgpool_desc->pool_width             = kernel_shape[1] - 1;
  avgpool_desc->pool_height            = kernel_shape[0] - 1;
  avgpool_desc->stride_x               = strides[1];
  avgpool_desc->stride_y               = strides[0];
  avgpool_desc->pad_top                = pad_shapes[0]; // pad_shape - H
  avgpool_desc->pad_left               = pad_shapes[1]; // pad_shape - W
  avgpool_desc->pad_bottom             = pad_shapes[2];
  avgpool_desc->pad_right              = pad_shapes[3];

  avgpool_desc->precision = DLA_PRECISION;

  struct dla_pdp_surface_desc* avgpool_surf = (struct dla_pdp_surface_desc*)(&(avgpool_op->op_surf));
  avgpool_surf->src_data.type               = DLA_MEM_MC;
  avgpool_surf->src_data.address            = issueDlaAddr(X_mid, X_cube);
  avgpool_surf->src_data.size               = X_mle.size;
  avgpool_surf->src_data.width              = X_cube.dim_w;
  avgpool_surf->src_data.height             = X_cube.dim_h;
  avgpool_surf->src_data.channel            = X_cube.dim_c;
  avgpool_surf->src_data.line_stride        = X_cube.stride_line;
  avgpool_surf->src_data.surf_stride        = X_cube.stride_surface;
  avgpool_surf->src_data.plane_stride       = X_cube.stride_plane;

  avgpool_surf->dst_data.type         = DLA_MEM_MC;
  avgpool_surf->dst_data.address      = issueDlaAddr(Y_mid, Y_cube);
  avgpool_surf->dst_data.size         = Y_mle.size;
  avgpool_surf->dst_data.width        = Y_cube.dim_w;
  avgpool_surf->dst_data.height       = Y_cube.dim_h;
  avgpool_surf->dst_data.channel      = Y_cube.dim_c;
  avgpool_surf->dst_data.line_stride  = Y_cube.stride_line;
  avgpool_surf->dst_data.surf_stride  = Y_cube.stride_surface;
  avgpool_surf->dst_data.plane_stride = Y_cube.stride_plane;

  issueDlaOp(avgpool_op, NULL, m_pMeta.m_pPrevOp);
}
