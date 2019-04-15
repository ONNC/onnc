//===- All.inc ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
PP_GEN_VISIT_DEF(Abs, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Acos, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(Add, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(And, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(ArgMax, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto axis     = toExpr(pOp.getAxis().value());
  const auto keepdims = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axis, keepdims});
}

PP_GEN_VISIT_DEF(ArgMin, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto axis     = toExpr(pOp.getAxis().value());
  const auto keepdims = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axis, keepdims});
}

PP_GEN_VISIT_DEF(Asin, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(Atan, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(AveragePool, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto auto_pad               = toExpr(pOp.getAutoPad().value().c_str());
  const auto count_include_pad      = toExpr(pOp.getCountIncludePad().value());
  const auto number_of_kernel_shape = toExpr(pOp.getKernelShape().vector().size());
  const auto kernel_shape           = defineArray<int32_t>(stream, indent, pOp.getKernelShape().vector());
  const auto number_of_pads         = toExpr(pOp.getPads().vector().size());
  const auto pads                   = defineArray<int32_t>(stream, indent, pOp.getPads().vector());
  const auto number_of_strides      = toExpr(pOp.getStrides().vector().size());
  const auto strides                = defineArray<int32_t>(stream, indent, pOp.getStrides().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, auto_pad, count_include_pad, kernel_shape, number_of_kernel_shape, pads, number_of_pads,
          strides, number_of_strides});
}

PP_GEN_VISIT_DEF(BatchNormalization, pOp)
{
  // Prepare input
  const Tensor* input_X_t        = pOp.getInput(0);
  const auto    input_X          = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim     = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims     = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_scale_t    = pOp.getInput(1);
  const auto    input_scale      = defineTensor(indent, *input_scale_t);
  const auto    input_scale_ndim = toExpr(input_scale_t->getNumOfDimensions());
  const auto    input_scale_dims = defineDimensionArray(stream, indent, *input_scale_t);
  const Tensor* input_B_t        = pOp.getInput(2);
  const auto    input_B          = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim     = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims     = defineDimensionArray(stream, indent, *input_B_t);
  const Tensor* input_mean_t     = pOp.getInput(3);
  const auto    input_mean       = defineTensor(indent, *input_mean_t);
  const auto    input_mean_ndim  = toExpr(input_mean_t->getNumOfDimensions());
  const auto    input_mean_dims  = defineDimensionArray(stream, indent, *input_mean_t);
  const Tensor* input_var_t      = pOp.getInput(4);
  const auto    input_var        = defineTensor(indent, *input_var_t);
  const auto    input_var_ndim   = toExpr(input_var_t->getNumOfDimensions());
  const auto    input_var_dims   = defineDimensionArray(stream, indent, *input_var_t);
  // Prepare output
  const Tensor* output_Y_t      = pOp.getOutput(0);
  const auto    output_Y        = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim   = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims   = defineDimensionArray(stream, indent, *output_Y_t);
  const bool    has_output_mean = (1 < pOp.getNumOfOutputs());
  const auto    output_mean =
    has_output_mean ? defineTensor(indent, *pOp.getOutput(1)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_mean_ndim = has_output_mean ? toExpr(pOp.getOutput(1)->getNumOfDimensions()) : toExpr(0);
  const auto output_mean_dims = has_output_mean ? defineDimensionArray(stream, indent, *pOp.getOutput(1))
                                                : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_output_var = (2 < pOp.getNumOfOutputs());
  const auto output_var =
    has_output_var ? defineTensor(indent, *pOp.getOutput(2)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_var_ndim = has_output_var ? toExpr(pOp.getOutput(2)->getNumOfDimensions()) : toExpr(0);
  const auto output_var_dims = has_output_var ? defineDimensionArray(stream, indent, *pOp.getOutput(2))
                                              : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_output_saved_mean = (3 < pOp.getNumOfOutputs());
  const auto output_saved_mean =
    has_output_saved_mean ? defineTensor(indent, *pOp.getOutput(3)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_saved_mean_ndim =
    has_output_saved_mean ? toExpr(pOp.getOutput(3)->getNumOfDimensions()) : toExpr(0);
  const auto output_saved_mean_dims = has_output_saved_mean ? defineDimensionArray(stream, indent, *pOp.getOutput(3))
                                                            : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_output_saved_var = (4 < pOp.getNumOfOutputs());
  const auto output_saved_var =
    has_output_saved_var ? defineTensor(indent, *pOp.getOutput(4)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_saved_var_ndim = has_output_saved_var ? toExpr(pOp.getOutput(4)->getNumOfDimensions()) : toExpr(0);
  const auto output_saved_var_dims = has_output_saved_var ? defineDimensionArray(stream, indent, *pOp.getOutput(4))
                                                          : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare attributes
  const auto epsilon  = toExpr(pOp.getEpsilon().value());
  const auto momentum = toExpr(pOp.getMomentum().value());
  const auto spatial  = toExpr(pOp.getSpatial().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL",
          castExpr<float*>(input_X),
          input_X_ndim,
          input_X_dims,
          castExpr<float*>(input_scale),
          input_scale_ndim,
          input_scale_dims,
          castExpr<float*>(input_B),
          input_B_ndim,
          input_B_dims,
          castExpr<float*>(input_mean),
          input_mean_ndim,
          input_mean_dims,
          castExpr<float*>(input_var),
          input_var_ndim,
          input_var_dims,
          castExpr<float*>(output_Y),
          output_Y_ndim,
          output_Y_dims,
          castExpr<float*>(output_mean),
          output_mean_ndim,
          output_mean_dims,
          castExpr<float*>(output_var),
          output_var_ndim,
          output_var_dims,
          castExpr<float*>(output_saved_mean),
          output_saved_mean_ndim,
          output_saved_mean_dims,
          castExpr<float*>(output_saved_var),
          output_saved_var_ndim,
          output_saved_var_dims,
          epsilon,
          momentum,
          spatial});
}

PP_GEN_VISIT_DEF(Cast, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto to = toExpr(pOp.getTo().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, to});
}

PP_GEN_VISIT_DEF(Ceil, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Clip, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto max = toExpr(pOp.getMax().value());
  const auto min = toExpr(pOp.getMin().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, max, min});
}

PP_GEN_VISIT_DEF(Concat, pOp)
{
  // Prepare input
  const auto input_inputs_ntensor = toExpr(pOp.getNumOfInputs() - 0);
  const auto input_inputs         = defineTensors<TensorType::inputs>(indent, pOp, 0);
  const auto input_inputs_ndim    = defineDimensionNumberArray<TensorType::inputs>(stream, indent, pOp, 0);
  const auto input_inputs_dims    = defineDimensionArrays<TensorType::inputs>(stream, indent, pOp, 0);
  // Prepare output
  const Tensor* output_concat_result_t    = pOp.getOutput(0);
  const auto    output_concat_result      = defineTensor(indent, *output_concat_result_t);
  const auto    output_concat_result_ndim = toExpr(output_concat_result_t->getNumOfDimensions());
  const auto    output_concat_result_dims = defineDimensionArray(stream, indent, *output_concat_result_t);
  // Prepare attributes
  const auto axis = toExpr(pOp.getAxis().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float**>(input_inputs), input_inputs_ntensor, input_inputs_ndim, input_inputs_dims,
          castExpr<float*>(output_concat_result), output_concat_result_ndim, output_concat_result_dims, axis});
}

PP_GEN_VISIT_DEF(Constant, pOp)
{
  // Prepare input

  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto value = toExpr(&pOp.getValue().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL"

          ,
          castExpr<float*>(output_output), output_output_ndim, output_output_dims, value});
}

