PP_GEN_VISIT_DEF(Conv, conv)
{
  const Tensor *input_X_t = conv.getInput(0);
  const auto input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const auto input_X      = defineTensor(indent, *input_X_t);

  const Tensor *input_W_t = conv.getInput(1);
  const auto input_W_dims = defineDimensionArray(stream, indent, *input_W_t);
  const auto input_W      = defineTensor(indent, *input_W_t);
  // Prepare input
  const bool hasBias      = (2 < conv.getNumOfInputs());
  const auto input_B      = hasBias ? defineTensor(indent, *conv.getInput(2))
                                    : defineVarByExpr<void*>(stream, indent, PP_STRINGIFY(NULL))
                                    ;
  const auto input_B_dims = hasBias ? defineDimensionArray(stream, indent, *conv.getInput(2))
                                    : defineVarByExpr<int32_t*>(stream, indent, PP_STRINGIFY(NULL))
                                    ;
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
