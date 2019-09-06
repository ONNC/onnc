PP_DEFINE_VISIT(Softmax, pOp)
{
  using namespace internal;

  m_pMeta.m_DlaAddresses = m_pMeta.m_AddressListEntries.size();

  const Tensor* input_input_t       = pOp.getInput(0);
  int32_t       input_input_ndim    = input_input_t->getNumOfDimensions();
  int32_t       input_input_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_input_ndim; ++i)
    input_input_dims[i] = input_input_t->dimension(i);
  int           input_mid = m_pMeta.getMemoryListEntryId(*input_input_t);
  NvDlaCubeInfo iinfo(*this, NVDLA_CUBE_FEATURE, input_input_dims[0], input_input_dims[1], input_input_dims[2],
                      input_input_dims[3]);

  const Tensor* output_output_t       = pOp.getOutput(0);
  int32_t       output_output_ndim    = output_output_t->getNumOfDimensions();
  int32_t       output_output_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_output_ndim; ++i)
    output_output_dims[i] = output_output_t->dimension(i);
  int           output_mid = m_pMeta.getMemoryListEntryId(*output_output_t);
  NvDlaCubeInfo oinfo(*this, NVDLA_CUBE_FEATURE, output_output_dims[0], output_output_dims[1], output_output_dims[2],
                      output_output_dims[3]);

  int32_t axis = pOp.getAxis().value();

  NvDlaEmuOperation*          softmax_op = new NvDlaEmuOperation();
  struct emu_softmax_op_desc* op_desc    = (struct emu_softmax_op_desc*)(&(softmax_op->op_desc));
  emu_common_op_desc          common;
  std::memset(&common, 0, sizeof(common));
  common.op_type             = NVDLA_EMU_OP_SOFTMAX;
  op_desc->axis              = axis;
  op_desc->common = common;

  struct emu_softmax_buffer_descs* op_buf = (struct emu_softmax_buffer_descs*)(&(softmax_op->op_buf));
  op_buf->src_data.addressIndex           = issueEmuAddr(input_mid);
  op_buf->src_data.size                   = m_pMeta.getMemoryListEntrySize(input_mid);
  op_buf->src_data.format                 = DLA_PRECISION;
  op_buf->src_data.width                  = input_input_dims[3];
  op_buf->src_data.height                 = input_input_dims[2];
  op_buf->src_data.channel                = input_input_dims[1];
  op_buf->src_data.line_stride            = iinfo.stride_line;
  op_buf->src_data.surf_stride            = iinfo.stride_surface;

  op_buf->dst_data.addressIndex = issueEmuAddr(output_mid);
  op_buf->dst_data.size         = m_pMeta.getMemoryListEntrySize(output_mid);
  op_buf->dst_data.format       = DLA_PRECISION;
  op_buf->dst_data.width        = output_output_dims[3];
  op_buf->dst_data.height       = output_output_dims[2];
  op_buf->dst_data.channel      = output_output_dims[1];
  op_buf->dst_data.line_stride  = oinfo.stride_line;
  op_buf->dst_data.surf_stride  = oinfo.stride_surface;

  issueEmuOp(softmax_op);
}