PP_GEN_VISIT_DEF(Conv, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_W_t    = pOp.getInput(1);
  const auto    input_W      = defineTensor(indent, *input_W_t);
  const auto    input_W_ndim = toExpr(input_W_t->getNumOfDimensions());
  const auto    input_W_dims = defineDimensionArray(stream, indent, *input_W_t);
  const bool    has_input_B  = (2 < pOp.getNumOfInputs());
  const auto    input_B =
    has_input_B ? defineTensor(indent, *pOp.getInput(2)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_B_ndim = has_input_B ? toExpr(pOp.getInput(2)->getNumOfDimensions()) : toExpr(0);
  const auto input_B_dims = has_input_B ? defineDimensionArray(stream, indent, *pOp.getInput(2))
                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto auto_pad               = toExpr(pOp.getAutoPad().value().c_str());
  const auto number_of_dilations    = toExpr(pOp.getDilations().vector().size());
  const auto dilations              = defineArray<int32_t>(stream, indent, pOp.getDilations().vector());
  const auto group                  = toExpr(pOp.getGroup().value());
  const auto number_of_kernel_shape = toExpr(pOp.getKernelShape().vector().size());
  const auto kernel_shape           = defineArray<int32_t>(stream, indent, pOp.getKernelShape().vector());
  const auto number_of_pads         = toExpr(pOp.getPads().vector().size());
  const auto pads                   = defineArray<int32_t>(stream, indent, pOp.getPads().vector());
  const auto number_of_strides      = toExpr(pOp.getStrides().vector().size());
  const auto strides                = defineArray<int32_t>(stream, indent, pOp.getStrides().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL",
          castExpr<float*>(input_X),
          input_X_ndim,
          input_X_dims,
          castExpr<float*>(input_W),
          input_W_ndim,
          input_W_dims,
          castExpr<float*>(input_B),
          input_B_ndim,
          input_B_dims,
          castExpr<float*>(output_Y),
          output_Y_ndim,
          output_Y_dims,
          auto_pad,
          dilations,
          number_of_dilations,
          group,
          kernel_shape,
          number_of_kernel_shape,
          pads,
          number_of_pads,
          strides,
          number_of_strides});
}

PP_GEN_VISIT_DEF(ConvTranspose, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_W_t    = pOp.getInput(1);
  const auto    input_W      = defineTensor(indent, *input_W_t);
  const auto    input_W_ndim = toExpr(input_W_t->getNumOfDimensions());
  const auto    input_W_dims = defineDimensionArray(stream, indent, *input_W_t);
  const bool    has_input_B  = (2 < pOp.getNumOfInputs());
  const auto    input_B =
    has_input_B ? defineTensor(indent, *pOp.getInput(2)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_B_ndim = has_input_B ? toExpr(pOp.getInput(2)->getNumOfDimensions()) : toExpr(0);
  const auto input_B_dims = has_input_B ? defineDimensionArray(stream, indent, *pOp.getInput(2))
                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto auto_pad                 = toExpr(pOp.getAutoPad().value().c_str());
  const auto number_of_dilations      = toExpr(pOp.getDilations().vector().size());
  const auto dilations                = defineArray<int32_t>(stream, indent, pOp.getDilations().vector());
  const auto group                    = toExpr(pOp.getGroup().value());
  const auto number_of_kernel_shape   = toExpr(pOp.getKernelShape().vector().size());
  const auto kernel_shape             = defineArray<int32_t>(stream, indent, pOp.getKernelShape().vector());
  const auto number_of_output_padding = toExpr(pOp.getOutputPadding().vector().size());
  const auto output_padding           = defineArray<int32_t>(stream, indent, pOp.getOutputPadding().vector());
  const auto number_of_output_shape   = toExpr(pOp.getOutputShape().vector().size());
  const auto output_shape             = defineArray<int32_t>(stream, indent, pOp.getOutputShape().vector());
  const auto number_of_pads           = toExpr(pOp.getPads().vector().size());
  const auto pads                     = defineArray<int32_t>(stream, indent, pOp.getPads().vector());
  const auto number_of_strides        = toExpr(pOp.getStrides().vector().size());
  const auto strides                  = defineArray<int32_t>(stream, indent, pOp.getStrides().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL",
          castExpr<float*>(input_X),
          input_X_ndim,
          input_X_dims,
          castExpr<float*>(input_W),
          input_W_ndim,
          input_W_dims,
          castExpr<float*>(input_B),
          input_B_ndim,
          input_B_dims,
          castExpr<float*>(output_Y),
          output_Y_ndim,
          output_Y_dims,
          auto_pad,
          dilations,
          number_of_dilations,
          group,
          kernel_shape,
          number_of_kernel_shape,
          output_padding,
          number_of_output_padding,
          output_shape,
          number_of_output_shape,
          pads,
          number_of_pads,
          strides,
          number_of_strides});
}

PP_GEN_VISIT_DEF(Cos, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(DepthToSpace, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto blocksize = toExpr(pOp.getBlocksize().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, blocksize});
}

PP_GEN_VISIT_DEF(Div, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(Dropout, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  const bool    has_output_mask    = (1 < pOp.getNumOfOutputs());
  const auto    output_mask =
    has_output_mask ? defineTensor(indent, *pOp.getOutput(1)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_mask_ndim = has_output_mask ? toExpr(pOp.getOutput(1)->getNumOfDimensions()) : toExpr(0);
  const auto output_mask_dims = has_output_mask ? defineDimensionArray(stream, indent, *pOp.getOutput(1))
                                                : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare attributes
  const auto ratio = toExpr(pOp.getRatio().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, castExpr<float*>(output_mask), output_mask_ndim, output_mask_dims,
          ratio});
}

PP_GEN_VISIT_DEF(Elu, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, alpha});
}

