PP_DEFINE_VISIT(Sum, pSum)
{
  auto operation = makeNvDlaOp(NvDlaOpType::sdp);

  auto& desc             = getDesc<NvDlaOpType::sdp>(*operation);
  desc.src_precision     = DLA_PRECISION;
  desc.dst_precision     = DLA_PRECISION;
  desc.lut_index         = -1;
  desc.out_cvt.scale     = 1;
  desc.out_cvt.truncate  = 0;
  desc.out_cvt.enable    = 1;
  desc.out_cvt.offset    = 0;
  desc.conv_mode         = CONV_MODE_DIRECT;
  desc.batch_num         = 1;
  desc.batch_stride      = 0;
  desc.x1_op.enable      = 1;
  desc.x1_op.alu_type    = SDP_ALU_OP_SUM;
  desc.x1_op.type        = SDP_OP_ADD;
  desc.x1_op.mode        = SDP_OP_PER_POINT;
  desc.x1_op.act         = ACTIVATION_NONE;
  desc.x1_op.shift_value = 0;
  desc.x1_op.truncate    = 0;
  desc.x1_op.precision   = DLA_PRECISION;
  desc.x1_op.alu_operand = 0;
  desc.x1_op.mul_operand = 1;
  if (3 <= pSum.getNumOfInputs()) {
    desc.x2_op.enable      = 1;
    desc.x2_op.alu_type    = SDP_ALU_OP_SUM;
    desc.x2_op.type        = SDP_OP_ADD;
    desc.x2_op.mode        = SDP_OP_PER_POINT;
    desc.x2_op.act         = ACTIVATION_NONE;
    desc.x2_op.shift_value = 0;
    desc.x2_op.truncate    = 0;
    desc.x2_op.precision   = DLA_PRECISION;
    desc.x2_op.alu_operand = 0;
    desc.x2_op.mul_operand = 1;

    if (4 <= pSum.getNumOfInputs()) {
      desc.y_op.enable      = 1;
      desc.y_op.alu_type    = SDP_ALU_OP_SUM;
      desc.y_op.type        = SDP_OP_ADD;
      desc.y_op.mode        = SDP_OP_PER_POINT;
      desc.y_op.act         = ACTIVATION_NONE;
      desc.y_op.shift_value = 0;
      desc.y_op.truncate    = 0;
      desc.y_op.precision   = DLA_PRECISION;
      desc.y_op.alu_operand = 0;
      desc.y_op.mul_operand = 1;
    }
  }

  auto& surface = getSurface<NvDlaOpType::sdp>(*operation);

  {
    const Tensor&       first         = *pSum.getInput(0);
    const NvDlaCubeInfo firstCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, first);
    NvDlaDataCubeModifier{surface.src_data, NvDlaMemType::mc}
      .setAddress(issueDlaAddr(first, firstCubeInfo))
      .setInfo(firstCubeInfo);
  }

  {
    const Tensor&       second         = *pSum.getInput(1);
    const NvDlaCubeInfo secondCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, second);
    NvDlaDataCubeModifier{surface.x1_data, NvDlaMemType::mc}
      .setAddress(issueDlaAddr(second, secondCubeInfo))
      .setInfo(secondCubeInfo);
  }

  if (3 <= pSum.getNumOfInputs()) {
    const Tensor&       third         = *pSum.getInput(2);
    const NvDlaCubeInfo thirdCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, third);
    NvDlaDataCubeModifier{surface.x2_data, NvDlaMemType::mc}
      .setAddress(issueDlaAddr(third, thirdCubeInfo))
      .setInfo(thirdCubeInfo);

    if (4 <= pSum.getNumOfInputs()) {
      const Tensor&       fourth         = *pSum.getInput(3);
      const NvDlaCubeInfo fourthCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, fourth);
      NvDlaDataCubeModifier{surface.y_data, NvDlaMemType::mc}
        .setAddress(issueDlaAddr(fourth, fourthCubeInfo))
        .setInfo(fourthCubeInfo);
    }
  }

  const Tensor&       output         = *pSum.getOutput(0);
  const NvDlaCubeInfo outputCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, output);
  NvDlaDataCubeModifier{surface.dst_data, NvDlaMemType::mc}
    .setAddress(issueDlaAddr(output, outputCubeInfo))
    .setInfo(outputCubeInfo);

  issueDlaOp(std::move(operation));
}
