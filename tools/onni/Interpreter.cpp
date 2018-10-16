//===- Interpreter.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Interpreter.h"
#include <onnc/Support/IOStream.h>

#include <onnc/IR/Compute/Abs.h>
#include <onnc/IR/Compute/Acos.h>
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/Compute/And.h>
#include <onnc/IR/Compute/ArgMax.h>
#include <onnc/IR/Compute/ArgMin.h>
#include <onnc/IR/Compute/Asin.h>
#include <onnc/IR/Compute/Atan.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/BatchNormalization.h>
#include <onnc/IR/Compute/Cast.h>
#include <onnc/IR/Compute/Ceil.h>
#include <onnc/IR/Compute/Clip.h>
#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Constant.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/ConvTranspose.h>
#include <onnc/IR/Compute/Cos.h>
#include <onnc/IR/Compute/DepthToSpace.h>
#include <onnc/IR/Compute/Div.h>
#include <onnc/IR/Compute/Dropout.h>
#include <onnc/IR/Compute/Elu.h>
#include <onnc/IR/Compute/Equal.h>
#include <onnc/IR/Compute/Exp.h>
#include <onnc/IR/Compute/Expand.h>
#include <onnc/IR/Compute/Flatten.h>
#include <onnc/IR/Compute/Floor.h>
#include <onnc/IR/Compute/GRU.h>
#include <onnc/IR/Compute/Gather.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/GlobalAveragePool.h>
#include <onnc/IR/Compute/GlobalLpPool.h>
#include <onnc/IR/Compute/GlobalMaxPool.h>
#include <onnc/IR/Compute/Greater.h>
#include <onnc/IR/Compute/HardSigmoid.h>
#include <onnc/IR/Compute/Hardmax.h>
#include <onnc/IR/Compute/Identity.h>
#include <onnc/IR/Compute/InstanceNormalization.h>
#include <onnc/IR/Compute/LRN.h>
#include <onnc/IR/Compute/LSTM.h>
#include <onnc/IR/Compute/LeakyRelu.h>
#include <onnc/IR/Compute/Less.h>
#include <onnc/IR/Compute/Log.h>
#include <onnc/IR/Compute/LogSoftmax.h>
#include <onnc/IR/Compute/LpNormalization.h>
#include <onnc/IR/Compute/LpPool.h>
#include <onnc/IR/Compute/MatMul.h>
#include <onnc/IR/Compute/Max.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/MaxRoiPool.h>
#include <onnc/IR/Compute/Mean.h>
#include <onnc/IR/Compute/Min.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/Multinomial.h>
#include <onnc/IR/Compute/Neg.h>
#include <onnc/IR/Compute/Not.h>
#include <onnc/IR/Compute/Or.h>
#include <onnc/IR/Compute/PRelu.h>
#include <onnc/IR/Compute/Pad.h>
#include <onnc/IR/Compute/Pow.h>
#include <onnc/IR/Compute/RNN.h>
#include <onnc/IR/Compute/RandomNormal.h>
#include <onnc/IR/Compute/RandomNormalLike.h>
#include <onnc/IR/Compute/RandomUniform.h>
#include <onnc/IR/Compute/RandomUniformLike.h>
#include <onnc/IR/Compute/Reciprocal.h>
#include <onnc/IR/Compute/ReduceL1.h>
#include <onnc/IR/Compute/ReduceL2.h>
#include <onnc/IR/Compute/ReduceLogSum.h>
#include <onnc/IR/Compute/ReduceLogSumExp.h>
#include <onnc/IR/Compute/ReduceMax.h>
#include <onnc/IR/Compute/ReduceMean.h>
#include <onnc/IR/Compute/ReduceMin.h>
#include <onnc/IR/Compute/ReduceProd.h>
#include <onnc/IR/Compute/ReduceSum.h>
#include <onnc/IR/Compute/ReduceSumSquare.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Selu.h>
#include <onnc/IR/Compute/Shape.h>
#include <onnc/IR/Compute/Sigmoid.h>
#include <onnc/IR/Compute/Sin.h>
#include <onnc/IR/Compute/Size.h>
#include <onnc/IR/Compute/Slice.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/IR/Compute/Softplus.h>
#include <onnc/IR/Compute/Softsign.h>
#include <onnc/IR/Compute/SpaceToDepth.h>
#include <onnc/IR/Compute/Split.h>
#include <onnc/IR/Compute/Sqrt.h>
#include <onnc/IR/Compute/Squeeze.h>
#include <onnc/IR/Compute/Sub.h>
#include <onnc/IR/Compute/Sum.h>
#include <onnc/IR/Compute/Tan.h>
#include <onnc/IR/Compute/Tanh.h>
#include <onnc/IR/Compute/Tile.h>
#include <onnc/IR/Compute/TopK.h>
#include <onnc/IR/Compute/Transpose.h>
#include <onnc/IR/Compute/Unsqueeze.h>
#include <onnc/IR/Compute/Upsample.h>
#include <onnc/IR/Compute/Xor.h>
#include <onnc/IR/Compute/ATen.h>
#include <onnc/IR/Compute/Affine.h>
#include <onnc/IR/Compute/ConstantFill.h>
#include <onnc/IR/Compute/Crop.h>
#include <onnc/IR/Compute/GRUUnit.h>
#include <onnc/IR/Compute/GivenTensorFill.h>
#include <onnc/IR/Compute/ImageScaler.h>
#include <onnc/IR/Compute/MeanVarianceNormalization.h>
#include <onnc/IR/Compute/ParametricSoftplus.h>
#include <onnc/IR/Compute/Scale.h>
#include <onnc/IR/Compute/ScaledTanh.h>
#include <onnc/IR/Compute/ThresholdedRelu.h>

#define restrict __restrict__
extern "C" {
#include <onnc/Runtime/onnc-runtime.h>
}
#undef restrict

using namespace onnc;

//===----------------------------------------------------------------------===//
// Interpreter
//===----------------------------------------------------------------------===//
void Interpreter::visit(Abs& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_abs_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Acos& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_acos_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(Add& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_add_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(And& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_and_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(ArgMax& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_argmax_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axis
    , keepdims
  );
};


void Interpreter::visit(ArgMin& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_argmin_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axis
    , keepdims
  );
};


void Interpreter::visit(Asin& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_asin_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(Atan& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_atan_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(AveragePool& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  const char * auto_pad = pOp.getAutoPad().value().c_str();
  int32_t count_include_pad = pOp.getCountIncludePad().value();
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pads[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i) pads[i] = pOp.getPads().at(i);
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i) strides[i] = pOp.getStrides().at(i);

  // Call to Runtime
  ONNC_RUNTIME_averagepool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , auto_pad
    , count_include_pad
    , kernel_shape
    , number_of_kernel_shape
    , pads
    , number_of_pads
    , strides
    , number_of_strides
  );
};


void Interpreter::visit(BatchNormalization& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_scale_t = pOp.getInput(1);
  void *input_scale = m_ATable[input_scale_t];
  int32_t input_scale_ndim = input_scale_t->getNumOfDimensions();
  int32_t input_scale_dims[input_scale_ndim];
  for (int i = 0; i < input_scale_ndim; ++i) input_scale_dims[i] = input_scale_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(2);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  Tensor *input_mean_t = pOp.getInput(3);
  void *input_mean = m_ATable[input_mean_t];
  int32_t input_mean_ndim = input_mean_t->getNumOfDimensions();
  int32_t input_mean_dims[input_mean_ndim];
  for (int i = 0; i < input_mean_ndim; ++i) input_mean_dims[i] = input_mean_t->dimension(i);
  Tensor *input_var_t = pOp.getInput(4);
  void *input_var = m_ATable[input_var_t];
  int32_t input_var_ndim = input_var_t->getNumOfDimensions();
  int32_t input_var_dims[input_var_ndim];
  for (int i = 0; i < input_var_ndim; ++i) input_var_dims[i] = input_var_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  Tensor *output_mean_t = NULL;
  void *output_mean = NULL;
  int32_t output_mean_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_mean_t = pOp.getOutput(1);
    output_mean = m_ATable[output_mean_t];
    output_mean_ndim = output_mean_t->getNumOfDimensions();
  }
  int32_t output_mean_dims[output_mean_ndim];
  for (int i = 0; i < output_mean_ndim; ++i) output_mean_dims[i] = output_mean_t->dimension(i);
  Tensor *output_var_t = NULL;
  void *output_var = NULL;
  int32_t output_var_ndim = 0;
  if (pOp.getNumOfOutputs() > 2) {
    output_var_t = pOp.getOutput(2);
    output_var = m_ATable[output_var_t];
    output_var_ndim = output_var_t->getNumOfDimensions();
  }
  int32_t output_var_dims[output_var_ndim];
  for (int i = 0; i < output_var_ndim; ++i) output_var_dims[i] = output_var_t->dimension(i);
  Tensor *output_saved_mean_t = NULL;
  void *output_saved_mean = NULL;
  int32_t output_saved_mean_ndim = 0;
  if (pOp.getNumOfOutputs() > 3) {
    output_saved_mean_t = pOp.getOutput(3);
    output_saved_mean = m_ATable[output_saved_mean_t];
    output_saved_mean_ndim = output_saved_mean_t->getNumOfDimensions();
  }
  int32_t output_saved_mean_dims[output_saved_mean_ndim];
  for (int i = 0; i < output_saved_mean_ndim; ++i) output_saved_mean_dims[i] = output_saved_mean_t->dimension(i);
  Tensor *output_saved_var_t = NULL;
  void *output_saved_var = NULL;
  int32_t output_saved_var_ndim = 0;
  if (pOp.getNumOfOutputs() > 4) {
    output_saved_var_t = pOp.getOutput(4);
    output_saved_var = m_ATable[output_saved_var_t];
    output_saved_var_ndim = output_saved_var_t->getNumOfDimensions();
  }
  int32_t output_saved_var_dims[output_saved_var_ndim];
  for (int i = 0; i < output_saved_var_ndim; ++i) output_saved_var_dims[i] = output_saved_var_t->dimension(i);
  // Prepare attributes
  float epsilon = pOp.getEpsilon().value();
  float momentum = pOp.getMomentum().value();
  int32_t spatial = pOp.getSpatial().value();

  // Call to Runtime
  ONNC_RUNTIME_batchnormalization_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_scale)
    , input_scale_ndim, input_scale_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(input_mean)
    , input_mean_ndim, input_mean_dims
    , reinterpret_cast<float *>(input_var)
    , input_var_ndim, input_var_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , reinterpret_cast<float *>(output_mean)
    , output_mean_ndim, output_mean_dims
    , reinterpret_cast<float *>(output_var)
    , output_var_ndim, output_var_dims
    , reinterpret_cast<float *>(output_saved_mean)
    , output_saved_mean_ndim, output_saved_mean_dims
    , reinterpret_cast<float *>(output_saved_var)
    , output_saved_var_ndim, output_saved_var_dims
    , epsilon
    , momentum
    , spatial
  );
};


