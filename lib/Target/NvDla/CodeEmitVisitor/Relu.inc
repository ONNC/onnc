PP_DEFINE_VISIT(Relu, pOp)
{
  const Tensor* input_X_t       = pOp.getInput(0);
  int32_t       input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t       input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);
  NvDlaCubeInfo X_cube(*this, NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);

  const Tensor* output_Y_t       = pOp.getOutput(0);
  int32_t       output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t       output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);

  NvDlaCubeInfo Y_cube(*this, NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2],
                       output_Y_dims[3]);

  NvDlaDlaOperation* relu_op = new NvDlaDlaOperation();
  relu_op->op_dep.op_type    = DLA_OP_SDP;

  struct dla_sdp_op_desc* relu_desc     = (struct dla_sdp_op_desc*)(&(relu_op->op_desc));
  relu_desc->src_precision              = DLA_PRECISION;
  relu_desc->dst_precision              = DLA_PRECISION;
  relu_desc->lut_index                  = -1;
  relu_desc->conv_mode                  = 0;
  relu_desc->out_cvt.scale              = 1;
  relu_desc->out_cvt.truncate           = 0;
  relu_desc->out_cvt.enable             = 1;
  relu_desc->out_cvt.offset             = 0;
  relu_desc->conv_mode                  = CONV_MODE_DIRECT;
  relu_desc->batch_num                  = 1;
  relu_desc->batch_stride               = 0;
  relu_desc->x1_op.enable               = 1;
  relu_desc->x1_op.alu_type             = SDP_ALU_OP_SUM;
  relu_desc->x1_op.type                 = SDP_OP_NONE;
  relu_desc->x1_op.mode                 = SDP_OP_PER_LAYER;
  relu_desc->x1_op.act                  = ACTIVATION_RELU;
  relu_desc->x1_op.shift_value          = 0;
  relu_desc->x1_op.truncate             = 0;
  relu_desc->x1_op.precision            = DLA_PRECISION;
  relu_desc->x1_op.alu_operand          = 0;
  relu_desc->x1_op.mul_operand          = 1;
  relu_desc->x1_op.cvt.alu_cvt.scale    = 0;
  relu_desc->x1_op.cvt.alu_cvt.truncate = 0;
  relu_desc->x1_op.cvt.alu_cvt.enable   = 0;
  relu_desc->x1_op.cvt.alu_cvt.offset   = 0;
  relu_desc->x1_op.cvt.mul_cvt.scale    = 0;
  relu_desc->x1_op.cvt.mul_cvt.truncate = 0;
  relu_desc->x1_op.cvt.mul_cvt.enable   = 0;
  relu_desc->x1_op.cvt.mul_cvt.offset   = 0;

  struct dla_sdp_surface_desc* relu_surf = (struct dla_sdp_surface_desc*)(&(relu_op->op_surf));
  relu_surf->src_data.type               = DLA_MEM_MC;
  relu_surf->src_data.address            = issueDlaAddr(*input_X_t, X_cube);
  relu_surf->src_data.size               = m_pMeta.getMemoryListEntrySize(*input_X_t);
  relu_surf->src_data.width              = X_cube.dim_w;
  relu_surf->src_data.height             = X_cube.dim_h;
  relu_surf->src_data.channel            = X_cube.dim_c;
  relu_surf->src_data.line_stride        = X_cube.stride_line;
  relu_surf->src_data.surf_stride        = X_cube.stride_surface;
  relu_surf->src_data.plane_stride       = X_cube.stride_plane;

  relu_surf->dst_data.type         = DLA_MEM_MC;
  relu_surf->dst_data.address      = issueDlaAddr(*output_Y_t, Y_cube);
  relu_surf->dst_data.size         = m_pMeta.getMemoryListEntrySize(*output_Y_t);
  relu_surf->dst_data.width        = Y_cube.dim_w;
  relu_surf->dst_data.height       = Y_cube.dim_h;
  relu_surf->dst_data.channel      = Y_cube.dim_c;
  relu_surf->dst_data.line_stride  = Y_cube.stride_line;
  relu_surf->dst_data.surf_stride  = Y_cube.stride_surface;
  relu_surf->dst_data.plane_stride = Y_cube.stride_plane;

  issueDlaOp(relu_op, NULL, m_pMeta.m_pPrevOp);
}
