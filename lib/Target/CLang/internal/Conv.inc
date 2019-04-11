PP_GEN_VISIT_DEF(Conv, conv)
{
  const Tensor *input_X_t = conv.getInput(0);
  const auto input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const auto input_X      = defineTensor(indent, *input_X_t);

  const Tensor *input_W_t = conv.getInput(1);
  const auto input_W_dims = defineDimensionArray(stream, indent, *input_W_t);
  const auto input_W      = defineTensor(indent, *input_W_t);
  // // Prepare input
  // Tensor *input_B_t = NULL;
  // void *input_B = NULL;
  // ArrayHolder<int32_t> input_B_dims;
  // if (pOp.getNumOfInputs() > 2) {
  //   input_B_t = pOp.getInput(2);
  //   input_B = m_ATable[input_B_t];
  //   input_B_dims = getTensorDimensions(*input_B_t);
  // }
  // Prepare output
  const Tensor *output_Y_t = conv.getOutput(0);
  const auto output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  const auto output_Y      = defineTensor(indent, *output_Y_t);
  // Prepare attributes
  const auto auto_pad     = defineVarByExpr<const char*>(stream, indent, toStringLiteral(conv.getAutoPad().value()));
  const auto dilations    = defineArray<int32_t>(stream, indent, conv.getDilations().vector());
  const auto group        = defineVar<int32_t>(stream, indent, conv.getGroup().value());
  const auto kernel_shape = defineArray<int32_t>(stream, indent, conv.getKernelShape().vector());
  const auto pads         = defineArray<int32_t>(stream, indent, conv.getPads().vector());
}