PP_GEN_VISIT_DEF(Equal, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(Exp, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(Expand, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  const Tensor* input_shape_t    = pOp.getInput(1);
  const auto    input_shape      = defineTensor(indent, *input_shape_t);
  const auto    input_shape_ndim = toExpr(input_shape_t->getNumOfDimensions());
  const auto    input_shape_dims = defineDimensionArray(stream, indent, *input_shape_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(input_shape),
          input_shape_ndim, input_shape_dims, castExpr<float*>(output_output), output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(Flatten, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto axis = toExpr(pOp.getAxis().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, axis});
}

PP_GEN_VISIT_DEF(Floor, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(GRU, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_W_t    = pOp.getInput(1);
  const auto    input_W      = defineTensor(indent, *input_W_t);
  const auto    input_W_ndim = toExpr(input_W_t->getNumOfDimensions());
  const auto    input_W_dims = defineDimensionArray(stream, indent, *input_W_t);
  const Tensor* input_R_t    = pOp.getInput(2);
  const auto    input_R      = defineTensor(indent, *input_R_t);
  const auto    input_R_ndim = toExpr(input_R_t->getNumOfDimensions());
  const auto    input_R_dims = defineDimensionArray(stream, indent, *input_R_t);
  const bool    has_input_B  = (3 < pOp.getNumOfInputs());
  const auto    input_B =
    has_input_B ? defineTensor(indent, *pOp.getInput(3)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_B_ndim = has_input_B ? toExpr(pOp.getInput(3)->getNumOfDimensions()) : toExpr(0);
  const auto input_B_dims = has_input_B ? defineDimensionArray(stream, indent, *pOp.getInput(3))
                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_input_sequence_lens = (4 < pOp.getNumOfInputs());
  const auto input_sequence_lens =
    has_input_sequence_lens ? defineTensor(indent, *pOp.getInput(4)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_sequence_lens_ndim =
    has_input_sequence_lens ? toExpr(pOp.getInput(4)->getNumOfDimensions()) : toExpr(0);
  const auto input_sequence_lens_dims = has_input_sequence_lens ? defineDimensionArray(stream, indent, *pOp.getInput(4))
                                                                : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_input_initial_h = (5 < pOp.getNumOfInputs());
  const auto input_initial_h =
    has_input_initial_h ? defineTensor(indent, *pOp.getInput(5)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_initial_h_ndim = has_input_initial_h ? toExpr(pOp.getInput(5)->getNumOfDimensions()) : toExpr(0);
  const auto input_initial_h_dims = has_input_initial_h ? defineDimensionArray(stream, indent, *pOp.getInput(5))
                                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare output
  const bool has_output_Y = (0 < pOp.getNumOfOutputs());
  const auto output_Y =
    has_output_Y ? defineTensor(indent, *pOp.getOutput(0)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_Y_ndim = has_output_Y ? toExpr(pOp.getOutput(0)->getNumOfDimensions()) : toExpr(0);
  const auto output_Y_dims = has_output_Y ? defineDimensionArray(stream, indent, *pOp.getOutput(0))
                                          : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_output_Y_h = (1 < pOp.getNumOfOutputs());
  const auto output_Y_h =
    has_output_Y_h ? defineTensor(indent, *pOp.getOutput(1)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_Y_h_ndim = has_output_Y_h ? toExpr(pOp.getOutput(1)->getNumOfDimensions()) : toExpr(0);
  const auto output_Y_h_dims = has_output_Y_h ? defineDimensionArray(stream, indent, *pOp.getOutput(1))
                                              : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare attributes
  const auto number_of_activation_alpha = toExpr(pOp.getActivationAlpha().vector().size());
  const auto activation_alpha           = defineArray<float>(stream, indent, pOp.getActivationAlpha().vector());
  const auto number_of_activation_beta  = toExpr(pOp.getActivationBeta().vector().size());
  const auto activation_beta            = defineArray<float>(stream, indent, pOp.getActivationBeta().vector());
  const auto number_of_activations      = toExpr(pOp.getActivations().vector().size());
  const auto activations                = defineArray<const char*>(stream, indent, pOp.getActivations().vector());
  const auto clip                       = toExpr(pOp.getClip().value());
  const auto direction                  = toExpr(pOp.getDirection().value().c_str());
  const auto hidden_size                = toExpr(pOp.getHiddenSize().value());
  const auto linear_before_reset        = toExpr(pOp.getLinearBeforeReset().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL",
          castExpr<float*>(input_X),
          input_X_ndim,
          input_X_dims,
          castExpr<float*>(input_W),
          input_W_ndim,
          input_W_dims,
          castExpr<float*>(input_R),
          input_R_ndim,
          input_R_dims,
          castExpr<float*>(input_B),
          input_B_ndim,
          input_B_dims,
          castExpr<float*>(input_sequence_lens),
          input_sequence_lens_ndim,
          input_sequence_lens_dims,
          castExpr<float*>(input_initial_h),
          input_initial_h_ndim,
          input_initial_h_dims,
          castExpr<float*>(output_Y),
          output_Y_ndim,
          output_Y_dims,
          castExpr<float*>(output_Y_h),
          output_Y_h_ndim,
          output_Y_h_dims,
          activation_alpha,
          number_of_activation_alpha,
          activation_beta,
          number_of_activation_beta,
          activations,
          number_of_activations,
          clip,
          direction,
          hidden_size,
          linear_before_reset});
}

PP_GEN_VISIT_DEF(Gather, pOp)
{
  // Prepare input
  const Tensor* input_data_t       = pOp.getInput(0);
  const auto    input_data         = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim    = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims    = defineDimensionArray(stream, indent, *input_data_t);
  const Tensor* input_indices_t    = pOp.getInput(1);
  const auto    input_indices      = defineTensor(indent, *input_indices_t);
  const auto    input_indices_ndim = toExpr(input_indices_t->getNumOfDimensions());
  const auto    input_indices_dims = defineDimensionArray(stream, indent, *input_indices_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto axis = toExpr(pOp.getAxis().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(input_indices),
          input_indices_ndim, input_indices_dims, castExpr<float*>(output_output), output_output_ndim,
          output_output_dims, axis});
}

PP_GEN_VISIT_DEF(Gemm, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  const Tensor* input_C_t    = pOp.getInput(2);
  const auto    input_C      = defineTensor(indent, *input_C_t);
  const auto    input_C_ndim = toExpr(input_C_t->getNumOfDimensions());
  const auto    input_C_dims = defineDimensionArray(stream, indent, *input_C_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha  = toExpr(pOp.getAlpha().value());
  const auto beta   = toExpr(pOp.getBeta().value());
  const auto transA = toExpr(pOp.getTransA().value());
  const auto transB = toExpr(pOp.getTransB().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(input_C), input_C_ndim, input_C_dims, castExpr<float*>(output_Y),
          output_Y_ndim, output_Y_dims, alpha, beta, transA, transB});
}

PP_GEN_VISIT_DEF(GlobalAveragePool, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(GlobalLpPool, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto p = toExpr(pOp.getP().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, p});
}

PP_GEN_VISIT_DEF(GlobalMaxPool, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Greater, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(HardSigmoid, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());
  const auto beta  = toExpr(pOp.getBeta().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, alpha, beta});
}

PP_GEN_VISIT_DEF(Hardmax, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto axis = toExpr(pOp.getAxis().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, axis});
}

PP_GEN_VISIT_DEF(Identity, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(InstanceNormalization, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  const Tensor* input_scale_t    = pOp.getInput(1);
  const auto    input_scale      = defineTensor(indent, *input_scale_t);
  const auto    input_scale_ndim = toExpr(input_scale_t->getNumOfDimensions());
  const auto    input_scale_dims = defineDimensionArray(stream, indent, *input_scale_t);
  const Tensor* input_B_t        = pOp.getInput(2);
  const auto    input_B          = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim     = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims     = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto epsilon = toExpr(pOp.getEpsilon().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(input_scale),
          input_scale_ndim, input_scale_dims, castExpr<float*>(input_B), input_B_ndim, input_B_dims,
          castExpr<float*>(output_output), output_output_ndim, output_output_dims, epsilon});
}

PP_GEN_VISIT_DEF(LRN, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());
  const auto beta  = toExpr(pOp.getBeta().value());
  const auto bias  = toExpr(pOp.getBias().value());
  const auto size  = toExpr(pOp.getSize().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, alpha, beta, bias, size});
}

PP_GEN_VISIT_DEF(LSTM, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_W_t    = pOp.getInput(1);
  const auto    input_W      = defineTensor(indent, *input_W_t);
  const auto    input_W_ndim = toExpr(input_W_t->getNumOfDimensions());
  const auto    input_W_dims = defineDimensionArray(stream, indent, *input_W_t);
  const Tensor* input_R_t    = pOp.getInput(2);
  const auto    input_R      = defineTensor(indent, *input_R_t);
  const auto    input_R_ndim = toExpr(input_R_t->getNumOfDimensions());
  const auto    input_R_dims = defineDimensionArray(stream, indent, *input_R_t);
  const bool    has_input_B  = (3 < pOp.getNumOfInputs());
  const auto    input_B =
    has_input_B ? defineTensor(indent, *pOp.getInput(3)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_B_ndim = has_input_B ? toExpr(pOp.getInput(3)->getNumOfDimensions()) : toExpr(0);
  const auto input_B_dims = has_input_B ? defineDimensionArray(stream, indent, *pOp.getInput(3))
                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_input_sequence_lens = (4 < pOp.getNumOfInputs());
  const auto input_sequence_lens =
    has_input_sequence_lens ? defineTensor(indent, *pOp.getInput(4)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_sequence_lens_ndim =
    has_input_sequence_lens ? toExpr(pOp.getInput(4)->getNumOfDimensions()) : toExpr(0);
  const auto input_sequence_lens_dims = has_input_sequence_lens ? defineDimensionArray(stream, indent, *pOp.getInput(4))
                                                                : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_input_initial_h = (5 < pOp.getNumOfInputs());
  const auto input_initial_h =
    has_input_initial_h ? defineTensor(indent, *pOp.getInput(5)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_initial_h_ndim = has_input_initial_h ? toExpr(pOp.getInput(5)->getNumOfDimensions()) : toExpr(0);
  const auto input_initial_h_dims = has_input_initial_h ? defineDimensionArray(stream, indent, *pOp.getInput(5))
                                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_input_initial_c = (6 < pOp.getNumOfInputs());
  const auto input_initial_c =
    has_input_initial_c ? defineTensor(indent, *pOp.getInput(6)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_initial_c_ndim = has_input_initial_c ? toExpr(pOp.getInput(6)->getNumOfDimensions()) : toExpr(0);
  const auto input_initial_c_dims = has_input_initial_c ? defineDimensionArray(stream, indent, *pOp.getInput(6))
                                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_input_P = (7 < pOp.getNumOfInputs());
  const auto input_P =
    has_input_P ? defineTensor(indent, *pOp.getInput(7)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_P_ndim = has_input_P ? toExpr(pOp.getInput(7)->getNumOfDimensions()) : toExpr(0);
  const auto input_P_dims = has_input_P ? defineDimensionArray(stream, indent, *pOp.getInput(7))
                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare output
  const bool has_output_Y = (0 < pOp.getNumOfOutputs());
  const auto output_Y =
    has_output_Y ? defineTensor(indent, *pOp.getOutput(0)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_Y_ndim = has_output_Y ? toExpr(pOp.getOutput(0)->getNumOfDimensions()) : toExpr(0);
  const auto output_Y_dims = has_output_Y ? defineDimensionArray(stream, indent, *pOp.getOutput(0))
                                          : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_output_Y_h = (1 < pOp.getNumOfOutputs());
  const auto output_Y_h =
    has_output_Y_h ? defineTensor(indent, *pOp.getOutput(1)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_Y_h_ndim = has_output_Y_h ? toExpr(pOp.getOutput(1)->getNumOfDimensions()) : toExpr(0);
  const auto output_Y_h_dims = has_output_Y_h ? defineDimensionArray(stream, indent, *pOp.getOutput(1))
                                              : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_output_Y_c = (2 < pOp.getNumOfOutputs());
  const auto output_Y_c =
    has_output_Y_c ? defineTensor(indent, *pOp.getOutput(2)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_Y_c_ndim = has_output_Y_c ? toExpr(pOp.getOutput(2)->getNumOfDimensions()) : toExpr(0);
  const auto output_Y_c_dims = has_output_Y_c ? defineDimensionArray(stream, indent, *pOp.getOutput(2))
                                              : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare attributes
  const auto number_of_activation_alpha = toExpr(pOp.getActivationAlpha().vector().size());
  const auto activation_alpha           = defineArray<float>(stream, indent, pOp.getActivationAlpha().vector());
  const auto number_of_activation_beta  = toExpr(pOp.getActivationBeta().vector().size());
  const auto activation_beta            = defineArray<float>(stream, indent, pOp.getActivationBeta().vector());
  const auto number_of_activations      = toExpr(pOp.getActivations().vector().size());
  const auto activations                = defineArray<const char*>(stream, indent, pOp.getActivations().vector());
  const auto clip                       = toExpr(pOp.getClip().value());
  const auto direction                  = toExpr(pOp.getDirection().value().c_str());
  const auto hidden_size                = toExpr(pOp.getHiddenSize().value());
  const auto input_forget               = toExpr(pOp.getInputForget().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL",
          castExpr<float*>(input_X),
          input_X_ndim,
          input_X_dims,
          castExpr<float*>(input_W),
          input_W_ndim,
          input_W_dims,
          castExpr<float*>(input_R),
          input_R_ndim,
          input_R_dims,
          castExpr<float*>(input_B),
          input_B_ndim,
          input_B_dims,
          castExpr<float*>(input_sequence_lens),
          input_sequence_lens_ndim,
          input_sequence_lens_dims,
          castExpr<float*>(input_initial_h),
          input_initial_h_ndim,
          input_initial_h_dims,
          castExpr<float*>(input_initial_c),
          input_initial_c_ndim,
          input_initial_c_dims,
          castExpr<float*>(input_P),
          input_P_ndim,
          input_P_dims,
          castExpr<float*>(output_Y),
          output_Y_ndim,
          output_Y_dims,
          castExpr<float*>(output_Y_h),
          output_Y_h_ndim,
          output_Y_h_dims,
          castExpr<float*>(output_Y_c),
          output_Y_c_ndim,
          output_Y_c_dims,
          activation_alpha,
          number_of_activation_alpha,
          activation_beta,
          number_of_activation_beta,
          activations,
          number_of_activations,
          clip,
          direction,
          hidden_size,
          input_forget});
}

PP_GEN_VISIT_DEF(LeakyRelu, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, alpha});
}

PP_GEN_VISIT_DEF(Less, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(Log, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(LogSoftmax, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto axis = toExpr(pOp.getAxis().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, axis});
}

PP_GEN_VISIT_DEF(LpNormalization, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto axis = toExpr(pOp.getAxis().value());
  const auto p    = toExpr(pOp.getP().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, axis, p});
}

PP_GEN_VISIT_DEF(LpPool, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto auto_pad               = toExpr(pOp.getAutoPad().value().c_str());
  const auto number_of_kernel_shape = toExpr(pOp.getKernelShape().vector().size());
  const auto kernel_shape           = defineArray<int32_t>(stream, indent, pOp.getKernelShape().vector());
  const auto p                      = toExpr(pOp.getP().value());
  const auto number_of_pads         = toExpr(pOp.getPads().vector().size());
  const auto pads                   = defineArray<int32_t>(stream, indent, pOp.getPads().vector());
  const auto number_of_strides      = toExpr(pOp.getStrides().vector().size());
  const auto strides                = defineArray<int32_t>(stream, indent, pOp.getStrides().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, auto_pad, kernel_shape, number_of_kernel_shape, p, pads, number_of_pads, strides,
          number_of_strides});
}

PP_GEN_VISIT_DEF(MatMul, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_Y), output_Y_ndim, output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Max, pOp)
{
  // Prepare input
  const auto input_data_0_ntensor = toExpr(pOp.getNumOfInputs() - 0);
  const auto input_data_0         = defineTensors<TensorType::data_0>(indent, pOp, 0);
  const auto input_data_0_ndim    = defineDimensionNumberArray<TensorType::data_0>(stream, indent, pOp, 0);
  const auto input_data_0_dims    = defineDimensionArrays<TensorType::data_0>(stream, indent, pOp, 0);
  // Prepare output
  const Tensor* output_max_t    = pOp.getOutput(0);
  const auto    output_max      = defineTensor(indent, *output_max_t);
  const auto    output_max_ndim = toExpr(output_max_t->getNumOfDimensions());
  const auto    output_max_dims = defineDimensionArray(stream, indent, *output_max_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float**>(input_data_0), input_data_0_ntensor, input_data_0_ndim, input_data_0_dims,
          castExpr<float*>(output_max), output_max_ndim, output_max_dims

         });
}

PP_GEN_VISIT_DEF(MaxPool, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t         = pOp.getOutput(0);
  const auto    output_Y           = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim      = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims      = defineDimensionArray(stream, indent, *output_Y_t);
  const bool    has_output_Indices = (1 < pOp.getNumOfOutputs());
  const auto    output_Indices =
    has_output_Indices ? defineTensor(indent, *pOp.getOutput(1)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_Indices_ndim = has_output_Indices ? toExpr(pOp.getOutput(1)->getNumOfDimensions()) : toExpr(0);
  const auto output_Indices_dims = has_output_Indices ? defineDimensionArray(stream, indent, *pOp.getOutput(1))
                                                      : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare attributes
  const auto auto_pad               = toExpr(pOp.getAutoPad().value().c_str());
  const auto number_of_kernel_shape = toExpr(pOp.getKernelShape().vector().size());
  const auto kernel_shape           = defineArray<int32_t>(stream, indent, pOp.getKernelShape().vector());
  const auto number_of_pads         = toExpr(pOp.getPads().vector().size());
  const auto pads                   = defineArray<int32_t>(stream, indent, pOp.getPads().vector());
  const auto storage_order          = toExpr(pOp.getStorageOrder().value());
  const auto number_of_strides      = toExpr(pOp.getStrides().vector().size());
  const auto strides                = defineArray<int32_t>(stream, indent, pOp.getStrides().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, castExpr<float*>(output_Indices), output_Indices_ndim, output_Indices_dims, auto_pad,
          kernel_shape, number_of_kernel_shape, pads, number_of_pads, storage_order, strides, number_of_strides});
}

PP_GEN_VISIT_DEF(MaxRoiPool, pOp)
{
  // Prepare input
  const Tensor* input_X_t       = pOp.getInput(0);
  const auto    input_X         = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim    = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims    = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_rois_t    = pOp.getInput(1);
  const auto    input_rois      = defineTensor(indent, *input_rois_t);
  const auto    input_rois_ndim = toExpr(input_rois_t->getNumOfDimensions());
  const auto    input_rois_dims = defineDimensionArray(stream, indent, *input_rois_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto number_of_pooled_shape = toExpr(pOp.getPooledShape().vector().size());
  const auto pooled_shape           = defineArray<int32_t>(stream, indent, pOp.getPooledShape().vector());
  const auto spatial_scale          = toExpr(pOp.getSpatialScale().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(input_rois), input_rois_ndim,
          input_rois_dims, castExpr<float*>(output_Y), output_Y_ndim, output_Y_dims, pooled_shape,
          number_of_pooled_shape, spatial_scale});
}

PP_GEN_VISIT_DEF(Mean, pOp)
{
  // Prepare input
  const auto input_data_0_ntensor = toExpr(pOp.getNumOfInputs() - 0);
  const auto input_data_0         = defineTensors<TensorType::data_0>(indent, pOp, 0);
  const auto input_data_0_ndim    = defineDimensionNumberArray<TensorType::data_0>(stream, indent, pOp, 0);
  const auto input_data_0_dims    = defineDimensionArrays<TensorType::data_0>(stream, indent, pOp, 0);
  // Prepare output
  const Tensor* output_mean_t    = pOp.getOutput(0);
  const auto    output_mean      = defineTensor(indent, *output_mean_t);
  const auto    output_mean_ndim = toExpr(output_mean_t->getNumOfDimensions());
  const auto    output_mean_dims = defineDimensionArray(stream, indent, *output_mean_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float**>(input_data_0), input_data_0_ntensor, input_data_0_ndim, input_data_0_dims,
          castExpr<float*>(output_mean), output_mean_ndim, output_mean_dims

         });
}

PP_GEN_VISIT_DEF(Min, pOp)
{
  // Prepare input
  const auto input_data_0_ntensor = toExpr(pOp.getNumOfInputs() - 0);
  const auto input_data_0         = defineTensors<TensorType::data_0>(indent, pOp, 0);
  const auto input_data_0_ndim    = defineDimensionNumberArray<TensorType::data_0>(stream, indent, pOp, 0);
  const auto input_data_0_dims    = defineDimensionArrays<TensorType::data_0>(stream, indent, pOp, 0);
  // Prepare output
  const Tensor* output_min_t    = pOp.getOutput(0);
  const auto    output_min      = defineTensor(indent, *output_min_t);
  const auto    output_min_ndim = toExpr(output_min_t->getNumOfDimensions());
  const auto    output_min_dims = defineDimensionArray(stream, indent, *output_min_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float**>(input_data_0), input_data_0_ntensor, input_data_0_ndim, input_data_0_dims,
          castExpr<float*>(output_min), output_min_ndim, output_min_dims

         });
}

PP_GEN_VISIT_DEF(Mul, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(Multinomial, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto dtype       = toExpr(pOp.getDtype().value());
  const auto sample_size = toExpr(pOp.getSampleSize().value());
  const auto seed        = toExpr(pOp.getSeed().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, dtype, sample_size, seed});
}

PP_GEN_VISIT_DEF(Neg, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Not, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Or, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(PRelu, pOp)
{
  // Prepare input
  const Tensor* input_X_t        = pOp.getInput(0);
  const auto    input_X          = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim     = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims     = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_slope_t    = pOp.getInput(1);
  const auto    input_slope      = defineTensor(indent, *input_slope_t);
  const auto    input_slope_ndim = toExpr(input_slope_t->getNumOfDimensions());
  const auto    input_slope_dims = defineDimensionArray(stream, indent, *input_slope_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(input_slope),
          input_slope_ndim, input_slope_dims, castExpr<float*>(output_Y), output_Y_ndim, output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Pad, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto mode           = toExpr(pOp.getMode().value().c_str());
  const auto number_of_pads = toExpr(pOp.getPads().vector().size());
  const auto pads           = defineArray<int32_t>(stream, indent, pOp.getPads().vector());
  const auto value          = toExpr(pOp.getValue().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, mode, pads, number_of_pads, value});
}

PP_GEN_VISIT_DEF(Pow, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_Y_t    = pOp.getInput(1);
  const auto    input_Y      = defineTensor(indent, *input_Y_t);
  const auto    input_Y_ndim = toExpr(input_Y_t->getNumOfDimensions());
  const auto    input_Y_dims = defineDimensionArray(stream, indent, *input_Y_t);
  // Prepare output
  const Tensor* output_Z_t    = pOp.getOutput(0);
  const auto    output_Z      = defineTensor(indent, *output_Z_t);
  const auto    output_Z_ndim = toExpr(output_Z_t->getNumOfDimensions());
  const auto    output_Z_dims = defineDimensionArray(stream, indent, *output_Z_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(input_Y), input_Y_ndim,
          input_Y_dims, castExpr<float*>(output_Z), output_Z_ndim, output_Z_dims

         });
}

PP_GEN_VISIT_DEF(RNN, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  const Tensor* input_W_t    = pOp.getInput(1);
  const auto    input_W      = defineTensor(indent, *input_W_t);
  const auto    input_W_ndim = toExpr(input_W_t->getNumOfDimensions());
  const auto    input_W_dims = defineDimensionArray(stream, indent, *input_W_t);
  const Tensor* input_R_t    = pOp.getInput(2);
  const auto    input_R      = defineTensor(indent, *input_R_t);
  const auto    input_R_ndim = toExpr(input_R_t->getNumOfDimensions());
  const auto    input_R_dims = defineDimensionArray(stream, indent, *input_R_t);
  const bool    has_input_B  = (3 < pOp.getNumOfInputs());
  const auto    input_B =
    has_input_B ? defineTensor(indent, *pOp.getInput(3)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_B_ndim = has_input_B ? toExpr(pOp.getInput(3)->getNumOfDimensions()) : toExpr(0);
  const auto input_B_dims = has_input_B ? defineDimensionArray(stream, indent, *pOp.getInput(3))
                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_input_sequence_lens = (4 < pOp.getNumOfInputs());
  const auto input_sequence_lens =
    has_input_sequence_lens ? defineTensor(indent, *pOp.getInput(4)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_sequence_lens_ndim =
    has_input_sequence_lens ? toExpr(pOp.getInput(4)->getNumOfDimensions()) : toExpr(0);
  const auto input_sequence_lens_dims = has_input_sequence_lens ? defineDimensionArray(stream, indent, *pOp.getInput(4))
                                                                : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_input_initial_h = (5 < pOp.getNumOfInputs());
  const auto input_initial_h =
    has_input_initial_h ? defineTensor(indent, *pOp.getInput(5)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_initial_h_ndim = has_input_initial_h ? toExpr(pOp.getInput(5)->getNumOfDimensions()) : toExpr(0);
  const auto input_initial_h_dims = has_input_initial_h ? defineDimensionArray(stream, indent, *pOp.getInput(5))
                                                        : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare output
  const bool has_output_Y = (0 < pOp.getNumOfOutputs());
  const auto output_Y =
    has_output_Y ? defineTensor(indent, *pOp.getOutput(0)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_Y_ndim = has_output_Y ? toExpr(pOp.getOutput(0)->getNumOfDimensions()) : toExpr(0);
  const auto output_Y_dims = has_output_Y ? defineDimensionArray(stream, indent, *pOp.getOutput(0))
                                          : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  const bool has_output_Y_h = (1 < pOp.getNumOfOutputs());
  const auto output_Y_h =
    has_output_Y_h ? defineTensor(indent, *pOp.getOutput(1)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto output_Y_h_ndim = has_output_Y_h ? toExpr(pOp.getOutput(1)->getNumOfDimensions()) : toExpr(0);
  const auto output_Y_h_dims = has_output_Y_h ? defineDimensionArray(stream, indent, *pOp.getOutput(1))
                                              : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare attributes
  const auto number_of_activation_alpha = toExpr(pOp.getActivationAlpha().vector().size());
  const auto activation_alpha           = defineArray<float>(stream, indent, pOp.getActivationAlpha().vector());
  const auto number_of_activation_beta  = toExpr(pOp.getActivationBeta().vector().size());
  const auto activation_beta            = defineArray<float>(stream, indent, pOp.getActivationBeta().vector());
  const auto number_of_activations      = toExpr(pOp.getActivations().vector().size());
  const auto activations                = defineArray<const char*>(stream, indent, pOp.getActivations().vector());
  const auto clip                       = toExpr(pOp.getClip().value());
  const auto direction                  = toExpr(pOp.getDirection().value().c_str());
  const auto hidden_size                = toExpr(pOp.getHiddenSize().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL",
          castExpr<float*>(input_X),
          input_X_ndim,
          input_X_dims,
          castExpr<float*>(input_W),
          input_W_ndim,
          input_W_dims,
          castExpr<float*>(input_R),
          input_R_ndim,
          input_R_dims,
          castExpr<float*>(input_B),
          input_B_ndim,
          input_B_dims,
          castExpr<float*>(input_sequence_lens),
          input_sequence_lens_ndim,
          input_sequence_lens_dims,
          castExpr<float*>(input_initial_h),
          input_initial_h_ndim,
          input_initial_h_dims,
          castExpr<float*>(output_Y),
          output_Y_ndim,
          output_Y_dims,
          castExpr<float*>(output_Y_h),
          output_Y_h_ndim,
          output_Y_h_dims,
          activation_alpha,
          number_of_activation_alpha,
          activation_beta,
          number_of_activation_beta,
          activations,
          number_of_activations,
          clip,
          direction,
          hidden_size});
}

PP_GEN_VISIT_DEF(RandomNormal, pOp)
{
  // Prepare input

  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto dtype           = toExpr(pOp.getDtype().value());
  const auto mean            = toExpr(pOp.getMean().value());
  const auto scale           = toExpr(pOp.getScale().value());
  const auto seed            = toExpr(pOp.getSeed().value());
  const auto number_of_shape = toExpr(pOp.getShape().vector().size());
  const auto shape           = defineArray<int32_t>(stream, indent, pOp.getShape().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL"

          ,
          castExpr<float*>(output_output), output_output_ndim, output_output_dims, dtype, mean, scale, seed, shape,
          number_of_shape});
}

PP_GEN_VISIT_DEF(RandomNormalLike, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto dtype = toExpr(pOp.getDtype().value());
  const auto mean  = toExpr(pOp.getMean().value());
  const auto scale = toExpr(pOp.getScale().value());
  const auto seed  = toExpr(pOp.getSeed().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, dtype, mean, scale, seed});
}

PP_GEN_VISIT_DEF(RandomUniform, pOp)
{
  // Prepare input

  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto dtype           = toExpr(pOp.getDtype().value());
  const auto high            = toExpr(pOp.getHigh().value());
  const auto low             = toExpr(pOp.getLow().value());
  const auto seed            = toExpr(pOp.getSeed().value());
  const auto number_of_shape = toExpr(pOp.getShape().vector().size());
  const auto shape           = defineArray<int32_t>(stream, indent, pOp.getShape().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL"

          ,
          castExpr<float*>(output_output), output_output_ndim, output_output_dims, dtype, high, low, seed, shape,
          number_of_shape});
}

PP_GEN_VISIT_DEF(RandomUniformLike, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto dtype = toExpr(pOp.getDtype().value());
  const auto high  = toExpr(pOp.getHigh().value());
  const auto low   = toExpr(pOp.getLow().value());
  const auto seed  = toExpr(pOp.getSeed().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, dtype, high, low, seed});
}

PP_GEN_VISIT_DEF(Reciprocal, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(ReduceL1, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceL2, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceLogSum, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceLogSumExp, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceMax, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceMean, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceMin, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceProd, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceSum, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(ReduceSumSquare, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_reduced_t    = pOp.getOutput(0);
  const auto    output_reduced      = defineTensor(indent, *output_reduced_t);
  const auto    output_reduced_ndim = toExpr(output_reduced_t->getNumOfDimensions());
  const auto    output_reduced_dims = defineDimensionArray(stream, indent, *output_reduced_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto keepdims       = toExpr(pOp.getKeepdims().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_reduced),
          output_reduced_ndim, output_reduced_dims, axes, number_of_axes, keepdims});
}

PP_GEN_VISIT_DEF(Relu, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Reshape, pOp)
{
  // Prepare input
  const Tensor* input_data_t     = pOp.getInput(0);
  const auto    input_data       = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim  = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims  = defineDimensionArray(stream, indent, *input_data_t);
  const Tensor* input_shape_t    = pOp.getInput(1);
  const auto    input_shape      = defineTensor(indent, *input_shape_t);
  const auto    input_shape_ndim = toExpr(input_shape_t->getNumOfDimensions());
  const auto    input_shape_dims = defineDimensionArray(stream, indent, *input_shape_t);
  // Prepare output
  const Tensor* output_reshaped_t    = pOp.getOutput(0);
  const auto    output_reshaped      = defineTensor(indent, *output_reshaped_t);
  const auto    output_reshaped_ndim = toExpr(output_reshaped_t->getNumOfDimensions());
  const auto    output_reshaped_dims = defineDimensionArray(stream, indent, *output_reshaped_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(input_shape),
          input_shape_ndim, input_shape_dims, castExpr<float*>(output_reshaped), output_reshaped_ndim,
          output_reshaped_dims

         });
}

PP_GEN_VISIT_DEF(Selu, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());
  const auto gamma = toExpr(pOp.getGamma().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, alpha, gamma});
}

PP_GEN_VISIT_DEF(Shape, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_shape_t    = pOp.getOutput(0);
  const auto    output_shape      = defineTensor(indent, *output_shape_t);
  const auto    output_shape_ndim = toExpr(output_shape_t->getNumOfDimensions());
  const auto    output_shape_dims = defineDimensionArray(stream, indent, *output_shape_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_shape),
          output_shape_ndim, output_shape_dims

         });
}

