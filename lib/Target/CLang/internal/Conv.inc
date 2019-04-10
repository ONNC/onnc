PP_GEN_VISIT_DEF(Conv, conv)
{
  const Tensor *input_X_t = conv.getInput(0);
  std::cout << input_X_t << std::endl;
  const auto input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const auto input_X      = defineTensor(indent, *input_X_t);
  const Tensor *input_W_t = conv.getInput(1);
  std::cout << input_W_t << std::endl;
  const auto input_W_dims = defineDimensionArray(stream, indent, *input_W_t);
  const auto input_W      = defineTensor(indent, *input_W_t);
  // // Prepare input
  // Tensor *input_X_t = pOp.getInput(0);
  // void *input_X = m_ATable[input_X_t];
  // ArrayHolder<int32_t> input_X_dims = getTensorDimensions(*input_X_t);
  // Tensor *input_W_t = pOp.getInput(1);
  // void *input_W = m_ATable[input_W_t];
  // ArrayHolder<int32_t> input_W_dims = getTensorDimensions(*input_W_t);
  // Tensor *input_B_t = NULL;
  // void *input_B = NULL;
  // ArrayHolder<int32_t> input_B_dims;
  // if (pOp.getNumOfInputs() > 2) {
  //   input_B_t = pOp.getInput(2);
  //   input_B = m_ATable[input_B_t];
  //   input_B_dims = getTensorDimensions(*input_B_t);
  // }
  // // Prepare output
  const Tensor *input_Y_t = conv.getOutput(0);
  const auto input_Y_dims = defineDimensionArray(stream, indent, *input_Y_t);
  const auto input_Y      = defineTensor(indent, *input_Y_t);
  // Tensor *output_Y_t = pOp.getOutput(0);
  // void *output_Y = m_ATable[output_Y_t];
  // ArrayHolder<int32_t> output_Y_dims = getTensorDimensions(*output_Y_t);
  // // Prepare attributes
  // const char * auto_pad = pOp.getAutoPad().value().c_str();
  // ArrayHolder<int32_t> dilations{pOp.getDilations().vector()};
  // int32_t group = pOp.getGroup().value();
  // ArrayHolder<int32_t> kernel_shape{pOp.getKernelShape().vector()};
  // ArrayHolder<int32_t> pads{pOp.getPads().vector()};
  // ArrayHolder<int32_t> strides{pOp.getStrides().vector()};

}