void Interpreter::visit(Cast& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t to = pOp.getTo().value();

  // Call to Runtime
  ONNC_RUNTIME_cast_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , to
  );
};


void Interpreter::visit(Ceil& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_ceil_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Clip& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  float max = pOp.getMax().value();
  float min = pOp.getMin().value();

  // Call to Runtime
  ONNC_RUNTIME_clip_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , max
    , min
  );
};


void Interpreter::visit(Concat& pOp) {
  // Prepare input
  int32_t input_inputs_ntensor = pOp.getNumOfInputs() - 0;
  void *input_inputs[input_inputs_ntensor];
  int32_t input_inputs_ndim[input_inputs_ntensor];
  int32_t *input_inputs_dims[input_inputs_ntensor];
  for (int i = 0; i < input_inputs_ntensor; ++i){
    input_inputs[i] = m_ATable[pOp.getInput(0 + i)];
    input_inputs_ndim[i] = pOp.getInput(0 + i)->getNumOfDimensions();
    input_inputs_dims[i] = new int32_t[input_inputs_ndim[i]];
    for(int32_t j = 0; j < input_inputs_ndim[i]; ++j){
      input_inputs_dims[i][j] = pOp.getInput(0 + i)->dimension(j);
    }
  }
  // Prepare output
  Tensor *output_concat_result_t = pOp.getOutput(0);
  void *output_concat_result = m_ATable[output_concat_result_t];
  int32_t output_concat_result_ndim = output_concat_result_t->getNumOfDimensions();
  int32_t output_concat_result_dims[output_concat_result_ndim];
  for (int i = 0; i < output_concat_result_ndim; ++i) output_concat_result_dims[i] = output_concat_result_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();

  // Call to Runtime
  ONNC_RUNTIME_concat_float(
    m_pContext
    , reinterpret_cast<float **>(input_inputs)
    , input_inputs_ntensor
    , input_inputs_ndim, input_inputs_dims
    , reinterpret_cast<float *>(output_concat_result)
    , output_concat_result_ndim, output_concat_result_dims
    , axis
  );

  // Clean
  for (int i = 0; i < input_inputs_ntensor; ++i){
    delete [] input_inputs_dims[i];
  }
};


void Interpreter::visit(Constant& pOp) {
  // Prepare input
  
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  const void * value = &pOp.getValue().value();

  // Call to Runtime
  ONNC_RUNTIME_constant_float(
    m_pContext
    
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , value
  );
};


void Interpreter::visit(Conv& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_W_t = pOp.getInput(1);
  void *input_W = m_ATable[input_W_t];
  int32_t input_W_ndim = input_W_t->getNumOfDimensions();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_t->dimension(i);
  Tensor *input_B_t = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pOp.getNumOfInputs() > 2) {
    input_B_t = pOp.getInput(2);
    input_B = m_ATable[input_B_t];
    input_B_ndim = input_B_t->getNumOfDimensions();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  const char * auto_pad = pOp.getAutoPad().value().c_str();
  int32_t number_of_dilations = pOp.getDilations().vector().size();
  int32_t dilations[number_of_dilations];
  for (int i = 0; i < number_of_dilations; ++i) dilations[i] = pOp.getDilations().at(i);
  int32_t group = pOp.getGroup().value();
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pads[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i) pads[i] = pOp.getPads().at(i);
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i) strides[i] = pOp.getStrides().at(i);

  // Call to Runtime
  ONNC_RUNTIME_conv_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_W)
    , input_W_ndim, input_W_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , auto_pad
    , dilations
    , number_of_dilations
    , group
    , kernel_shape
    , number_of_kernel_shape
    , pads
    , number_of_pads
    , strides
    , number_of_strides
  );
};


void Interpreter::visit(ConvTranspose& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_W_t = pOp.getInput(1);
  void *input_W = m_ATable[input_W_t];
  int32_t input_W_ndim = input_W_t->getNumOfDimensions();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_t->dimension(i);
  Tensor *input_B_t = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pOp.getNumOfInputs() > 2) {
    input_B_t = pOp.getInput(2);
    input_B = m_ATable[input_B_t];
    input_B_ndim = input_B_t->getNumOfDimensions();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  const char * auto_pad = pOp.getAutoPad().value().c_str();
  int32_t number_of_dilations = pOp.getDilations().vector().size();
  int32_t dilations[number_of_dilations];
  for (int i = 0; i < number_of_dilations; ++i) dilations[i] = pOp.getDilations().at(i);
  int32_t group = pOp.getGroup().value();
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_output_padding = pOp.getOutputPadding().vector().size();
  int32_t output_padding[number_of_output_padding];
  for (int i = 0; i < number_of_output_padding; ++i) output_padding[i] = pOp.getOutputPadding().at(i);
  int32_t number_of_output_shape = pOp.getOutputShape().vector().size();
  int32_t output_shape[number_of_output_shape];
  for (int i = 0; i < number_of_output_shape; ++i) output_shape[i] = pOp.getOutputShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pads[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i) pads[i] = pOp.getPads().at(i);
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i) strides[i] = pOp.getStrides().at(i);

  // Call to Runtime
  ONNC_RUNTIME_convtranspose_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_W)
    , input_W_ndim, input_W_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , auto_pad
    , dilations
    , number_of_dilations
    , group
    , kernel_shape
    , number_of_kernel_shape
    , output_padding
    , number_of_output_padding
    , output_shape
    , number_of_output_shape
    , pads
    , number_of_pads
    , strides
    , number_of_strides
  );
};


void Interpreter::visit(Cos& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_cos_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(DepthToSpace& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t blocksize = pOp.getBlocksize().value();

  // Call to Runtime
  ONNC_RUNTIME_depthtospace_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , blocksize
  );
};


void Interpreter::visit(Div& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_div_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(Dropout& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  Tensor *output_mask_t = NULL;
  void *output_mask = NULL;
  int32_t output_mask_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_mask_t = pOp.getOutput(1);
    output_mask = m_ATable[output_mask_t];
    output_mask_ndim = output_mask_t->getNumOfDimensions();
  }
  int32_t output_mask_dims[output_mask_ndim];
  for (int i = 0; i < output_mask_ndim; ++i) output_mask_dims[i] = output_mask_t->dimension(i);
  // Prepare attributes
  float ratio = pOp.getRatio().value();

  // Call to Runtime
  ONNC_RUNTIME_dropout_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , reinterpret_cast<float *>(output_mask)
    , output_mask_ndim, output_mask_dims
    , ratio
  );
};


void Interpreter::visit(Elu& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();

  // Call to Runtime
  ONNC_RUNTIME_elu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
  );
};