PP_GEN_VISIT_DEF(Sigmoid, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Sin, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(Size, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_size_t    = pOp.getOutput(0);
  const auto    output_size      = defineTensor(indent, *output_size_t);
  const auto    output_size_ndim = toExpr(output_size_t->getNumOfDimensions());
  const auto    output_size_dims = defineDimensionArray(stream, indent, *output_size_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_size),
          output_size_ndim, output_size_dims

         });
}

PP_GEN_VISIT_DEF(Slice, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto number_of_axes   = toExpr(pOp.getAxes().vector().size());
  const auto axes             = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());
  const auto number_of_ends   = toExpr(pOp.getEnds().vector().size());
  const auto ends             = defineArray<int32_t>(stream, indent, pOp.getEnds().vector());
  const auto number_of_starts = toExpr(pOp.getStarts().vector().size());
  const auto starts           = defineArray<int32_t>(stream, indent, pOp.getStarts().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, axes, number_of_axes, ends, number_of_ends, starts,
          number_of_starts});
}

PP_GEN_VISIT_DEF(Softmax, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto axis = toExpr(pOp.getAxis().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, axis});
}

PP_GEN_VISIT_DEF(Softplus, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Softsign, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(SpaceToDepth, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto blocksize = toExpr(pOp.getBlocksize().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, blocksize});
}

