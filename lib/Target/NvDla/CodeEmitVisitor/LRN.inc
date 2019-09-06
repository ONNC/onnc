PP_DEFINE_VISIT(LRN, pOp)
{
  // Prepare input
  const Tensor* input_X_t       = pOp.getInput(0);
  int32_t       input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t       input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);
  NvDlaCubeInfo X_cube(*this, NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);

  // Prepare outputSNESSNES
  const Tensor* output_Y_t       = pOp.getOutput(0);
  int32_t       output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t       output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);
  NvDlaCubeInfo Y_cube(*this, NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2],
                       output_Y_dims[3]);

  // Prepare attributes
  float   alpha = 0.0001;
  float   beta  = 0.75;
  float   bias  = 1.0;
  int32_t size  = pOp.getSize().value();

  // LUT parameter
  NvDlaBackendMeta::LutId lut_id    = -1;
  int8_t                  lut_index = 0;

  const NvDlaBackendMeta::LutParams params(alpha, beta, bias, size, lut_index);
  if (!m_pMeta.hasLutId(params)) {
    struct dla_lut_param* lut_param = new dla_lut_param();
    memset(lut_param, 0, sizeof(*lut_param));

    // Set up X table, aka linear_exp table, aka raw table.
    lut_param->method = LUT_METHOD_EXPONENTIAL; // X table can be either
                                                // LUT_METHOD_EXPONENTIAL or LUT_METHOD_LINEAR
    lut_param->linear_exp_offset.exp_offset = 2;
    float le_start                          = 0.0;
    float le_end                            = std::pow(2, 30);
    memcpy_fp32_to_uint32(le_start, (uint32_t&)lut_param->linear_exp_start);
    memcpy_fp32_to_uint32(le_end, (uint32_t&)lut_param->linear_exp_end);

    // Set up Y table, aka linear_only table, aka density table.
    lut_param->linear_only_offset.frac_bits = 0;
    float lo_start                          = pow(2, lut_param->linear_exp_offset.exp_offset);
    float lo_end                            = lo_start + std::pow(2.0, lut_param->linear_only_offset.frac_bits) * 256.0;
    memcpy_fp32_to_uint32(lo_start, (uint32_t&)lut_param->linear_only_start);
    memcpy_fp32_to_uint32(lo_end, (uint32_t&)lut_param->linear_only_end);

    float outdata_scale  = 32768;
    float outdata_offset = 0;

    SetLUTParam(lut_param, alpha, beta, bias, size, outdata_scale, outdata_offset);

    lut_id = m_pMeta.m_LUTList.size();
    m_pMeta.m_LUTList.push_back(lut_param);
    m_pMeta.m_NumLUTs++;

    m_pMeta.addLutId(params, lut_id);
  } else {
    lut_id = m_pMeta.getLutId(params);
  }

  // cdp op parmeters
  NvDlaDlaOperation* lrn_op = new NvDlaDlaOperation();
  lrn_op->op_dep.op_type    = DLA_OP_CDP;

  struct dla_cdp_op_desc* lrn_desc = (struct dla_cdp_op_desc*)(&(lrn_op->op_desc));
  lrn_desc->in_precision           = DLA_PRECISION;
  lrn_desc->out_precision          = DLA_PRECISION;
  lrn_desc->lut_index              = lut_id;
  lrn_desc->in_cvt.scale           = 1;
  lrn_desc->in_cvt.truncate        = 0;
  lrn_desc->in_cvt.enable          = 1;
  lrn_desc->in_cvt.offset          = 0;
  lrn_desc->out_cvt.scale          = 1;
  lrn_desc->out_cvt.truncate       = 0;
  lrn_desc->out_cvt.enable         = 1;
  lrn_desc->out_cvt.offset         = 0;
  lrn_desc->local_size             = size;
  lrn_desc->bypass_sqsum           = 0;
  lrn_desc->bypass_out_mul         = 0;

  struct dla_cdp_surface_desc* lrn_surf = (struct dla_cdp_surface_desc*)(&(lrn_op->op_surf));
  lrn_surf->src_data.type               = DLA_MEM_MC;
  lrn_surf->src_data.address            = issueDlaAddr(*input_X_t, X_cube);
  lrn_surf->src_data.size               = m_pMeta.getMemoryListEntrySize(*input_X_t);
  lrn_surf->src_data.width              = X_cube.dim_w;
  lrn_surf->src_data.height             = X_cube.dim_h;
  lrn_surf->src_data.channel            = X_cube.dim_c;
  lrn_surf->src_data.line_stride        = X_cube.stride_line;
  lrn_surf->src_data.surf_stride        = X_cube.stride_surface;
  lrn_surf->src_data.plane_stride       = X_cube.stride_plane;

  lrn_surf->dst_data.type         = DLA_MEM_MC;
  lrn_surf->dst_data.address      = issueDlaAddr(*output_Y_t, Y_cube);
  lrn_surf->dst_data.size         = m_pMeta.getMemoryListEntrySize(*output_Y_t);
  lrn_surf->dst_data.width        = Y_cube.dim_w;
  lrn_surf->dst_data.height       = Y_cube.dim_h;
  lrn_surf->dst_data.channel      = Y_cube.dim_c;
  lrn_surf->dst_data.line_stride  = Y_cube.stride_line;
  lrn_surf->dst_data.surf_stride  = Y_cube.stride_surface;
  lrn_surf->dst_data.plane_stride = Y_cube.stride_plane;

  issueDlaOp(lrn_op, NULL, m_pMeta.m_pPrevOp);
}