void Interpreter::visit(Equal& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_equal_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(Exp& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_exp_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(Expand& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  Tensor *input_shape_t = pOp.getInput(1);
  void *input_shape = m_ATable[input_shape_t];
  int32_t input_shape_ndim = input_shape_t->getNumOfDimensions();
  int32_t input_shape_dims[input_shape_ndim];
  for (int i = 0; i < input_shape_ndim; ++i) input_shape_dims[i] = input_shape_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_expand_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(input_shape)
    , input_shape_ndim, input_shape_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(Flatten& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();

  // Call to Runtime
  ONNC_RUNTIME_flatten_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visit(Floor& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_floor_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(GRU& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_W_t = pOp.getInput(1);
  void *input_W = m_ATable[input_W_t];
  int32_t input_W_ndim = input_W_t->getNumOfDimensions();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_t->dimension(i);
  Tensor *input_R_t = pOp.getInput(2);
  void *input_R = m_ATable[input_R_t];
  int32_t input_R_ndim = input_R_t->getNumOfDimensions();
  int32_t input_R_dims[input_R_ndim];
  for (int i = 0; i < input_R_ndim; ++i) input_R_dims[i] = input_R_t->dimension(i);
  Tensor *input_B_t = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pOp.getNumOfInputs() > 3) {
    input_B_t = pOp.getInput(3);
    input_B = m_ATable[input_B_t];
    input_B_ndim = input_B_t->getNumOfDimensions();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  Tensor *input_sequence_lens_t = NULL;
  void *input_sequence_lens = NULL;
  int32_t input_sequence_lens_ndim = 0;
  if (pOp.getNumOfInputs() > 4) {
    input_sequence_lens_t = pOp.getInput(4);
    input_sequence_lens = m_ATable[input_sequence_lens_t];
    input_sequence_lens_ndim = input_sequence_lens_t->getNumOfDimensions();
  }
  int32_t input_sequence_lens_dims[input_sequence_lens_ndim];
  for (int i = 0; i < input_sequence_lens_ndim; ++i) input_sequence_lens_dims[i] = input_sequence_lens_t->dimension(i);
  Tensor *input_initial_h_t = NULL;
  void *input_initial_h = NULL;
  int32_t input_initial_h_ndim = 0;
  if (pOp.getNumOfInputs() > 5) {
    input_initial_h_t = pOp.getInput(5);
    input_initial_h = m_ATable[input_initial_h_t];
    input_initial_h_ndim = input_initial_h_t->getNumOfDimensions();
  }
  int32_t input_initial_h_dims[input_initial_h_ndim];
  for (int i = 0; i < input_initial_h_ndim; ++i) input_initial_h_dims[i] = input_initial_h_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = NULL;
  void *output_Y = NULL;
  int32_t output_Y_ndim = 0;
  if (pOp.getNumOfOutputs() > 0) {
    output_Y_t = pOp.getOutput(0);
    output_Y = m_ATable[output_Y_t];
    output_Y_ndim = output_Y_t->getNumOfDimensions();
  }
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  Tensor *output_Y_h_t = NULL;
  void *output_Y_h = NULL;
  int32_t output_Y_h_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_Y_h_t = pOp.getOutput(1);
    output_Y_h = m_ATable[output_Y_h_t];
    output_Y_h_ndim = output_Y_h_t->getNumOfDimensions();
  }
  int32_t output_Y_h_dims[output_Y_h_ndim];
  for (int i = 0; i < output_Y_h_ndim; ++i) output_Y_h_dims[i] = output_Y_h_t->dimension(i);
  // Prepare attributes
  int32_t number_of_activation_alpha = pOp.getActivationAlpha().vector().size();
  float activation_alpha[number_of_activation_alpha];
  for (int i = 0; i < number_of_activation_alpha; ++i) activation_alpha[i] = pOp.getActivationAlpha().at(i);
  int32_t number_of_activation_beta = pOp.getActivationBeta().vector().size();
  float activation_beta[number_of_activation_beta];
  for (int i = 0; i < number_of_activation_beta; ++i) activation_beta[i] = pOp.getActivationBeta().at(i);
  int32_t number_of_activations = pOp.getActivations().vector().size();
  const char * activations[number_of_activations];
  for (int i = 0; i < number_of_activations; ++i) activations[i] = pOp.getActivations().at(i).c_str();
  float clip = pOp.getClip().value();
  const char * direction = pOp.getDirection().value().c_str();
  int32_t hidden_size = pOp.getHiddenSize().value();
  int32_t linear_before_reset = pOp.getLinearBeforeReset().value();

  // Call to Runtime
  ONNC_RUNTIME_gru_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_W)
    , input_W_ndim, input_W_dims
    , reinterpret_cast<float *>(input_R)
    , input_R_ndim, input_R_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(input_sequence_lens)
    , input_sequence_lens_ndim, input_sequence_lens_dims
    , reinterpret_cast<float *>(input_initial_h)
    , input_initial_h_ndim, input_initial_h_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , reinterpret_cast<float *>(output_Y_h)
    , output_Y_h_ndim, output_Y_h_dims
    , activation_alpha
    , number_of_activation_alpha
    , activation_beta
    , number_of_activation_beta
    , activations
    , number_of_activations
    , clip
    , direction
    , hidden_size
    , linear_before_reset
  );
};


void Interpreter::visit(Gather& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  Tensor *input_indices_t = pOp.getInput(1);
  void *input_indices = m_ATable[input_indices_t];
  int32_t input_indices_ndim = input_indices_t->getNumOfDimensions();
  int32_t input_indices_dims[input_indices_ndim];
  for (int i = 0; i < input_indices_ndim; ++i) input_indices_dims[i] = input_indices_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();

  // Call to Runtime
  ONNC_RUNTIME_gather_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(input_indices)
    , input_indices_ndim, input_indices_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visit(Gemm& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  Tensor *input_C_t = pOp.getInput(2);
  void *input_C = m_ATable[input_C_t];
  int32_t input_C_ndim = input_C_t->getNumOfDimensions();
  int32_t input_C_dims[input_C_ndim];
  for (int i = 0; i < input_C_ndim; ++i) input_C_dims[i] = input_C_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();
  float beta = pOp.getBeta().value();
  int32_t transA = pOp.getTransA().value();
  int32_t transB = pOp.getTransB().value();

  // Call to Runtime
  ONNC_RUNTIME_gemm_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(input_C)
    , input_C_ndim, input_C_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
    , beta
    , transA
    , transB
  );
};


void Interpreter::visit(GlobalAveragePool& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_globalaveragepool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(GlobalLpPool& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  int32_t p = pOp.getP().value();

  // Call to Runtime
  ONNC_RUNTIME_globallppool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , p
  );
};


void Interpreter::visit(GlobalMaxPool& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_globalmaxpool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Greater& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_greater_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(HardSigmoid& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();
  float beta = pOp.getBeta().value();

  // Call to Runtime
  ONNC_RUNTIME_hardsigmoid_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
    , beta
  );
};


void Interpreter::visit(Hardmax& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();

  // Call to Runtime
  ONNC_RUNTIME_hardmax_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visit(Identity& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_identity_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(InstanceNormalization& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  Tensor *input_scale_t = pOp.getInput(1);
  void *input_scale = m_ATable[input_scale_t];
  int32_t input_scale_ndim = input_scale_t->getNumOfDimensions();
  int32_t input_scale_dims[input_scale_ndim];
  for (int i = 0; i < input_scale_ndim; ++i) input_scale_dims[i] = input_scale_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(2);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  float epsilon = pOp.getEpsilon().value();

  // Call to Runtime
  ONNC_RUNTIME_instancenormalization_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(input_scale)
    , input_scale_ndim, input_scale_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , epsilon
  );
};


void Interpreter::visit(LRN& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();
  float beta = pOp.getBeta().value();
  float bias = pOp.getBias().value();
  int32_t size = pOp.getSize().value();

  // Call to Runtime
  ONNC_RUNTIME_lrn_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
    , beta
    , bias
    , size
  );
};


void Interpreter::visit(LSTM& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_W_t = pOp.getInput(1);
  void *input_W = m_ATable[input_W_t];
  int32_t input_W_ndim = input_W_t->getNumOfDimensions();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_t->dimension(i);
  Tensor *input_R_t = pOp.getInput(2);
  void *input_R = m_ATable[input_R_t];
  int32_t input_R_ndim = input_R_t->getNumOfDimensions();
  int32_t input_R_dims[input_R_ndim];
  for (int i = 0; i < input_R_ndim; ++i) input_R_dims[i] = input_R_t->dimension(i);
  Tensor *input_B_t = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pOp.getNumOfInputs() > 3) {
    input_B_t = pOp.getInput(3);
    input_B = m_ATable[input_B_t];
    input_B_ndim = input_B_t->getNumOfDimensions();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  Tensor *input_sequence_lens_t = NULL;
  void *input_sequence_lens = NULL;
  int32_t input_sequence_lens_ndim = 0;
  if (pOp.getNumOfInputs() > 4) {
    input_sequence_lens_t = pOp.getInput(4);
    input_sequence_lens = m_ATable[input_sequence_lens_t];
    input_sequence_lens_ndim = input_sequence_lens_t->getNumOfDimensions();
  }
  int32_t input_sequence_lens_dims[input_sequence_lens_ndim];
  for (int i = 0; i < input_sequence_lens_ndim; ++i) input_sequence_lens_dims[i] = input_sequence_lens_t->dimension(i);
  Tensor *input_initial_h_t = NULL;
  void *input_initial_h = NULL;
  int32_t input_initial_h_ndim = 0;
  if (pOp.getNumOfInputs() > 5) {
    input_initial_h_t = pOp.getInput(5);
    input_initial_h = m_ATable[input_initial_h_t];
    input_initial_h_ndim = input_initial_h_t->getNumOfDimensions();
  }
  int32_t input_initial_h_dims[input_initial_h_ndim];
  for (int i = 0; i < input_initial_h_ndim; ++i) input_initial_h_dims[i] = input_initial_h_t->dimension(i);
  Tensor *input_initial_c_t = NULL;
  void *input_initial_c = NULL;
  int32_t input_initial_c_ndim = 0;
  if (pOp.getNumOfInputs() > 6) {
    input_initial_c_t = pOp.getInput(6);
    input_initial_c = m_ATable[input_initial_c_t];
    input_initial_c_ndim = input_initial_c_t->getNumOfDimensions();
  }
  int32_t input_initial_c_dims[input_initial_c_ndim];
  for (int i = 0; i < input_initial_c_ndim; ++i) input_initial_c_dims[i] = input_initial_c_t->dimension(i);
  Tensor *input_P_t = NULL;
  void *input_P = NULL;
  int32_t input_P_ndim = 0;
  if (pOp.getNumOfInputs() > 7) {
    input_P_t = pOp.getInput(7);
    input_P = m_ATable[input_P_t];
    input_P_ndim = input_P_t->getNumOfDimensions();
  }
  int32_t input_P_dims[input_P_ndim];
  for (int i = 0; i < input_P_ndim; ++i) input_P_dims[i] = input_P_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = NULL;
  void *output_Y = NULL;
  int32_t output_Y_ndim = 0;
  if (pOp.getNumOfOutputs() > 0) {
    output_Y_t = pOp.getOutput(0);
    output_Y = m_ATable[output_Y_t];
    output_Y_ndim = output_Y_t->getNumOfDimensions();
  }
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  Tensor *output_Y_h_t = NULL;
  void *output_Y_h = NULL;
  int32_t output_Y_h_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_Y_h_t = pOp.getOutput(1);
    output_Y_h = m_ATable[output_Y_h_t];
    output_Y_h_ndim = output_Y_h_t->getNumOfDimensions();
  }
  int32_t output_Y_h_dims[output_Y_h_ndim];
  for (int i = 0; i < output_Y_h_ndim; ++i) output_Y_h_dims[i] = output_Y_h_t->dimension(i);
  Tensor *output_Y_c_t = NULL;
  void *output_Y_c = NULL;
  int32_t output_Y_c_ndim = 0;
  if (pOp.getNumOfOutputs() > 2) {
    output_Y_c_t = pOp.getOutput(2);
    output_Y_c = m_ATable[output_Y_c_t];
    output_Y_c_ndim = output_Y_c_t->getNumOfDimensions();
  }
  int32_t output_Y_c_dims[output_Y_c_ndim];
  for (int i = 0; i < output_Y_c_ndim; ++i) output_Y_c_dims[i] = output_Y_c_t->dimension(i);
  // Prepare attributes
  int32_t number_of_activation_alpha = pOp.getActivationAlpha().vector().size();
  float activation_alpha[number_of_activation_alpha];
  for (int i = 0; i < number_of_activation_alpha; ++i) activation_alpha[i] = pOp.getActivationAlpha().at(i);
  int32_t number_of_activation_beta = pOp.getActivationBeta().vector().size();
  float activation_beta[number_of_activation_beta];
  for (int i = 0; i < number_of_activation_beta; ++i) activation_beta[i] = pOp.getActivationBeta().at(i);
  int32_t number_of_activations = pOp.getActivations().vector().size();
  const char * activations[number_of_activations];
  for (int i = 0; i < number_of_activations; ++i) activations[i] = pOp.getActivations().at(i).c_str();
  float clip = pOp.getClip().value();
  const char * direction = pOp.getDirection().value().c_str();
  int32_t hidden_size = pOp.getHiddenSize().value();
  int32_t input_forget = pOp.getInputForget().value();

  // Call to Runtime
  ONNC_RUNTIME_lstm_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_W)
    , input_W_ndim, input_W_dims
    , reinterpret_cast<float *>(input_R)
    , input_R_ndim, input_R_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(input_sequence_lens)
    , input_sequence_lens_ndim, input_sequence_lens_dims
    , reinterpret_cast<float *>(input_initial_h)
    , input_initial_h_ndim, input_initial_h_dims
    , reinterpret_cast<float *>(input_initial_c)
    , input_initial_c_ndim, input_initial_c_dims
    , reinterpret_cast<float *>(input_P)
    , input_P_ndim, input_P_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , reinterpret_cast<float *>(output_Y_h)
    , output_Y_h_ndim, output_Y_h_dims
    , reinterpret_cast<float *>(output_Y_c)
    , output_Y_c_ndim, output_Y_c_dims
    , activation_alpha
    , number_of_activation_alpha
    , activation_beta
    , number_of_activation_beta
    , activations
    , number_of_activations
    , clip
    , direction
    , hidden_size
    , input_forget
  );
};