PP_GEN_VISIT_DEF(Split, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const auto output_outputs_ntensor = toExpr(pOp.getNumOfOutputs() - 0);
  const auto output_outputs         = defineTensors<TensorType::outputs>(indent, pOp, 0);
  const auto output_outputs_ndim    = defineDimensionNumberArray<TensorType::outputs>(stream, indent, pOp, 0);
  const auto output_outputs_dims    = defineDimensionArrays<TensorType::outputs>(stream, indent, pOp, 0);
  // Prepare attributes
  const auto axis            = toExpr(pOp.getAxis().value());
  const auto number_of_split = toExpr(pOp.getSplit().vector().size());
  const auto split           = defineArray<int32_t>(stream, indent, pOp.getSplit().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float**>(output_outputs),
          output_outputs_ntensor, output_outputs_ndim, output_outputs_dims, axis, split, number_of_split});
}

PP_GEN_VISIT_DEF(Sqrt, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims

         });
}

PP_GEN_VISIT_DEF(Squeeze, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_squeezed_t    = pOp.getOutput(0);
  const auto    output_squeezed      = defineTensor(indent, *output_squeezed_t);
  const auto    output_squeezed_ndim = toExpr(output_squeezed_t->getNumOfDimensions());
  const auto    output_squeezed_dims = defineDimensionArray(stream, indent, *output_squeezed_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_squeezed),
          output_squeezed_ndim, output_squeezed_dims, axes, number_of_axes});
}

PP_GEN_VISIT_DEF(Sub, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(Sum, pOp)
{
  // Prepare input
  const auto input_data_0_ntensor = toExpr(pOp.getNumOfInputs() - 0);
  const auto input_data_0         = defineTensors<TensorType::data_0>(indent, pOp, 0);
  const auto input_data_0_ndim    = defineDimensionNumberArray<TensorType::data_0>(stream, indent, pOp, 0);
  const auto input_data_0_dims    = defineDimensionArrays<TensorType::data_0>(stream, indent, pOp, 0);
  // Prepare output
  const Tensor* output_sum_t    = pOp.getOutput(0);
  const auto    output_sum      = defineTensor(indent, *output_sum_t);
  const auto    output_sum_ndim = toExpr(output_sum_t->getNumOfDimensions());
  const auto    output_sum_dims = defineDimensionArray(stream, indent, *output_sum_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float**>(input_data_0), input_data_0_ntensor, input_data_0_ndim, input_data_0_dims,
          castExpr<float*>(output_sum), output_sum_ndim, output_sum_dims

         });
}

PP_GEN_VISIT_DEF(Tan, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(Tanh, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(Tile, pOp)
{
  // Prepare input
  const Tensor* input_input_t      = pOp.getInput(0);
  const auto    input_input        = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim   = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims   = defineDimensionArray(stream, indent, *input_input_t);
  const Tensor* input_repeats_t    = pOp.getInput(1);
  const auto    input_repeats      = defineTensor(indent, *input_repeats_t);
  const auto    input_repeats_ndim = toExpr(input_repeats_t->getNumOfDimensions());
  const auto    input_repeats_dims = defineDimensionArray(stream, indent, *input_repeats_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(input_repeats),
          input_repeats_ndim, input_repeats_dims, castExpr<float*>(output_output), output_output_ndim,
          output_output_dims

         });
}

PP_GEN_VISIT_DEF(TopK, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Values_t     = pOp.getOutput(0);
  const auto    output_Values       = defineTensor(indent, *output_Values_t);
  const auto    output_Values_ndim  = toExpr(output_Values_t->getNumOfDimensions());
  const auto    output_Values_dims  = defineDimensionArray(stream, indent, *output_Values_t);
  const Tensor* output_Indices_t    = pOp.getOutput(1);
  const auto    output_Indices      = defineTensor(indent, *output_Indices_t);
  const auto    output_Indices_ndim = toExpr(output_Indices_t->getNumOfDimensions());
  const auto    output_Indices_dims = defineDimensionArray(stream, indent, *output_Indices_t);
  // Prepare attributes
  const auto axis = toExpr(pOp.getAxis().value());
  const auto k    = toExpr(pOp.getK().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Values),
          output_Values_ndim, output_Values_dims, castExpr<float*>(output_Indices), output_Indices_ndim,
          output_Indices_dims, axis, k});
}