void Interpreter::visit(LeakyRelu& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();

  // Call to Runtime
  ONNC_RUNTIME_leakyrelu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
  );
};


void Interpreter::visit(Less& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_less_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(Log& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_log_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(LogSoftmax& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();

  // Call to Runtime
  ONNC_RUNTIME_logsoftmax_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visit(LpNormalization& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();
  int32_t p = pOp.getP().value();

  // Call to Runtime
  ONNC_RUNTIME_lpnormalization_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
    , p
  );
};


void Interpreter::visit(LpPool& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  const char * auto_pad = pOp.getAutoPad().value().c_str();
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t p = pOp.getP().value();
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pads[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i) pads[i] = pOp.getPads().at(i);
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i) strides[i] = pOp.getStrides().at(i);

  // Call to Runtime
  ONNC_RUNTIME_lppool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , auto_pad
    , kernel_shape
    , number_of_kernel_shape
    , p
    , pads
    , number_of_pads
    , strides
    , number_of_strides
  );
};


void Interpreter::visit(MatMul& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_matmul_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Max& pOp) {
  // Prepare input
  int32_t input_data_0_ntensor = pOp.getNumOfInputs() - 0;
  void *input_data_0[input_data_0_ntensor];
  for (int i = 0; i < input_data_0_ntensor; ++i) input_data_0[i] = m_ATable[pOp.getInput(0 + i)];
  int32_t input_data_0_ndim[input_data_0_ntensor]; // FIXME: = input_data_0_v->sizes().size();
  int32_t *input_data_0_dims[input_data_0_ntensor]; // FIXME: [input_data_0_ndim[0]];
  // FIXME: for (int i = 0; i < input_data_0_ndim; ++i) input_data_0_dims[i] = input_data_0_v->sizes()[i].dim;
  // Prepare output
  Tensor *output_max_t = pOp.getOutput(0);
  void *output_max = m_ATable[output_max_t];
  int32_t output_max_ndim = output_max_t->getNumOfDimensions();
  int32_t output_max_dims[output_max_ndim];
  for (int i = 0; i < output_max_ndim; ++i) output_max_dims[i] = output_max_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_max_float(
    m_pContext
    , reinterpret_cast<float **>(input_data_0)
    , input_data_0_ntensor
    , input_data_0_ndim, input_data_0_dims
    , reinterpret_cast<float *>(output_max)
    , output_max_ndim, output_max_dims
    
  );
};


void Interpreter::visit(MaxPool& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  Tensor *output_Indices_t = NULL;
  void *output_Indices = NULL;
  int32_t output_Indices_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_Indices_t = pOp.getOutput(1);
    output_Indices = m_ATable[output_Indices_t];
    output_Indices_ndim = output_Indices_t->getNumOfDimensions();
  }
  int32_t output_Indices_dims[output_Indices_ndim];
  for (int i = 0; i < output_Indices_ndim; ++i) output_Indices_dims[i] = output_Indices_t->dimension(i);
  // Prepare attributes
  const char * auto_pad = pOp.getAutoPad().value().c_str();
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pads[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i) pads[i] = pOp.getPads().at(i);
  int32_t storage_order = pOp.getStorageOrder().value();
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i) strides[i] = pOp.getStrides().at(i);

  // Call to Runtime
  ONNC_RUNTIME_maxpool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , reinterpret_cast<float *>(output_Indices)
    , output_Indices_ndim, output_Indices_dims
    , auto_pad
    , kernel_shape
    , number_of_kernel_shape
    , pads
    , number_of_pads
    , storage_order
    , strides
    , number_of_strides
  );
};