PP_GEN_VISIT_DEF(Transpose, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_transposed_t    = pOp.getOutput(0);
  const auto    output_transposed      = defineTensor(indent, *output_transposed_t);
  const auto    output_transposed_ndim = toExpr(output_transposed_t->getNumOfDimensions());
  const auto    output_transposed_dims = defineDimensionArray(stream, indent, *output_transposed_t);
  // Prepare attributes
  const auto number_of_perm = toExpr(pOp.getPerm().vector().size());
  const auto perm           = defineArray<int32_t>(stream, indent, pOp.getPerm().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_transposed),
          output_transposed_ndim, output_transposed_dims, perm, number_of_perm});
}

PP_GEN_VISIT_DEF(Unsqueeze, pOp)
{
  // Prepare input
  const Tensor* input_data_t    = pOp.getInput(0);
  const auto    input_data      = defineTensor(indent, *input_data_t);
  const auto    input_data_ndim = toExpr(input_data_t->getNumOfDimensions());
  const auto    input_data_dims = defineDimensionArray(stream, indent, *input_data_t);
  // Prepare output
  const Tensor* output_expanded_t    = pOp.getOutput(0);
  const auto    output_expanded      = defineTensor(indent, *output_expanded_t);
  const auto    output_expanded_ndim = toExpr(output_expanded_t->getNumOfDimensions());
  const auto    output_expanded_dims = defineDimensionArray(stream, indent, *output_expanded_t);
  // Prepare attributes
  const auto number_of_axes = toExpr(pOp.getAxes().vector().size());
  const auto axes           = defineArray<int32_t>(stream, indent, pOp.getAxes().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_data), input_data_ndim, input_data_dims, castExpr<float*>(output_expanded),
          output_expanded_ndim, output_expanded_dims, axes, number_of_axes});
}

PP_GEN_VISIT_DEF(Upsample, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto mode             = toExpr(pOp.getMode().value().c_str());
  const auto number_of_scales = toExpr(pOp.getScales().vector().size());
  const auto scales           = defineArray<float>(stream, indent, pOp.getScales().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, mode, scales, number_of_scales});
}

PP_GEN_VISIT_DEF(Xor, pOp)
{
  // Prepare input
  const Tensor* input_A_t    = pOp.getInput(0);
  const auto    input_A      = defineTensor(indent, *input_A_t);
  const auto    input_A_ndim = toExpr(input_A_t->getNumOfDimensions());
  const auto    input_A_dims = defineDimensionArray(stream, indent, *input_A_t);
  const Tensor* input_B_t    = pOp.getInput(1);
  const auto    input_B      = defineTensor(indent, *input_B_t);
  const auto    input_B_ndim = toExpr(input_B_t->getNumOfDimensions());
  const auto    input_B_dims = defineDimensionArray(stream, indent, *input_B_t);
  // Prepare output
  const Tensor* output_C_t    = pOp.getOutput(0);
  const auto    output_C      = defineTensor(indent, *output_C_t);
  const auto    output_C_ndim = toExpr(output_C_t->getNumOfDimensions());
  const auto    output_C_dims = defineDimensionArray(stream, indent, *output_C_t);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_A), input_A_ndim, input_A_dims, castExpr<float*>(input_B), input_B_ndim,
          input_B_dims, castExpr<float*>(output_C), output_C_ndim, output_C_dims

         });
}

PP_GEN_VISIT_DEF(ATen, pOp)
{
  // Prepare input
  const auto input_input_ntensor = toExpr(pOp.getNumOfInputs() - 0);
  const auto input_input         = defineTensors<TensorType::input>(indent, pOp, 0);
  const auto input_input_ndim    = defineDimensionNumberArray<TensorType::input>(stream, indent, pOp, 0);
  const auto input_input_dims    = defineDimensionArrays<TensorType::input>(stream, indent, pOp, 0);
  // Prepare output
  const auto output_output_ntensor = toExpr(pOp.getNumOfOutputs() - 0);
  const auto output_output         = defineTensors<TensorType::output>(indent, pOp, 0);
  const auto output_output_ndim    = defineDimensionNumberArray<TensorType::output>(stream, indent, pOp, 0);
  const auto output_output_dims    = defineDimensionArrays<TensorType::output>(stream, indent, pOp, 0);
  // Prepare attributes

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float**>(input_input), input_input_ntensor, input_input_ndim, input_input_dims,
          castExpr<float**>(output_output), output_output_ntensor, output_output_ndim, output_output_dims

         });
}

PP_GEN_VISIT_DEF(Affine, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());
  const auto beta  = toExpr(pOp.getBeta().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, alpha, beta});
}

PP_GEN_VISIT_DEF(ConstantFill, pOp)
{
  // Prepare input
  const bool has_input_input = (0 < pOp.getNumOfInputs());
  const auto input_input =
    has_input_input ? defineTensor(indent, *pOp.getInput(0)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_input_ndim = has_input_input ? toExpr(pOp.getInput(0)->getNumOfDimensions()) : toExpr(0);
  const auto input_input_dims = has_input_input ? defineDimensionArray(stream, indent, *pOp.getInput(0))
                                                : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto dtype                 = toExpr(pOp.getDtype().value());
  const auto number_of_extra_shape = toExpr(pOp.getExtraShape().vector().size());
  const auto extra_shape           = defineArray<int32_t>(stream, indent, pOp.getExtraShape().vector());
  const auto input_as_shape        = toExpr(pOp.getInputAsShape().value());
  const auto number_of_shape       = toExpr(pOp.getShape().vector().size());
  const auto shape                 = defineArray<int32_t>(stream, indent, pOp.getShape().vector());
  const auto value                 = toExpr(pOp.getValue().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, dtype, extra_shape, number_of_extra_shape, input_as_shape, shape,
          number_of_shape, value});
}

PP_GEN_VISIT_DEF(Crop, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto number_of_border = toExpr(pOp.getBorder().vector().size());
  const auto border           = defineArray<int32_t>(stream, indent, pOp.getBorder().vector());
  const auto number_of_scale  = toExpr(pOp.getScale().vector().size());
  const auto scale            = defineArray<int32_t>(stream, indent, pOp.getScale().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, border, number_of_border, scale, number_of_scale});
}

PP_GEN_VISIT_DEF(GRUUnit, pOp)
{
  // Prepare input
  const Tensor* input_hidden_prev_t    = pOp.getInput(0);
  const auto    input_hidden_prev      = defineTensor(indent, *input_hidden_prev_t);
  const auto    input_hidden_prev_ndim = toExpr(input_hidden_prev_t->getNumOfDimensions());
  const auto    input_hidden_prev_dims = defineDimensionArray(stream, indent, *input_hidden_prev_t);
  const Tensor* input_gates_t          = pOp.getInput(1);
  const auto    input_gates            = defineTensor(indent, *input_gates_t);
  const auto    input_gates_ndim       = toExpr(input_gates_t->getNumOfDimensions());
  const auto    input_gates_dims       = defineDimensionArray(stream, indent, *input_gates_t);
  const Tensor* input_seq_lengths_t    = pOp.getInput(2);
  const auto    input_seq_lengths      = defineTensor(indent, *input_seq_lengths_t);
  const auto    input_seq_lengths_ndim = toExpr(input_seq_lengths_t->getNumOfDimensions());
  const auto    input_seq_lengths_dims = defineDimensionArray(stream, indent, *input_seq_lengths_t);
  const Tensor* input_t_t              = pOp.getInput(3);
  const auto    input_t                = defineTensor(indent, *input_t_t);
  const auto    input_t_ndim           = toExpr(input_t_t->getNumOfDimensions());
  const auto    input_t_dims           = defineDimensionArray(stream, indent, *input_t_t);
  // Prepare output
  const Tensor* output_hidden_t    = pOp.getOutput(0);
  const auto    output_hidden      = defineTensor(indent, *output_hidden_t);
  const auto    output_hidden_ndim = toExpr(output_hidden_t->getNumOfDimensions());
  const auto    output_hidden_dims = defineDimensionArray(stream, indent, *output_hidden_t);
  // Prepare attributes
  const auto drop_states = toExpr(pOp.getDropStates().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_hidden_prev), input_hidden_prev_ndim, input_hidden_prev_dims,
          castExpr<float*>(input_gates), input_gates_ndim, input_gates_dims, castExpr<float*>(input_seq_lengths),
          input_seq_lengths_ndim, input_seq_lengths_dims, castExpr<float*>(input_t), input_t_ndim, input_t_dims,
          castExpr<float*>(output_hidden), output_hidden_ndim, output_hidden_dims, drop_states});
}

PP_GEN_VISIT_DEF(GivenTensorFill, pOp)
{
  // Prepare input
  const bool has_input_shape = (0 < pOp.getNumOfInputs());
  const auto input_shape =
    has_input_shape ? defineTensor(indent, *pOp.getInput(0)) : defineVarByExpr<void*>(stream, indent, "NULL");
  const auto input_shape_ndim = has_input_shape ? toExpr(pOp.getInput(0)->getNumOfDimensions()) : toExpr(0);
  const auto input_shape_dims = has_input_shape ? defineDimensionArray(stream, indent, *pOp.getInput(0))
                                                : defineVarByExpr<int32_t*>(stream, indent, "NULL");
  // Prepare output
  const Tensor* output_X_t    = pOp.getOutput(0);
  const auto    output_X      = defineTensor(indent, *output_X_t);
  const auto    output_X_ndim = toExpr(output_X_t->getNumOfDimensions());
  const auto    output_X_dims = defineDimensionArray(stream, indent, *output_X_t);
  // Prepare attributes
  const auto number_of_extra_shape = toExpr(pOp.getExtraShape().vector().size());
  const auto extra_shape           = defineArray<int32_t>(stream, indent, pOp.getExtraShape().vector());
  const auto input_as_shape        = toExpr(pOp.getInputAsShape().value());
  const auto number_of_shape       = toExpr(pOp.getShape().vector().size());
  const auto shape                 = defineArray<int32_t>(stream, indent, pOp.getShape().vector());
  const auto number_of_values      = toExpr(pOp.getValues().vector().size());
  const auto values                = defineArray<float>(stream, indent, pOp.getValues().vector());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_shape), input_shape_ndim, input_shape_dims, castExpr<float*>(output_X),
          output_X_ndim, output_X_dims, extra_shape, number_of_extra_shape, input_as_shape, shape, number_of_shape,
          values, number_of_values});
}

PP_GEN_VISIT_DEF(ImageScaler, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto number_of_bias = toExpr(pOp.getBias().vector().size());
  const auto bias           = defineArray<float>(stream, indent, pOp.getBias().vector());
  const auto scale          = toExpr(pOp.getScale().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, bias, number_of_bias, scale});
}

PP_GEN_VISIT_DEF(MeanVarianceNormalization, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto across_channels    = toExpr(pOp.getAcrossChannels().value());
  const auto normalize_variance = toExpr(pOp.getNormalizeVariance().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, across_channels, normalize_variance});
}

PP_GEN_VISIT_DEF(ParametricSoftplus, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());
  const auto beta  = toExpr(pOp.getBeta().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, alpha, beta});
}

PP_GEN_VISIT_DEF(Scale, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto scale = toExpr(pOp.getScale().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, scale});
}

PP_GEN_VISIT_DEF(ScaledTanh, pOp)
{
  // Prepare input
  const Tensor* input_input_t    = pOp.getInput(0);
  const auto    input_input      = defineTensor(indent, *input_input_t);
  const auto    input_input_ndim = toExpr(input_input_t->getNumOfDimensions());
  const auto    input_input_dims = defineDimensionArray(stream, indent, *input_input_t);
  // Prepare output
  const Tensor* output_output_t    = pOp.getOutput(0);
  const auto    output_output      = defineTensor(indent, *output_output_t);
  const auto    output_output_ndim = toExpr(output_output_t->getNumOfDimensions());
  const auto    output_output_dims = defineDimensionArray(stream, indent, *output_output_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());
  const auto beta  = toExpr(pOp.getBeta().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_input), input_input_ndim, input_input_dims, castExpr<float*>(output_output),
          output_output_ndim, output_output_dims, alpha, beta});
}

PP_GEN_VISIT_DEF(ThresholdedRelu, pOp)
{
  // Prepare input
  const Tensor* input_X_t    = pOp.getInput(0);
  const auto    input_X      = defineTensor(indent, *input_X_t);
  const auto    input_X_ndim = toExpr(input_X_t->getNumOfDimensions());
  const auto    input_X_dims = defineDimensionArray(stream, indent, *input_X_t);
  // Prepare output
  const Tensor* output_Y_t    = pOp.getOutput(0);
  const auto    output_Y      = defineTensor(indent, *output_Y_t);
  const auto    output_Y_ndim = toExpr(output_Y_t->getNumOfDimensions());
  const auto    output_Y_dims = defineDimensionArray(stream, indent, *output_Y_t);
  // Prepare attributes
  const auto alpha = toExpr(pOp.getAlpha().value());

  // Call to Runtime
  invoke(stream, indent, target,
         {"NULL", castExpr<float*>(input_X), input_X_ndim, input_X_dims, castExpr<float*>(output_Y), output_Y_ndim,
          output_Y_dims, alpha});
}