void Interpreter::visit(MaxRoiPool& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_rois_t = pOp.getInput(1);
  void *input_rois = m_ATable[input_rois_t];
  int32_t input_rois_ndim = input_rois_t->getNumOfDimensions();
  int32_t input_rois_dims[input_rois_ndim];
  for (int i = 0; i < input_rois_ndim; ++i) input_rois_dims[i] = input_rois_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  int32_t number_of_pooled_shape = pOp.getPooledShape().vector().size();
  int32_t pooled_shape[number_of_pooled_shape];
  for (int i = 0; i < number_of_pooled_shape; ++i) pooled_shape[i] = pOp.getPooledShape().at(i);
  float spatial_scale = pOp.getSpatialScale().value();

  // Call to Runtime
  ONNC_RUNTIME_maxroipool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_rois)
    , input_rois_ndim, input_rois_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , pooled_shape
    , number_of_pooled_shape
    , spatial_scale
  );
};


void Interpreter::visit(Mean& pOp) {
  // Prepare input
  int32_t input_data_0_ntensor = pOp.getNumOfInputs() - 0;
  void *input_data_0[input_data_0_ntensor];
  for (int i = 0; i < input_data_0_ntensor; ++i) input_data_0[i] = m_ATable[pOp.getInput(0 + i)];
  int32_t input_data_0_ndim[input_data_0_ntensor]; // FIXME: = input_data_0_v->sizes().size();
  int32_t *input_data_0_dims[input_data_0_ntensor]; // FIXME: [input_data_0_ndim[0]];
  // FIXME: for (int i = 0; i < input_data_0_ndim; ++i) input_data_0_dims[i] = input_data_0_v->sizes()[i].dim;
  // Prepare output
  Tensor *output_mean_t = pOp.getOutput(0);
  void *output_mean = m_ATable[output_mean_t];
  int32_t output_mean_ndim = output_mean_t->getNumOfDimensions();
  int32_t output_mean_dims[output_mean_ndim];
  for (int i = 0; i < output_mean_ndim; ++i) output_mean_dims[i] = output_mean_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_mean_float(
    m_pContext
    , reinterpret_cast<float **>(input_data_0)
    , input_data_0_ntensor
    , input_data_0_ndim, input_data_0_dims
    , reinterpret_cast<float *>(output_mean)
    , output_mean_ndim, output_mean_dims
    
  );
};


void Interpreter::visit(Min& pOp) {
  // Prepare input
  int32_t input_data_0_ntensor = pOp.getNumOfInputs() - 0;
  void *input_data_0[input_data_0_ntensor];
  for (int i = 0; i < input_data_0_ntensor; ++i) input_data_0[i] = m_ATable[pOp.getInput(0 + i)];
  int32_t input_data_0_ndim[input_data_0_ntensor]; // FIXME: = input_data_0_v->sizes().size();
  int32_t *input_data_0_dims[input_data_0_ntensor]; // FIXME: [input_data_0_ndim[0]];
  // FIXME: for (int i = 0; i < input_data_0_ndim; ++i) input_data_0_dims[i] = input_data_0_v->sizes()[i].dim;
  // Prepare output
  Tensor *output_min_t = pOp.getOutput(0);
  void *output_min = m_ATable[output_min_t];
  int32_t output_min_ndim = output_min_t->getNumOfDimensions();
  int32_t output_min_dims[output_min_ndim];
  for (int i = 0; i < output_min_ndim; ++i) output_min_dims[i] = output_min_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_min_float(
    m_pContext
    , reinterpret_cast<float **>(input_data_0)
    , input_data_0_ntensor
    , input_data_0_ndim, input_data_0_dims
    , reinterpret_cast<float *>(output_min)
    , output_min_ndim, output_min_dims
    
  );
};


void Interpreter::visit(Mul& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_mul_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(Multinomial& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t dtype = pOp.getDtype().value();
  int32_t sample_size = pOp.getSampleSize().value();
  float seed = pOp.getSeed().value();

  // Call to Runtime
  ONNC_RUNTIME_multinomial_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , dtype
    , sample_size
    , seed
  );
};


void Interpreter::visit(Neg& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_neg_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Not& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_not_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Or& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_or_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(PRelu& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_slope_t = pOp.getInput(1);
  void *input_slope = m_ATable[input_slope_t];
  int32_t input_slope_ndim = input_slope_t->getNumOfDimensions();
  int32_t input_slope_dims[input_slope_ndim];
  for (int i = 0; i < input_slope_ndim; ++i) input_slope_dims[i] = input_slope_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_prelu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_slope)
    , input_slope_ndim, input_slope_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Pad& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  const char * mode = pOp.getMode().value().c_str();
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pads[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i) pads[i] = pOp.getPads().at(i);
  float value = pOp.getValue().value();

  // Call to Runtime
  ONNC_RUNTIME_pad_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , mode
    , pads
    , number_of_pads
    , value
  );
};


void Interpreter::visit(Pow& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_Y_t = pOp.getInput(1);
  void *input_Y = m_ATable[input_Y_t];
  int32_t input_Y_ndim = input_Y_t->getNumOfDimensions();
  int32_t input_Y_dims[input_Y_ndim];
  for (int i = 0; i < input_Y_ndim; ++i) input_Y_dims[i] = input_Y_t->dimension(i);
  // Prepare output
  Tensor *output_Z_t = pOp.getOutput(0);
  void *output_Z = m_ATable[output_Z_t];
  int32_t output_Z_ndim = output_Z_t->getNumOfDimensions();
  int32_t output_Z_dims[output_Z_ndim];
  for (int i = 0; i < output_Z_ndim; ++i) output_Z_dims[i] = output_Z_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_pow_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_Y)
    , input_Y_ndim, input_Y_dims
    , reinterpret_cast<float *>(output_Z)
    , output_Z_ndim, output_Z_dims
    
  );
};


void Interpreter::visit(RNN& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  Tensor *input_W_t = pOp.getInput(1);
  void *input_W = m_ATable[input_W_t];
  int32_t input_W_ndim = input_W_t->getNumOfDimensions();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_t->dimension(i);
  Tensor *input_R_t = pOp.getInput(2);
  void *input_R = m_ATable[input_R_t];
  int32_t input_R_ndim = input_R_t->getNumOfDimensions();
  int32_t input_R_dims[input_R_ndim];
  for (int i = 0; i < input_R_ndim; ++i) input_R_dims[i] = input_R_t->dimension(i);
  Tensor *input_B_t = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pOp.getNumOfInputs() > 3) {
    input_B_t = pOp.getInput(3);
    input_B = m_ATable[input_B_t];
    input_B_ndim = input_B_t->getNumOfDimensions();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  Tensor *input_sequence_lens_t = NULL;
  void *input_sequence_lens = NULL;
  int32_t input_sequence_lens_ndim = 0;
  if (pOp.getNumOfInputs() > 4) {
    input_sequence_lens_t = pOp.getInput(4);
    input_sequence_lens = m_ATable[input_sequence_lens_t];
    input_sequence_lens_ndim = input_sequence_lens_t->getNumOfDimensions();
  }
  int32_t input_sequence_lens_dims[input_sequence_lens_ndim];
  for (int i = 0; i < input_sequence_lens_ndim; ++i) input_sequence_lens_dims[i] = input_sequence_lens_t->dimension(i);
  Tensor *input_initial_h_t = NULL;
  void *input_initial_h = NULL;
  int32_t input_initial_h_ndim = 0;
  if (pOp.getNumOfInputs() > 5) {
    input_initial_h_t = pOp.getInput(5);
    input_initial_h = m_ATable[input_initial_h_t];
    input_initial_h_ndim = input_initial_h_t->getNumOfDimensions();
  }
  int32_t input_initial_h_dims[input_initial_h_ndim];
  for (int i = 0; i < input_initial_h_ndim; ++i) input_initial_h_dims[i] = input_initial_h_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = NULL;
  void *output_Y = NULL;
  int32_t output_Y_ndim = 0;
  if (pOp.getNumOfOutputs() > 0) {
    output_Y_t = pOp.getOutput(0);
    output_Y = m_ATable[output_Y_t];
    output_Y_ndim = output_Y_t->getNumOfDimensions();
  }
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  Tensor *output_Y_h_t = NULL;
  void *output_Y_h = NULL;
  int32_t output_Y_h_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_Y_h_t = pOp.getOutput(1);
    output_Y_h = m_ATable[output_Y_h_t];
    output_Y_h_ndim = output_Y_h_t->getNumOfDimensions();
  }
  int32_t output_Y_h_dims[output_Y_h_ndim];
  for (int i = 0; i < output_Y_h_ndim; ++i) output_Y_h_dims[i] = output_Y_h_t->dimension(i);
  // Prepare attributes
  int32_t number_of_activation_alpha = pOp.getActivationAlpha().vector().size();
  float activation_alpha[number_of_activation_alpha];
  for (int i = 0; i < number_of_activation_alpha; ++i) activation_alpha[i] = pOp.getActivationAlpha().at(i);
  int32_t number_of_activation_beta = pOp.getActivationBeta().vector().size();
  float activation_beta[number_of_activation_beta];
  for (int i = 0; i < number_of_activation_beta; ++i) activation_beta[i] = pOp.getActivationBeta().at(i);
  int32_t number_of_activations = pOp.getActivations().vector().size();
  const char * activations[number_of_activations];
  for (int i = 0; i < number_of_activations; ++i) activations[i] = pOp.getActivations().at(i).c_str();
  float clip = pOp.getClip().value();
  const char * direction = pOp.getDirection().value().c_str();
  int32_t hidden_size = pOp.getHiddenSize().value();

  // Call to Runtime
  ONNC_RUNTIME_rnn_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(input_W)
    , input_W_ndim, input_W_dims
    , reinterpret_cast<float *>(input_R)
    , input_R_ndim, input_R_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(input_sequence_lens)
    , input_sequence_lens_ndim, input_sequence_lens_dims
    , reinterpret_cast<float *>(input_initial_h)
    , input_initial_h_ndim, input_initial_h_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , reinterpret_cast<float *>(output_Y_h)
    , output_Y_h_ndim, output_Y_h_dims
    , activation_alpha
    , number_of_activation_alpha
    , activation_beta
    , number_of_activation_beta
    , activations
    , number_of_activations
    , clip
    , direction
    , hidden_size
  );
};


void Interpreter::visit(RandomNormal& pOp) {
  // Prepare input
  
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t dtype = pOp.getDtype().value();
  float mean = pOp.getMean().value();
  float scale = pOp.getScale().value();
  float seed = pOp.getSeed().value();
  int32_t number_of_shape = pOp.getShape().vector().size();
  int32_t shape[number_of_shape];
  for (int i = 0; i < number_of_shape; ++i) shape[i] = pOp.getShape().at(i);

  // Call to Runtime
  ONNC_RUNTIME_randomnormal_float(
    m_pContext
    
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , dtype
    , mean
    , scale
    , seed
    , shape
    , number_of_shape
  );
};


void Interpreter::visit(RandomNormalLike& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t dtype = pOp.getDtype().value();
  float mean = pOp.getMean().value();
  float scale = pOp.getScale().value();
  float seed = pOp.getSeed().value();

  // Call to Runtime
  ONNC_RUNTIME_randomnormallike_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , dtype
    , mean
    , scale
    , seed
  );
};


void Interpreter::visit(RandomUniform& pOp) {
  // Prepare input
  
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t dtype = pOp.getDtype().value();
  float high = pOp.getHigh().value();
  float low = pOp.getLow().value();
  float seed = pOp.getSeed().value();
  int32_t number_of_shape = pOp.getShape().vector().size();
  int32_t shape[number_of_shape];
  for (int i = 0; i < number_of_shape; ++i) shape[i] = pOp.getShape().at(i);

  // Call to Runtime
  ONNC_RUNTIME_randomuniform_float(
    m_pContext
    
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , dtype
    , high
    , low
    , seed
    , shape
    , number_of_shape
  );
};


void Interpreter::visit(RandomUniformLike& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t dtype = pOp.getDtype().value();
  float high = pOp.getHigh().value();
  float low = pOp.getLow().value();
  float seed = pOp.getSeed().value();

  // Call to Runtime
  ONNC_RUNTIME_randomuniformlike_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , dtype
    , high
    , low
    , seed
  );
};


void Interpreter::visit(Reciprocal& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_reciprocal_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(ReduceL1& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducel1_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceL2& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducel2_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceLogSum& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducelogsum_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceLogSumExp& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducelogsumexp_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceMax& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducemax_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceMean& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducemean_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceMin& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducemin_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceProd& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reduceprod_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceSum& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducesum_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(ReduceSumSquare& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_reduced_t = pOp.getOutput(0);
  void *output_reduced = m_ATable[output_reduced_t];
  int32_t output_reduced_ndim = output_reduced_t->getNumOfDimensions();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t keepdims = pOp.getKeepdims().value();

  // Call to Runtime
  ONNC_RUNTIME_reducesumsquare_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_reduced)
    , output_reduced_ndim, output_reduced_dims
    , axes
    , number_of_axes
    , keepdims
  );
};


void Interpreter::visit(Relu& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_relu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Reshape& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  Tensor *input_shape_t = pOp.getInput(1);
  void *input_shape = m_ATable[input_shape_t];
  int32_t input_shape_ndim = input_shape_t->getNumOfDimensions();
  int32_t input_shape_dims[input_shape_ndim];
  for (int i = 0; i < input_shape_ndim; ++i) input_shape_dims[i] = input_shape_t->dimension(i);
  // Prepare output
  Tensor *output_reshaped_t = pOp.getOutput(0);
  void *output_reshaped = m_ATable[output_reshaped_t];
  int32_t output_reshaped_ndim = output_reshaped_t->getNumOfDimensions();
  int32_t output_reshaped_dims[output_reshaped_ndim];
  for (int i = 0; i < output_reshaped_ndim; ++i) output_reshaped_dims[i] = output_reshaped_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_reshape_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(input_shape)
    , input_shape_ndim, input_shape_dims
    , reinterpret_cast<float *>(output_reshaped)
    , output_reshaped_ndim, output_reshaped_dims
    
  );
};


void Interpreter::visit(Selu& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();
  float gamma = pOp.getGamma().value();

  // Call to Runtime
  ONNC_RUNTIME_selu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
    , gamma
  );
};


void Interpreter::visit(Shape& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_shape_t = pOp.getOutput(0);
  void *output_shape = m_ATable[output_shape_t];
  int32_t output_shape_ndim = output_shape_t->getNumOfDimensions();
  int32_t output_shape_dims[output_shape_ndim];
  for (int i = 0; i < output_shape_ndim; ++i) output_shape_dims[i] = output_shape_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_shape_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_shape)
    , output_shape_ndim, output_shape_dims
    
  );
};


void Interpreter::visit(Sigmoid& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_sigmoid_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Sin& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_sin_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(Size& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_size_t = pOp.getOutput(0);
  void *output_size = m_ATable[output_size_t];
  int32_t output_size_ndim = output_size_t->getNumOfDimensions();
  int32_t output_size_dims[output_size_ndim];
  for (int i = 0; i < output_size_ndim; ++i) output_size_dims[i] = output_size_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_size_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_size)
    , output_size_ndim, output_size_dims
    
  );
};


void Interpreter::visit(Slice& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);
  int32_t number_of_ends = pOp.getEnds().vector().size();
  int32_t ends[number_of_ends];
  for (int i = 0; i < number_of_ends; ++i) ends[i] = pOp.getEnds().at(i);
  int32_t number_of_starts = pOp.getStarts().vector().size();
  int32_t starts[number_of_starts];
  for (int i = 0; i < number_of_starts; ++i) starts[i] = pOp.getStarts().at(i);

  // Call to Runtime
  ONNC_RUNTIME_slice_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axes
    , number_of_axes
    , ends
    , number_of_ends
    , starts
    , number_of_starts
  );
};


void Interpreter::visit(Softmax& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();

  // Call to Runtime
  ONNC_RUNTIME_softmax_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visit(Softplus& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_softplus_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Softsign& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_softsign_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(SpaceToDepth& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t blocksize = pOp.getBlocksize().value();

  // Call to Runtime
  ONNC_RUNTIME_spacetodepth_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , blocksize
  );
};


void Interpreter::visit(Split& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  int32_t output_outputs_ntensor = pOp.getNumOfOutputs() - 0;
  void *output_outputs[output_outputs_ntensor];
  for (int i = 0; i < output_outputs_ntensor; ++i) output_outputs[i] = m_ATable[pOp.getOutput(0 + i)];
  int32_t output_outputs_ndim[output_outputs_ntensor]; // FIXME: = output_outputs_v->sizes().size();
  int32_t *output_outputs_dims[output_outputs_ntensor]; // FIXME: [output_outputs_ndim[0]];
  // FIXME: for (int i = 0; i < output_outputs_ndim; ++i) output_outputs_dims[i] = output_outputs_v->sizes()[i].dim;
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();
  int32_t number_of_split = pOp.getSplit().vector().size();
  int32_t split[number_of_split];
  for (int i = 0; i < number_of_split; ++i) split[i] = pOp.getSplit().at(i);

  // Call to Runtime
  ONNC_RUNTIME_split_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float **>(output_outputs)
    , output_outputs_ntensor
    , output_outputs_ndim, output_outputs_dims
    , axis
    , split
    , number_of_split
  );
};


void Interpreter::visit(Sqrt& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_sqrt_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visit(Squeeze& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_squeezed_t = pOp.getOutput(0);
  void *output_squeezed = m_ATable[output_squeezed_t];
  int32_t output_squeezed_ndim = output_squeezed_t->getNumOfDimensions();
  int32_t output_squeezed_dims[output_squeezed_ndim];
  for (int i = 0; i < output_squeezed_ndim; ++i) output_squeezed_dims[i] = output_squeezed_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);

  // Call to Runtime
  ONNC_RUNTIME_squeeze_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_squeezed)
    , output_squeezed_ndim, output_squeezed_dims
    , axes
    , number_of_axes
  );
};


void Interpreter::visit(Sub& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_sub_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(Sum& pOp) {
  // Prepare input
  int32_t input_data_0_ntensor = pOp.getNumOfInputs() - 0;
  void *input_data_0[input_data_0_ntensor];
  for (int i = 0; i < input_data_0_ntensor; ++i) input_data_0[i] = m_ATable[pOp.getInput(0 + i)];
  int32_t input_data_0_ndim[input_data_0_ntensor]; // FIXME: = input_data_0_v->sizes().size();
  int32_t *input_data_0_dims[input_data_0_ntensor]; // FIXME: [input_data_0_ndim[0]];
  // FIXME: for (int i = 0; i < input_data_0_ndim; ++i) input_data_0_dims[i] = input_data_0_v->sizes()[i].dim;
  // Prepare output
  Tensor *output_sum_t = pOp.getOutput(0);
  void *output_sum = m_ATable[output_sum_t];
  int32_t output_sum_ndim = output_sum_t->getNumOfDimensions();
  int32_t output_sum_dims[output_sum_ndim];
  for (int i = 0; i < output_sum_ndim; ++i) output_sum_dims[i] = output_sum_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_sum_float(
    m_pContext
    , reinterpret_cast<float **>(input_data_0)
    , input_data_0_ntensor
    , input_data_0_ndim, input_data_0_dims
    , reinterpret_cast<float *>(output_sum)
    , output_sum_ndim, output_sum_dims
    
  );
};


void Interpreter::visit(Tan& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_tan_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(Tanh& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_tanh_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(Tile& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  Tensor *input_repeats_t = pOp.getInput(1);
  void *input_repeats = m_ATable[input_repeats_t];
  int32_t input_repeats_ndim = input_repeats_t->getNumOfDimensions();
  int32_t input_repeats_dims[input_repeats_ndim];
  for (int i = 0; i < input_repeats_ndim; ++i) input_repeats_dims[i] = input_repeats_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_tile_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(input_repeats)
    , input_repeats_ndim, input_repeats_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(TopK& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Values_t = pOp.getOutput(0);
  void *output_Values = m_ATable[output_Values_t];
  int32_t output_Values_ndim = output_Values_t->getNumOfDimensions();
  int32_t output_Values_dims[output_Values_ndim];
  for (int i = 0; i < output_Values_ndim; ++i) output_Values_dims[i] = output_Values_t->dimension(i);
  Tensor *output_Indices_t = pOp.getOutput(1);
  void *output_Indices = m_ATable[output_Indices_t];
  int32_t output_Indices_ndim = output_Indices_t->getNumOfDimensions();
  int32_t output_Indices_dims[output_Indices_ndim];
  for (int i = 0; i < output_Indices_ndim; ++i) output_Indices_dims[i] = output_Indices_t->dimension(i);
  // Prepare attributes
  int32_t axis = pOp.getAxis().value();
  int32_t k = pOp.getK().value();

  // Call to Runtime
  ONNC_RUNTIME_topk_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Values)
    , output_Values_ndim, output_Values_dims
    , reinterpret_cast<float *>(output_Indices)
    , output_Indices_ndim, output_Indices_dims
    , axis
    , k
  );
};


void Interpreter::visit(Transpose& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_transposed_t = pOp.getOutput(0);
  void *output_transposed = m_ATable[output_transposed_t];
  int32_t output_transposed_ndim = output_transposed_t->getNumOfDimensions();
  int32_t output_transposed_dims[output_transposed_ndim];
  for (int i = 0; i < output_transposed_ndim; ++i) output_transposed_dims[i] = output_transposed_t->dimension(i);
  // Prepare attributes
  int32_t number_of_perm = pOp.getPerm().vector().size();
  int32_t perm[number_of_perm];
  for (int i = 0; i < number_of_perm; ++i) perm[i] = pOp.getPerm().at(i);

  // Call to Runtime
  ONNC_RUNTIME_transpose_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_transposed)
    , output_transposed_ndim, output_transposed_dims
    , perm
    , number_of_perm
  );
};


void Interpreter::visit(Unsqueeze& pOp) {
  // Prepare input
  Tensor *input_data_t = pOp.getInput(0);
  void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_t->dimension(i);
  // Prepare output
  Tensor *output_expanded_t = pOp.getOutput(0);
  void *output_expanded = m_ATable[output_expanded_t];
  int32_t output_expanded_ndim = output_expanded_t->getNumOfDimensions();
  int32_t output_expanded_dims[output_expanded_ndim];
  for (int i = 0; i < output_expanded_ndim; ++i) output_expanded_dims[i] = output_expanded_t->dimension(i);
  // Prepare attributes
  int32_t number_of_axes = pOp.getAxes().vector().size();
  int32_t axes[number_of_axes];
  for (int i = 0; i < number_of_axes; ++i) axes[i] = pOp.getAxes().at(i);

  // Call to Runtime
  ONNC_RUNTIME_unsqueeze_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_expanded)
    , output_expanded_ndim, output_expanded_dims
    , axes
    , number_of_axes
  );
};


void Interpreter::visit(Upsample& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  const char * mode = pOp.getMode().value().c_str();
  int32_t number_of_scales = pOp.getScales().vector().size();
  float scales[number_of_scales];
  for (int i = 0; i < number_of_scales; ++i) scales[i] = pOp.getScales().at(i);

  // Call to Runtime
  ONNC_RUNTIME_upsample_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , mode
    , scales
    , number_of_scales
  );
};


void Interpreter::visit(Xor& pOp) {
  // Prepare input
  Tensor *input_A_t = pOp.getInput(0);
  void *input_A = m_ATable[input_A_t];
  int32_t input_A_ndim = input_A_t->getNumOfDimensions();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_t->dimension(i);
  Tensor *input_B_t = pOp.getInput(1);
  void *input_B = m_ATable[input_B_t];
  int32_t input_B_ndim = input_B_t->getNumOfDimensions();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output
  Tensor *output_C_t = pOp.getOutput(0);
  void *output_C = m_ATable[output_C_t];
  int32_t output_C_ndim = output_C_t->getNumOfDimensions();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_t->dimension(i);
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_xor_float(
    m_pContext
    , reinterpret_cast<float *>(input_A)
    , input_A_ndim, input_A_dims
    , reinterpret_cast<float *>(input_B)
    , input_B_ndim, input_B_dims
    , reinterpret_cast<float *>(output_C)
    , output_C_ndim, output_C_dims
    
  );
};


void Interpreter::visit(ATen& pOp) {
  // Prepare input
  int32_t input_input_ntensor = pOp.getNumOfInputs() - 0;
  void *input_input[input_input_ntensor];
  for (int i = 0; i < input_input_ntensor; ++i) input_input[i] = m_ATable[pOp.getInput(0 + i)];
  int32_t input_input_ndim[input_input_ntensor]; // FIXME: = input_input_v->sizes().size();
  int32_t *input_input_dims[input_input_ntensor]; // FIXME: [input_input_ndim[0]];
  // FIXME: for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  int32_t output_output_ntensor = pOp.getNumOfOutputs() - 0;
  void *output_output[output_output_ntensor];
  for (int i = 0; i < output_output_ntensor; ++i) output_output[i] = m_ATable[pOp.getOutput(0 + i)];
  int32_t output_output_ndim[output_output_ntensor]; // FIXME: = output_output_v->sizes().size();
  int32_t *output_output_dims[output_output_ntensor]; // FIXME: [output_output_ndim[0]];
  // FIXME: for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  ONNC_RUNTIME_aten_float(
    m_pContext
    , reinterpret_cast<float **>(input_input)
    , input_input_ntensor
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float **>(output_output)
    , output_output_ntensor
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visit(Affine& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();
  float beta = pOp.getBeta().value();

  // Call to Runtime
  ONNC_RUNTIME_affine_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
    , beta
  );
};


void Interpreter::visit(ConstantFill& pOp) {
  // Prepare input
  Tensor *input_input_t = NULL;
  void *input_input = NULL;
  int32_t input_input_ndim = 0;
  if (pOp.getNumOfInputs() > 0) {
    input_input_t = pOp.getInput(0);
    input_input = m_ATable[input_input_t];
    input_input_ndim = input_input_t->getNumOfDimensions();
  }
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t dtype = pOp.getDtype().value();
  int32_t number_of_extra_shape = pOp.getExtraShape().vector().size();
  int32_t extra_shape[number_of_extra_shape];
  for (int i = 0; i < number_of_extra_shape; ++i) extra_shape[i] = pOp.getExtraShape().at(i);
  int32_t input_as_shape = pOp.getInputAsShape().value();
  int32_t number_of_shape = pOp.getShape().vector().size();
  int32_t shape[number_of_shape];
  for (int i = 0; i < number_of_shape; ++i) shape[i] = pOp.getShape().at(i);
  float value = pOp.getValue().value();

  // Call to Runtime
  ONNC_RUNTIME_constantfill_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , dtype
    , extra_shape
    , number_of_extra_shape
    , input_as_shape
    , shape
    , number_of_shape
    , value
  );
};


void Interpreter::visit(Crop& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t number_of_border = pOp.getBorder().vector().size();
  int32_t border[number_of_border];
  for (int i = 0; i < number_of_border; ++i) border[i] = pOp.getBorder().at(i);
  int32_t number_of_scale = pOp.getScale().vector().size();
  int32_t scale[number_of_scale];
  for (int i = 0; i < number_of_scale; ++i) scale[i] = pOp.getScale().at(i);

  // Call to Runtime
  ONNC_RUNTIME_crop_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , border
    , number_of_border
    , scale
    , number_of_scale
  );
};


void Interpreter::visit(GRUUnit& pOp) {
  // Prepare input
  Tensor *input_hidden_prev_t = pOp.getInput(0);
  void *input_hidden_prev = m_ATable[input_hidden_prev_t];
  int32_t input_hidden_prev_ndim = input_hidden_prev_t->getNumOfDimensions();
  int32_t input_hidden_prev_dims[input_hidden_prev_ndim];
  for (int i = 0; i < input_hidden_prev_ndim; ++i) input_hidden_prev_dims[i] = input_hidden_prev_t->dimension(i);
  Tensor *input_gates_t = pOp.getInput(1);
  void *input_gates = m_ATable[input_gates_t];
  int32_t input_gates_ndim = input_gates_t->getNumOfDimensions();
  int32_t input_gates_dims[input_gates_ndim];
  for (int i = 0; i < input_gates_ndim; ++i) input_gates_dims[i] = input_gates_t->dimension(i);
  Tensor *input_seq_lengths_t = pOp.getInput(2);
  void *input_seq_lengths = m_ATable[input_seq_lengths_t];
  int32_t input_seq_lengths_ndim = input_seq_lengths_t->getNumOfDimensions();
  int32_t input_seq_lengths_dims[input_seq_lengths_ndim];
  for (int i = 0; i < input_seq_lengths_ndim; ++i) input_seq_lengths_dims[i] = input_seq_lengths_t->dimension(i);
  Tensor *input_t_t = pOp.getInput(3);
  void *input_t = m_ATable[input_t_t];
  int32_t input_t_ndim = input_t_t->getNumOfDimensions();
  int32_t input_t_dims[input_t_ndim];
  for (int i = 0; i < input_t_ndim; ++i) input_t_dims[i] = input_t_t->dimension(i);
  // Prepare output
  Tensor *output_hidden_t = pOp.getOutput(0);
  void *output_hidden = m_ATable[output_hidden_t];
  int32_t output_hidden_ndim = output_hidden_t->getNumOfDimensions();
  int32_t output_hidden_dims[output_hidden_ndim];
  for (int i = 0; i < output_hidden_ndim; ++i) output_hidden_dims[i] = output_hidden_t->dimension(i);
  // Prepare attributes
  int32_t drop_states = pOp.getDropStates().value();

  // Call to Runtime
  ONNC_RUNTIME_gruunit_float(
    m_pContext
    , reinterpret_cast<float *>(input_hidden_prev)
    , input_hidden_prev_ndim, input_hidden_prev_dims
    , reinterpret_cast<float *>(input_gates)
    , input_gates_ndim, input_gates_dims
    , reinterpret_cast<float *>(input_seq_lengths)
    , input_seq_lengths_ndim, input_seq_lengths_dims
    , reinterpret_cast<float *>(input_t)
    , input_t_ndim, input_t_dims
    , reinterpret_cast<float *>(output_hidden)
    , output_hidden_ndim, output_hidden_dims
    , drop_states
  );
};


void Interpreter::visit(GivenTensorFill& pOp) {
  // Prepare input
  Tensor *input_shape_t = NULL;
  void *input_shape = NULL;
  int32_t input_shape_ndim = 0;
  if (pOp.getNumOfInputs() > 0) {
    input_shape_t = pOp.getInput(0);
    input_shape = m_ATable[input_shape_t];
    input_shape_ndim = input_shape_t->getNumOfDimensions();
  }
  int32_t input_shape_dims[input_shape_ndim];
  for (int i = 0; i < input_shape_ndim; ++i) input_shape_dims[i] = input_shape_t->dimension(i);
  // Prepare output
  Tensor *output_X_t = pOp.getOutput(0);
  void *output_X = m_ATable[output_X_t];
  int32_t output_X_ndim = output_X_t->getNumOfDimensions();
  int32_t output_X_dims[output_X_ndim];
  for (int i = 0; i < output_X_ndim; ++i) output_X_dims[i] = output_X_t->dimension(i);
  // Prepare attributes
  int32_t number_of_extra_shape = pOp.getExtraShape().vector().size();
  int32_t extra_shape[number_of_extra_shape];
  for (int i = 0; i < number_of_extra_shape; ++i) extra_shape[i] = pOp.getExtraShape().at(i);
  int32_t input_as_shape = pOp.getInputAsShape().value();
  int32_t number_of_shape = pOp.getShape().vector().size();
  int32_t shape[number_of_shape];
  for (int i = 0; i < number_of_shape; ++i) shape[i] = pOp.getShape().at(i);
  int32_t number_of_values = pOp.getValues().vector().size();
  float values[number_of_values];
  for (int i = 0; i < number_of_values; ++i) values[i] = pOp.getValues().at(i);

  // Call to Runtime
  ONNC_RUNTIME_giventensorfill_float(
    m_pContext
    , reinterpret_cast<float *>(input_shape)
    , input_shape_ndim, input_shape_dims
    , reinterpret_cast<float *>(output_X)
    , output_X_ndim, output_X_dims
    , extra_shape
    , number_of_extra_shape
    , input_as_shape
    , shape
    , number_of_shape
    , values
    , number_of_values
  );
};


void Interpreter::visit(ImageScaler& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t number_of_bias = pOp.getBias().vector().size();
  float bias[number_of_bias];
  for (int i = 0; i < number_of_bias; ++i) bias[i] = pOp.getBias().at(i);
  float scale = pOp.getScale().value();

  // Call to Runtime
  ONNC_RUNTIME_imagescaler_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , bias
    , number_of_bias
    , scale
  );
};


void Interpreter::visit(MeanVarianceNormalization& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  int32_t across_channels = pOp.getAcrossChannels().value();
  int32_t normalize_variance = pOp.getNormalizeVariance().value();

  // Call to Runtime
  ONNC_RUNTIME_meanvariancenormalization_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , across_channels
    , normalize_variance
  );
};


void Interpreter::visit(ParametricSoftplus& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();
  float beta = pOp.getBeta().value();

  // Call to Runtime
  ONNC_RUNTIME_parametricsoftplus_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
    , beta
  );
};


void Interpreter::visit(Scale& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  float scale = pOp.getScale().value();

  // Call to Runtime
  ONNC_RUNTIME_scale_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , scale
  );
};


void Interpreter::visit(ScaledTanh& pOp) {
  // Prepare input
  Tensor *input_input_t = pOp.getInput(0);
  void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_t->dimension(i);
  // Prepare output
  Tensor *output_output_t = pOp.getOutput(0);
  void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();
  float beta = pOp.getBeta().value();

  // Call to Runtime
  ONNC_RUNTIME_scaledtanh_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , alpha
    , beta
  );
};


void Interpreter::visit(ThresholdedRelu& pOp) {
  // Prepare input
  Tensor *input_X_t = pOp.getInput(0);
  void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_t->dimension(i);
  // Prepare output
  Tensor *output_Y_t = pOp.getOutput(0);
  void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_t->dimension(i);
  // Prepare attributes
  float alpha = pOp.getAlpha().value();

  // Call to Runtime
  ONNC_RUNTIME_thresholdedrelu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
  );
};

