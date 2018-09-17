//===- Interpreter.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Interpreter.h"
#include <onnc/Support/IOStream.h>

#define restrict __restrict__
extern "C" {
#include <onnc/Runtime/onnc-runtime.h>
}
#undef restrict

using namespace onnc;


static inline
size_t sizeof_tensor_type(const ::onnx::TensorProto_DataType& dataType){
  switch(dataType){
    case ::onnx::TensorProto_DataType_BOOL:
      return sizeof(bool);
    case ::onnx::TensorProto_DataType_INT8:
    case ::onnx::TensorProto_DataType_UINT8:
      return 1;
    case ::onnx::TensorProto_DataType_UINT16:
    case ::onnx::TensorProto_DataType_INT16:
    case ::onnx::TensorProto_DataType_FLOAT16:
      return 2;
    case ::onnx::TensorProto_DataType_UINT32:
    case ::onnx::TensorProto_DataType_INT32:
      return 4;
    case ::onnx::TensorProto_DataType_FLOAT:
      return sizeof(float);
    case ::onnx::TensorProto_DataType_DOUBLE:
      return sizeof(double);
    case ::onnx::TensorProto_DataType_INT64:
    case ::onnx::TensorProto_DataType_UINT64:
    case ::onnx::TensorProto_DataType_COMPLEX64:
      return 8;
    case ::onnx::TensorProto_DataType_COMPLEX128:
      return 16;
    case ::onnx::TensorProto_DataType_STRING:
      return sizeof(char *);
    default:
      return 0;
  }
}

// XXX: Use runtime to allocate memory
static inline void *allocate_output_memory(::onnx::Value *v) {
  size_t size = 1;
  for (auto i: v->sizes()) {
    size *= i.dim;
  }
  return new char[sizeof_tensor_type(v->elemType()) * size];
}

static inline void print_value(::onnx::Value *v, const void *ptr) {
  size_t size = 1;
  for (auto i: v->sizes()) {
    size *= i.dim;
  }
  if (size > 100) size = 100;
  errs() << '[';
  for (size_t i = 0; i < size; ++i) {
    errs() << reinterpret_cast<const float *>(ptr)[i] << ", ";
  }
  errs() << ']' << std::endl;
}

//===----------------------------------------------------------------------===//
// Interpreter
//===----------------------------------------------------------------------===//
void Interpreter::visitNode(::onnx::Node *pNode) {
  errs() << "Not implemented node: " << pNode->kind().toString() << std::endl;
}

void Interpreter::visitAbs(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "abs" << std::endl;
  ONNC_RUNTIME_abs_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitAcos(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "acos" << std::endl;
  ONNC_RUNTIME_acos_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitAdd(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "add" << std::endl;
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


void Interpreter::visitAnd(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "and" << std::endl;
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


void Interpreter::visitArgMax(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "argmax" << std::endl;
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


void Interpreter::visitArgMin(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "argmin" << std::endl;
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


void Interpreter::visitAsin(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "asin" << std::endl;
  ONNC_RUNTIME_asin_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitAtan(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "atan" << std::endl;
  ONNC_RUNTIME_atan_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitAveragePool(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol auto_pad_s = ::onnx::Symbol("auto_pad");
  const char * auto_pad;
  if (pNode->hasAttribute(auto_pad_s)) auto_pad = pNode->s(auto_pad_s).c_str();
  ::onnx::Symbol count_include_pad_s = ::onnx::Symbol("count_include_pad");
  int32_t count_include_pad;
  if (pNode->hasAttribute(count_include_pad_s)) count_include_pad = pNode->i(count_include_pad_s);
  ::onnx::Symbol kernel_shape_s = ::onnx::Symbol("kernel_shape");
  int32_t number_of_kernel_shape = pNode->hasAttribute(kernel_shape_s) ? pNode->is(kernel_shape_s).size() : 0;
  int32_t kernel_shape[number_of_kernel_shape];
  if (pNode->hasAttribute(kernel_shape_s)) for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pNode->is(kernel_shape_s)[i];
  ::onnx::Symbol pads_s = ::onnx::Symbol("pads");
  int32_t number_of_pads = pNode->hasAttribute(pads_s) ? pNode->is(pads_s).size() : 0;
  int32_t pads[number_of_pads];
  if (pNode->hasAttribute(pads_s)) for (int i = 0; i < number_of_pads; ++i) pads[i] = pNode->is(pads_s)[i];
  ::onnx::Symbol strides_s = ::onnx::Symbol("strides");
  int32_t number_of_strides = pNode->hasAttribute(strides_s) ? pNode->is(strides_s).size() : 0;
  int32_t strides[number_of_strides];
  if (pNode->hasAttribute(strides_s)) for (int i = 0; i < number_of_strides; ++i) strides[i] = pNode->is(strides_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "averagepool" << std::endl;
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


void Interpreter::visitBatchNormalization(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_scale_v = pNode->inputs()[1];
  void *input_scale = m_ATable[input_scale_v];
  int32_t input_scale_ndim = input_scale_v->sizes().size();
  int32_t input_scale_dims[input_scale_ndim];
  for (int i = 0; i < input_scale_ndim; ++i) input_scale_dims[i] = input_scale_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[2];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  ::onnx::Value *input_mean_v = pNode->inputs()[3];
  void *input_mean = m_ATable[input_mean_v];
  int32_t input_mean_ndim = input_mean_v->sizes().size();
  int32_t input_mean_dims[input_mean_ndim];
  for (int i = 0; i < input_mean_ndim; ++i) input_mean_dims[i] = input_mean_v->sizes()[i].dim;
  ::onnx::Value *input_var_v = pNode->inputs()[4];
  void *input_var = m_ATable[input_var_v];
  int32_t input_var_ndim = input_var_v->sizes().size();
  int32_t input_var_dims[input_var_ndim];
  for (int i = 0; i < input_var_ndim; ++i) input_var_dims[i] = input_var_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  ::onnx::Value *output_mean_v = NULL;
  void *output_mean = NULL;
  int32_t output_mean_ndim = 0;
  if (pNode->outputs().size() > 1) {
    output_mean_v = pNode->outputs()[1];
    output_mean = m_ATable[output_mean_v];
    output_mean_ndim = output_mean_v->sizes().size();
  }
  int32_t output_mean_dims[output_mean_ndim];
  for (int i = 0; i < output_mean_ndim; ++i) output_mean_dims[i] = output_mean_v->sizes()[i].dim;
  ::onnx::Value *output_var_v = NULL;
  void *output_var = NULL;
  int32_t output_var_ndim = 0;
  if (pNode->outputs().size() > 2) {
    output_var_v = pNode->outputs()[2];
    output_var = m_ATable[output_var_v];
    output_var_ndim = output_var_v->sizes().size();
  }
  int32_t output_var_dims[output_var_ndim];
  for (int i = 0; i < output_var_ndim; ++i) output_var_dims[i] = output_var_v->sizes()[i].dim;
  ::onnx::Value *output_saved_mean_v = NULL;
  void *output_saved_mean = NULL;
  int32_t output_saved_mean_ndim = 0;
  if (pNode->outputs().size() > 3) {
    output_saved_mean_v = pNode->outputs()[3];
    output_saved_mean = m_ATable[output_saved_mean_v];
    output_saved_mean_ndim = output_saved_mean_v->sizes().size();
  }
  int32_t output_saved_mean_dims[output_saved_mean_ndim];
  for (int i = 0; i < output_saved_mean_ndim; ++i) output_saved_mean_dims[i] = output_saved_mean_v->sizes()[i].dim;
  ::onnx::Value *output_saved_var_v = NULL;
  void *output_saved_var = NULL;
  int32_t output_saved_var_ndim = 0;
  if (pNode->outputs().size() > 4) {
    output_saved_var_v = pNode->outputs()[4];
    output_saved_var = m_ATable[output_saved_var_v];
    output_saved_var_ndim = output_saved_var_v->sizes().size();
  }
  int32_t output_saved_var_dims[output_saved_var_ndim];
  for (int i = 0; i < output_saved_var_ndim; ++i) output_saved_var_dims[i] = output_saved_var_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol epsilon_s = ::onnx::Symbol("epsilon");
  float epsilon;
  if (pNode->hasAttribute(epsilon_s)) epsilon = pNode->f(epsilon_s);
  ::onnx::Symbol momentum_s = ::onnx::Symbol("momentum");
  float momentum;
  if (pNode->hasAttribute(momentum_s)) momentum = pNode->f(momentum_s);
  ::onnx::Symbol spatial_s = ::onnx::Symbol("spatial");
  int32_t spatial;
  if (pNode->hasAttribute(spatial_s)) spatial = pNode->i(spatial_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "batchnormalization" << std::endl;
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


void Interpreter::visitCast(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol to_s = ::onnx::Symbol("to");
  int32_t to;
  if (pNode->hasAttribute(to_s)) to = pNode->i(to_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "cast" << std::endl;
  ONNC_RUNTIME_cast_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , to
  );
};


void Interpreter::visitCeil(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "ceil" << std::endl;
  ONNC_RUNTIME_ceil_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitClip(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol max_s = ::onnx::Symbol("max");
  float max;
  if (pNode->hasAttribute(max_s)) max = pNode->f(max_s);
  ::onnx::Symbol min_s = ::onnx::Symbol("min");
  float min;
  if (pNode->hasAttribute(min_s)) min = pNode->f(min_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "clip" << std::endl;
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


void Interpreter::visitConcat(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_inputs_v = pNode->inputs()[0];
  int32_t input_inputs_ntensor = pNode->inputs().size() - 0;
  void *input_inputs[input_inputs_ntensor]; // FIXME: = m_ATable[input_inputs_v];
  int32_t input_inputs_ndim[input_inputs_ntensor]; // FIXME: = input_inputs_v->sizes().size();
  int32_t *input_inputs_dims[input_inputs_ntensor]; // FIXME: [input_inputs_ndim[0]];
  // FIXME: for (int i = 0; i < input_inputs_ndim; ++i) input_inputs_dims[i] = input_inputs_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_concat_result_v = pNode->outputs()[0];
  m_ATable[output_concat_result_v] = allocate_output_memory(output_concat_result_v);
  void *output_concat_result = m_ATable[output_concat_result_v];
  int32_t output_concat_result_ndim = output_concat_result_v->sizes().size();
  int32_t output_concat_result_dims[output_concat_result_ndim];
  for (int i = 0; i < output_concat_result_ndim; ++i) output_concat_result_dims[i] = output_concat_result_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "concat" << std::endl;
  ONNC_RUNTIME_concat_float(
    m_pContext
    , reinterpret_cast<float **>(input_inputs)
    , input_inputs_ntensor
    , input_inputs_ndim, input_inputs_dims
    , reinterpret_cast<float *>(output_concat_result)
    , output_concat_result_ndim, output_concat_result_dims
    , axis
  );
};


void Interpreter::visitConstant(::onnx::Node *pNode) {
  // Prepare input
  
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol value_s = ::onnx::Symbol("value");
  const void * value;
  if (pNode->hasAttribute(value_s)) value = &pNode->t(value_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "constant" << std::endl;
  ONNC_RUNTIME_constant_float(
    m_pContext
    
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , value
  );
};


void Interpreter::visitConv(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_W_v = pNode->inputs()[1];
  void *input_W = m_ATable[input_W_v];
  int32_t input_W_ndim = input_W_v->sizes().size();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pNode->inputs().size() > 2) {
    input_B_v = pNode->inputs()[2];
    input_B = m_ATable[input_B_v];
    input_B_ndim = input_B_v->sizes().size();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol auto_pad_s = ::onnx::Symbol("auto_pad");
  const char * auto_pad;
  if (pNode->hasAttribute(auto_pad_s)) auto_pad = pNode->s(auto_pad_s).c_str();
  ::onnx::Symbol dilations_s = ::onnx::Symbol("dilations");
  int32_t number_of_dilations = pNode->hasAttribute(dilations_s) ? pNode->is(dilations_s).size() : 0;
  int32_t dilations[number_of_dilations];
  if (pNode->hasAttribute(dilations_s)) for (int i = 0; i < number_of_dilations; ++i) dilations[i] = pNode->is(dilations_s)[i];
  ::onnx::Symbol group_s = ::onnx::Symbol("group");
  int32_t group;
  if (pNode->hasAttribute(group_s)) group = pNode->i(group_s);
  ::onnx::Symbol kernel_shape_s = ::onnx::Symbol("kernel_shape");
  int32_t number_of_kernel_shape = pNode->hasAttribute(kernel_shape_s) ? pNode->is(kernel_shape_s).size() : 0;
  int32_t kernel_shape[number_of_kernel_shape];
  if (pNode->hasAttribute(kernel_shape_s)) for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pNode->is(kernel_shape_s)[i];
  ::onnx::Symbol pads_s = ::onnx::Symbol("pads");
  int32_t number_of_pads = pNode->hasAttribute(pads_s) ? pNode->is(pads_s).size() : 0;
  int32_t pads[number_of_pads];
  if (pNode->hasAttribute(pads_s)) for (int i = 0; i < number_of_pads; ++i) pads[i] = pNode->is(pads_s)[i];
  ::onnx::Symbol strides_s = ::onnx::Symbol("strides");
  int32_t number_of_strides = pNode->hasAttribute(strides_s) ? pNode->is(strides_s).size() : 0;
  int32_t strides[number_of_strides];
  if (pNode->hasAttribute(strides_s)) for (int i = 0; i < number_of_strides; ++i) strides[i] = pNode->is(strides_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "conv" << std::endl;
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


void Interpreter::visitConvTranspose(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_W_v = pNode->inputs()[1];
  void *input_W = m_ATable[input_W_v];
  int32_t input_W_ndim = input_W_v->sizes().size();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pNode->inputs().size() > 2) {
    input_B_v = pNode->inputs()[2];
    input_B = m_ATable[input_B_v];
    input_B_ndim = input_B_v->sizes().size();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol auto_pad_s = ::onnx::Symbol("auto_pad");
  const char * auto_pad;
  if (pNode->hasAttribute(auto_pad_s)) auto_pad = pNode->s(auto_pad_s).c_str();
  ::onnx::Symbol dilations_s = ::onnx::Symbol("dilations");
  int32_t number_of_dilations = pNode->hasAttribute(dilations_s) ? pNode->is(dilations_s).size() : 0;
  int32_t dilations[number_of_dilations];
  if (pNode->hasAttribute(dilations_s)) for (int i = 0; i < number_of_dilations; ++i) dilations[i] = pNode->is(dilations_s)[i];
  ::onnx::Symbol group_s = ::onnx::Symbol("group");
  int32_t group;
  if (pNode->hasAttribute(group_s)) group = pNode->i(group_s);
  ::onnx::Symbol kernel_shape_s = ::onnx::Symbol("kernel_shape");
  int32_t number_of_kernel_shape = pNode->hasAttribute(kernel_shape_s) ? pNode->is(kernel_shape_s).size() : 0;
  int32_t kernel_shape[number_of_kernel_shape];
  if (pNode->hasAttribute(kernel_shape_s)) for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pNode->is(kernel_shape_s)[i];
  ::onnx::Symbol output_padding_s = ::onnx::Symbol("output_padding");
  int32_t number_of_output_padding = pNode->hasAttribute(output_padding_s) ? pNode->is(output_padding_s).size() : 0;
  int32_t output_padding[number_of_output_padding];
  if (pNode->hasAttribute(output_padding_s)) for (int i = 0; i < number_of_output_padding; ++i) output_padding[i] = pNode->is(output_padding_s)[i];
  ::onnx::Symbol output_shape_s = ::onnx::Symbol("output_shape");
  int32_t number_of_output_shape = pNode->hasAttribute(output_shape_s) ? pNode->is(output_shape_s).size() : 0;
  int32_t output_shape[number_of_output_shape];
  if (pNode->hasAttribute(output_shape_s)) for (int i = 0; i < number_of_output_shape; ++i) output_shape[i] = pNode->is(output_shape_s)[i];
  ::onnx::Symbol pads_s = ::onnx::Symbol("pads");
  int32_t number_of_pads = pNode->hasAttribute(pads_s) ? pNode->is(pads_s).size() : 0;
  int32_t pads[number_of_pads];
  if (pNode->hasAttribute(pads_s)) for (int i = 0; i < number_of_pads; ++i) pads[i] = pNode->is(pads_s)[i];
  ::onnx::Symbol strides_s = ::onnx::Symbol("strides");
  int32_t number_of_strides = pNode->hasAttribute(strides_s) ? pNode->is(strides_s).size() : 0;
  int32_t strides[number_of_strides];
  if (pNode->hasAttribute(strides_s)) for (int i = 0; i < number_of_strides; ++i) strides[i] = pNode->is(strides_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "convtranspose" << std::endl;
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


void Interpreter::visitCos(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "cos" << std::endl;
  ONNC_RUNTIME_cos_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitDepthToSpace(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol blocksize_s = ::onnx::Symbol("blocksize");
  int32_t blocksize;
  if (pNode->hasAttribute(blocksize_s)) blocksize = pNode->i(blocksize_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "depthtospace" << std::endl;
  ONNC_RUNTIME_depthtospace_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , blocksize
  );
};


void Interpreter::visitDiv(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "div" << std::endl;
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


void Interpreter::visitDropout(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  ::onnx::Value *output_mask_v = NULL;
  void *output_mask = NULL;
  int32_t output_mask_ndim = 0;
  if (pNode->outputs().size() > 1) {
    output_mask_v = pNode->outputs()[1];
    output_mask = m_ATable[output_mask_v];
    output_mask_ndim = output_mask_v->sizes().size();
  }
  int32_t output_mask_dims[output_mask_ndim];
  for (int i = 0; i < output_mask_ndim; ++i) output_mask_dims[i] = output_mask_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol ratio_s = ::onnx::Symbol("ratio");
  float ratio;
  if (pNode->hasAttribute(ratio_s)) ratio = pNode->f(ratio_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "dropout" << std::endl;
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


void Interpreter::visitElu(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "elu" << std::endl;
  ONNC_RUNTIME_elu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
  );
};


void Interpreter::visitEqual(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "equal" << std::endl;
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


void Interpreter::visitExp(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "exp" << std::endl;
  ONNC_RUNTIME_exp_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitExpand(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  ::onnx::Value *input_shape_v = pNode->inputs()[1];
  void *input_shape = m_ATable[input_shape_v];
  int32_t input_shape_ndim = input_shape_v->sizes().size();
  int32_t input_shape_dims[input_shape_ndim];
  for (int i = 0; i < input_shape_ndim; ++i) input_shape_dims[i] = input_shape_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "expand" << std::endl;
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


void Interpreter::visitFlatten(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "flatten" << std::endl;
  ONNC_RUNTIME_flatten_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visitFloor(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "floor" << std::endl;
  ONNC_RUNTIME_floor_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitGRU(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_W_v = pNode->inputs()[1];
  void *input_W = m_ATable[input_W_v];
  int32_t input_W_ndim = input_W_v->sizes().size();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_v->sizes()[i].dim;
  ::onnx::Value *input_R_v = pNode->inputs()[2];
  void *input_R = m_ATable[input_R_v];
  int32_t input_R_ndim = input_R_v->sizes().size();
  int32_t input_R_dims[input_R_ndim];
  for (int i = 0; i < input_R_ndim; ++i) input_R_dims[i] = input_R_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pNode->inputs().size() > 3) {
    input_B_v = pNode->inputs()[3];
    input_B = m_ATable[input_B_v];
    input_B_ndim = input_B_v->sizes().size();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  ::onnx::Value *input_sequence_lens_v = NULL;
  void *input_sequence_lens = NULL;
  int32_t input_sequence_lens_ndim = 0;
  if (pNode->inputs().size() > 4) {
    input_sequence_lens_v = pNode->inputs()[4];
    input_sequence_lens = m_ATable[input_sequence_lens_v];
    input_sequence_lens_ndim = input_sequence_lens_v->sizes().size();
  }
  int32_t input_sequence_lens_dims[input_sequence_lens_ndim];
  for (int i = 0; i < input_sequence_lens_ndim; ++i) input_sequence_lens_dims[i] = input_sequence_lens_v->sizes()[i].dim;
  ::onnx::Value *input_initial_h_v = NULL;
  void *input_initial_h = NULL;
  int32_t input_initial_h_ndim = 0;
  if (pNode->inputs().size() > 5) {
    input_initial_h_v = pNode->inputs()[5];
    input_initial_h = m_ATable[input_initial_h_v];
    input_initial_h_ndim = input_initial_h_v->sizes().size();
  }
  int32_t input_initial_h_dims[input_initial_h_ndim];
  for (int i = 0; i < input_initial_h_ndim; ++i) input_initial_h_dims[i] = input_initial_h_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = NULL;
  void *output_Y = NULL;
  int32_t output_Y_ndim = 0;
  if (pNode->outputs().size() > 0) {
    output_Y_v = pNode->outputs()[0];
    output_Y = m_ATable[output_Y_v];
    output_Y_ndim = output_Y_v->sizes().size();
  }
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  ::onnx::Value *output_Y_h_v = NULL;
  void *output_Y_h = NULL;
  int32_t output_Y_h_ndim = 0;
  if (pNode->outputs().size() > 1) {
    output_Y_h_v = pNode->outputs()[1];
    output_Y_h = m_ATable[output_Y_h_v];
    output_Y_h_ndim = output_Y_h_v->sizes().size();
  }
  int32_t output_Y_h_dims[output_Y_h_ndim];
  for (int i = 0; i < output_Y_h_ndim; ++i) output_Y_h_dims[i] = output_Y_h_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol activation_alpha_s = ::onnx::Symbol("activation_alpha");
  int32_t number_of_activation_alpha = pNode->hasAttribute(activation_alpha_s) ? pNode->fs(activation_alpha_s).size() : 0;
  float activation_alpha[number_of_activation_alpha];
  if (pNode->hasAttribute(activation_alpha_s)) for (int i = 0; i < number_of_activation_alpha; ++i) activation_alpha[i] = pNode->fs(activation_alpha_s)[i];
  ::onnx::Symbol activation_beta_s = ::onnx::Symbol("activation_beta");
  int32_t number_of_activation_beta = pNode->hasAttribute(activation_beta_s) ? pNode->fs(activation_beta_s).size() : 0;
  float activation_beta[number_of_activation_beta];
  if (pNode->hasAttribute(activation_beta_s)) for (int i = 0; i < number_of_activation_beta; ++i) activation_beta[i] = pNode->fs(activation_beta_s)[i];
  ::onnx::Symbol activations_s = ::onnx::Symbol("activations");
  int32_t number_of_activations = pNode->hasAttribute(activations_s) ? pNode->ss(activations_s).size() : 0;
  const char * activations[number_of_activations];
  if (pNode->hasAttribute(activations_s)) for (int i = 0; i < number_of_activations; ++i) activations[i] = pNode->ss(activations_s)[i].c_str();
  ::onnx::Symbol clip_s = ::onnx::Symbol("clip");
  float clip;
  if (pNode->hasAttribute(clip_s)) clip = pNode->f(clip_s);
  ::onnx::Symbol direction_s = ::onnx::Symbol("direction");
  const char * direction;
  if (pNode->hasAttribute(direction_s)) direction = pNode->s(direction_s).c_str();
  ::onnx::Symbol hidden_size_s = ::onnx::Symbol("hidden_size");
  int32_t hidden_size;
  if (pNode->hasAttribute(hidden_size_s)) hidden_size = pNode->i(hidden_size_s);
  ::onnx::Symbol linear_before_reset_s = ::onnx::Symbol("linear_before_reset");
  int32_t linear_before_reset;
  if (pNode->hasAttribute(linear_before_reset_s)) linear_before_reset = pNode->i(linear_before_reset_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "gru" << std::endl;
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


void Interpreter::visitGather(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  ::onnx::Value *input_indices_v = pNode->inputs()[1];
  void *input_indices = m_ATable[input_indices_v];
  int32_t input_indices_ndim = input_indices_v->sizes().size();
  int32_t input_indices_dims[input_indices_ndim];
  for (int i = 0; i < input_indices_ndim; ++i) input_indices_dims[i] = input_indices_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "gather" << std::endl;
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


void Interpreter::visitGemm(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  ::onnx::Value *input_C_v = pNode->inputs()[2];
  void *input_C = m_ATable[input_C_v];
  int32_t input_C_ndim = input_C_v->sizes().size();
  int32_t input_C_dims[input_C_ndim];
  for (int i = 0; i < input_C_ndim; ++i) input_C_dims[i] = input_C_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);
  ::onnx::Symbol beta_s = ::onnx::Symbol("beta");
  float beta;
  if (pNode->hasAttribute(beta_s)) beta = pNode->f(beta_s);
  ::onnx::Symbol transA_s = ::onnx::Symbol("transA");
  int32_t transA;
  if (pNode->hasAttribute(transA_s)) transA = pNode->i(transA_s);
  ::onnx::Symbol transB_s = ::onnx::Symbol("transB");
  int32_t transB;
  if (pNode->hasAttribute(transB_s)) transB = pNode->i(transB_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "gemm" << std::endl;
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


void Interpreter::visitGlobalAveragePool(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "globalaveragepool" << std::endl;
  ONNC_RUNTIME_globalaveragepool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitGlobalLpPool(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol p_s = ::onnx::Symbol("p");
  int32_t p;
  if (pNode->hasAttribute(p_s)) p = pNode->i(p_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "globallppool" << std::endl;
  ONNC_RUNTIME_globallppool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , p
  );
};


void Interpreter::visitGlobalMaxPool(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "globalmaxpool" << std::endl;
  ONNC_RUNTIME_globalmaxpool_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitGreater(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "greater" << std::endl;
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


void Interpreter::visitHardSigmoid(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);
  ::onnx::Symbol beta_s = ::onnx::Symbol("beta");
  float beta;
  if (pNode->hasAttribute(beta_s)) beta = pNode->f(beta_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "hardsigmoid" << std::endl;
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


void Interpreter::visitHardmax(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "hardmax" << std::endl;
  ONNC_RUNTIME_hardmax_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visitIdentity(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "identity" << std::endl;
  ONNC_RUNTIME_identity_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitIf(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_cond_v = pNode->inputs()[0];
  void *input_cond = m_ATable[input_cond_v];
  int32_t input_cond_ndim = input_cond_v->sizes().size();
  int32_t input_cond_dims[input_cond_ndim];
  for (int i = 0; i < input_cond_ndim; ++i) input_cond_dims[i] = input_cond_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_outputs_v = pNode->outputs()[0];
  m_ATable[output_outputs_v] = allocate_output_memory(output_outputs_v);
  int32_t output_outputs_ntensor = pNode->outputs().size() - 0;
  void *output_outputs[output_outputs_ntensor]; // FIXME: = m_ATable[output_outputs_v];
  int32_t output_outputs_ndim[output_outputs_ntensor]; // FIXME: = output_outputs_v->sizes().size();
  int32_t *output_outputs_dims[output_outputs_ntensor]; // FIXME: [output_outputs_ndim[0]];
  // FIXME: for (int i = 0; i < output_outputs_ndim; ++i) output_outputs_dims[i] = output_outputs_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol else_branch_s = ::onnx::Symbol("else_branch");
  const void * else_branch;
  if (pNode->hasAttribute(else_branch_s)) else_branch = &pNode->g(else_branch_s);
  ::onnx::Symbol then_branch_s = ::onnx::Symbol("then_branch");
  const void * then_branch;
  if (pNode->hasAttribute(then_branch_s)) then_branch = &pNode->g(then_branch_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "if" << std::endl;
  ONNC_RUNTIME_if_float(
    m_pContext
    , reinterpret_cast<float *>(input_cond)
    , input_cond_ndim, input_cond_dims
    , reinterpret_cast<float **>(output_outputs)
    , output_outputs_ntensor
    , output_outputs_ndim, output_outputs_dims
    , else_branch
    , then_branch
  );
};


void Interpreter::visitInstanceNormalization(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  ::onnx::Value *input_scale_v = pNode->inputs()[1];
  void *input_scale = m_ATable[input_scale_v];
  int32_t input_scale_ndim = input_scale_v->sizes().size();
  int32_t input_scale_dims[input_scale_ndim];
  for (int i = 0; i < input_scale_ndim; ++i) input_scale_dims[i] = input_scale_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[2];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol epsilon_s = ::onnx::Symbol("epsilon");
  float epsilon;
  if (pNode->hasAttribute(epsilon_s)) epsilon = pNode->f(epsilon_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "instancenormalization" << std::endl;
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


void Interpreter::visitLRN(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);
  ::onnx::Symbol beta_s = ::onnx::Symbol("beta");
  float beta;
  if (pNode->hasAttribute(beta_s)) beta = pNode->f(beta_s);
  ::onnx::Symbol bias_s = ::onnx::Symbol("bias");
  float bias;
  if (pNode->hasAttribute(bias_s)) bias = pNode->f(bias_s);
  ::onnx::Symbol size_s = ::onnx::Symbol("size");
  int32_t size;
  if (pNode->hasAttribute(size_s)) size = pNode->i(size_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "lrn" << std::endl;
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


void Interpreter::visitLSTM(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_W_v = pNode->inputs()[1];
  void *input_W = m_ATable[input_W_v];
  int32_t input_W_ndim = input_W_v->sizes().size();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_v->sizes()[i].dim;
  ::onnx::Value *input_R_v = pNode->inputs()[2];
  void *input_R = m_ATable[input_R_v];
  int32_t input_R_ndim = input_R_v->sizes().size();
  int32_t input_R_dims[input_R_ndim];
  for (int i = 0; i < input_R_ndim; ++i) input_R_dims[i] = input_R_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pNode->inputs().size() > 3) {
    input_B_v = pNode->inputs()[3];
    input_B = m_ATable[input_B_v];
    input_B_ndim = input_B_v->sizes().size();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  ::onnx::Value *input_sequence_lens_v = NULL;
  void *input_sequence_lens = NULL;
  int32_t input_sequence_lens_ndim = 0;
  if (pNode->inputs().size() > 4) {
    input_sequence_lens_v = pNode->inputs()[4];
    input_sequence_lens = m_ATable[input_sequence_lens_v];
    input_sequence_lens_ndim = input_sequence_lens_v->sizes().size();
  }
  int32_t input_sequence_lens_dims[input_sequence_lens_ndim];
  for (int i = 0; i < input_sequence_lens_ndim; ++i) input_sequence_lens_dims[i] = input_sequence_lens_v->sizes()[i].dim;
  ::onnx::Value *input_initial_h_v = NULL;
  void *input_initial_h = NULL;
  int32_t input_initial_h_ndim = 0;
  if (pNode->inputs().size() > 5) {
    input_initial_h_v = pNode->inputs()[5];
    input_initial_h = m_ATable[input_initial_h_v];
    input_initial_h_ndim = input_initial_h_v->sizes().size();
  }
  int32_t input_initial_h_dims[input_initial_h_ndim];
  for (int i = 0; i < input_initial_h_ndim; ++i) input_initial_h_dims[i] = input_initial_h_v->sizes()[i].dim;
  ::onnx::Value *input_initial_c_v = NULL;
  void *input_initial_c = NULL;
  int32_t input_initial_c_ndim = 0;
  if (pNode->inputs().size() > 6) {
    input_initial_c_v = pNode->inputs()[6];
    input_initial_c = m_ATable[input_initial_c_v];
    input_initial_c_ndim = input_initial_c_v->sizes().size();
  }
  int32_t input_initial_c_dims[input_initial_c_ndim];
  for (int i = 0; i < input_initial_c_ndim; ++i) input_initial_c_dims[i] = input_initial_c_v->sizes()[i].dim;
  ::onnx::Value *input_P_v = NULL;
  void *input_P = NULL;
  int32_t input_P_ndim = 0;
  if (pNode->inputs().size() > 7) {
    input_P_v = pNode->inputs()[7];
    input_P = m_ATable[input_P_v];
    input_P_ndim = input_P_v->sizes().size();
  }
  int32_t input_P_dims[input_P_ndim];
  for (int i = 0; i < input_P_ndim; ++i) input_P_dims[i] = input_P_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = NULL;
  void *output_Y = NULL;
  int32_t output_Y_ndim = 0;
  if (pNode->outputs().size() > 0) {
    output_Y_v = pNode->outputs()[0];
    output_Y = m_ATable[output_Y_v];
    output_Y_ndim = output_Y_v->sizes().size();
  }
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  ::onnx::Value *output_Y_h_v = NULL;
  void *output_Y_h = NULL;
  int32_t output_Y_h_ndim = 0;
  if (pNode->outputs().size() > 1) {
    output_Y_h_v = pNode->outputs()[1];
    output_Y_h = m_ATable[output_Y_h_v];
    output_Y_h_ndim = output_Y_h_v->sizes().size();
  }
  int32_t output_Y_h_dims[output_Y_h_ndim];
  for (int i = 0; i < output_Y_h_ndim; ++i) output_Y_h_dims[i] = output_Y_h_v->sizes()[i].dim;
  ::onnx::Value *output_Y_c_v = NULL;
  void *output_Y_c = NULL;
  int32_t output_Y_c_ndim = 0;
  if (pNode->outputs().size() > 2) {
    output_Y_c_v = pNode->outputs()[2];
    output_Y_c = m_ATable[output_Y_c_v];
    output_Y_c_ndim = output_Y_c_v->sizes().size();
  }
  int32_t output_Y_c_dims[output_Y_c_ndim];
  for (int i = 0; i < output_Y_c_ndim; ++i) output_Y_c_dims[i] = output_Y_c_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol activation_alpha_s = ::onnx::Symbol("activation_alpha");
  int32_t number_of_activation_alpha = pNode->hasAttribute(activation_alpha_s) ? pNode->fs(activation_alpha_s).size() : 0;
  float activation_alpha[number_of_activation_alpha];
  if (pNode->hasAttribute(activation_alpha_s)) for (int i = 0; i < number_of_activation_alpha; ++i) activation_alpha[i] = pNode->fs(activation_alpha_s)[i];
  ::onnx::Symbol activation_beta_s = ::onnx::Symbol("activation_beta");
  int32_t number_of_activation_beta = pNode->hasAttribute(activation_beta_s) ? pNode->fs(activation_beta_s).size() : 0;
  float activation_beta[number_of_activation_beta];
  if (pNode->hasAttribute(activation_beta_s)) for (int i = 0; i < number_of_activation_beta; ++i) activation_beta[i] = pNode->fs(activation_beta_s)[i];
  ::onnx::Symbol activations_s = ::onnx::Symbol("activations");
  int32_t number_of_activations = pNode->hasAttribute(activations_s) ? pNode->ss(activations_s).size() : 0;
  const char * activations[number_of_activations];
  if (pNode->hasAttribute(activations_s)) for (int i = 0; i < number_of_activations; ++i) activations[i] = pNode->ss(activations_s)[i].c_str();
  ::onnx::Symbol clip_s = ::onnx::Symbol("clip");
  float clip;
  if (pNode->hasAttribute(clip_s)) clip = pNode->f(clip_s);
  ::onnx::Symbol direction_s = ::onnx::Symbol("direction");
  const char * direction;
  if (pNode->hasAttribute(direction_s)) direction = pNode->s(direction_s).c_str();
  ::onnx::Symbol hidden_size_s = ::onnx::Symbol("hidden_size");
  int32_t hidden_size;
  if (pNode->hasAttribute(hidden_size_s)) hidden_size = pNode->i(hidden_size_s);
  ::onnx::Symbol input_forget_s = ::onnx::Symbol("input_forget");
  int32_t input_forget;
  if (pNode->hasAttribute(input_forget_s)) input_forget = pNode->i(input_forget_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "lstm" << std::endl;
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


void Interpreter::visitLeakyRelu(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "leakyrelu" << std::endl;
  ONNC_RUNTIME_leakyrelu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
  );
};


void Interpreter::visitLess(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "less" << std::endl;
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


void Interpreter::visitLog(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "log" << std::endl;
  ONNC_RUNTIME_log_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitLogSoftmax(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "logsoftmax" << std::endl;
  ONNC_RUNTIME_logsoftmax_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visitLoop(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_M_v = pNode->inputs()[0];
  void *input_M = m_ATable[input_M_v];
  int32_t input_M_ndim = input_M_v->sizes().size();
  int32_t input_M_dims[input_M_ndim];
  for (int i = 0; i < input_M_ndim; ++i) input_M_dims[i] = input_M_v->sizes()[i].dim;
  ::onnx::Value *input_cond_v = pNode->inputs()[1];
  void *input_cond = m_ATable[input_cond_v];
  int32_t input_cond_ndim = input_cond_v->sizes().size();
  int32_t input_cond_dims[input_cond_ndim];
  for (int i = 0; i < input_cond_ndim; ++i) input_cond_dims[i] = input_cond_v->sizes()[i].dim;
  ::onnx::Value *input_v_initial_v = pNode->inputs()[2];
  int32_t input_v_initial_ntensor = pNode->inputs().size() - 2;
  void *input_v_initial[input_v_initial_ntensor]; // FIXME: = m_ATable[input_v_initial_v];
  int32_t input_v_initial_ndim[input_v_initial_ntensor]; // FIXME: = input_v_initial_v->sizes().size();
  int32_t *input_v_initial_dims[input_v_initial_ntensor]; // FIXME: [input_v_initial_ndim[0]];
  // FIXME: for (int i = 0; i < input_v_initial_ndim; ++i) input_v_initial_dims[i] = input_v_initial_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_v_final_and_scan_outputs_v = pNode->outputs()[0];
  m_ATable[output_v_final_and_scan_outputs_v] = allocate_output_memory(output_v_final_and_scan_outputs_v);
  int32_t output_v_final_and_scan_outputs_ntensor = pNode->outputs().size() - 0;
  void *output_v_final_and_scan_outputs[output_v_final_and_scan_outputs_ntensor]; // FIXME: = m_ATable[output_v_final_and_scan_outputs_v];
  int32_t output_v_final_and_scan_outputs_ndim[output_v_final_and_scan_outputs_ntensor]; // FIXME: = output_v_final_and_scan_outputs_v->sizes().size();
  int32_t *output_v_final_and_scan_outputs_dims[output_v_final_and_scan_outputs_ntensor]; // FIXME: [output_v_final_and_scan_outputs_ndim[0]];
  // FIXME: for (int i = 0; i < output_v_final_and_scan_outputs_ndim; ++i) output_v_final_and_scan_outputs_dims[i] = output_v_final_and_scan_outputs_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol body_s = ::onnx::Symbol("body");
  const void * body;
  if (pNode->hasAttribute(body_s)) body = &pNode->g(body_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "loop" << std::endl;
  ONNC_RUNTIME_loop_float(
    m_pContext
    , reinterpret_cast<float *>(input_M)
    , input_M_ndim, input_M_dims
    , reinterpret_cast<float *>(input_cond)
    , input_cond_ndim, input_cond_dims
    , reinterpret_cast<float **>(input_v_initial)
    , input_v_initial_ntensor
    , input_v_initial_ndim, input_v_initial_dims
    , reinterpret_cast<float **>(output_v_final_and_scan_outputs)
    , output_v_final_and_scan_outputs_ntensor
    , output_v_final_and_scan_outputs_ndim, output_v_final_and_scan_outputs_dims
    , body
  );
};


void Interpreter::visitLpNormalization(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);
  ::onnx::Symbol p_s = ::onnx::Symbol("p");
  int32_t p;
  if (pNode->hasAttribute(p_s)) p = pNode->i(p_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "lpnormalization" << std::endl;
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


void Interpreter::visitLpPool(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol auto_pad_s = ::onnx::Symbol("auto_pad");
  const char * auto_pad;
  if (pNode->hasAttribute(auto_pad_s)) auto_pad = pNode->s(auto_pad_s).c_str();
  ::onnx::Symbol kernel_shape_s = ::onnx::Symbol("kernel_shape");
  int32_t number_of_kernel_shape = pNode->hasAttribute(kernel_shape_s) ? pNode->is(kernel_shape_s).size() : 0;
  int32_t kernel_shape[number_of_kernel_shape];
  if (pNode->hasAttribute(kernel_shape_s)) for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pNode->is(kernel_shape_s)[i];
  ::onnx::Symbol p_s = ::onnx::Symbol("p");
  int32_t p;
  if (pNode->hasAttribute(p_s)) p = pNode->i(p_s);
  ::onnx::Symbol pads_s = ::onnx::Symbol("pads");
  int32_t number_of_pads = pNode->hasAttribute(pads_s) ? pNode->is(pads_s).size() : 0;
  int32_t pads[number_of_pads];
  if (pNode->hasAttribute(pads_s)) for (int i = 0; i < number_of_pads; ++i) pads[i] = pNode->is(pads_s)[i];
  ::onnx::Symbol strides_s = ::onnx::Symbol("strides");
  int32_t number_of_strides = pNode->hasAttribute(strides_s) ? pNode->is(strides_s).size() : 0;
  int32_t strides[number_of_strides];
  if (pNode->hasAttribute(strides_s)) for (int i = 0; i < number_of_strides; ++i) strides[i] = pNode->is(strides_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "lppool" << std::endl;
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


void Interpreter::visitMatMul(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "matmul" << std::endl;
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


void Interpreter::visitMax(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_0_v = pNode->inputs()[0];
  int32_t input_data_0_ntensor = pNode->inputs().size() - 0;
  void *input_data_0[input_data_0_ntensor]; // FIXME: = m_ATable[input_data_0_v];
  int32_t input_data_0_ndim[input_data_0_ntensor]; // FIXME: = input_data_0_v->sizes().size();
  int32_t *input_data_0_dims[input_data_0_ntensor]; // FIXME: [input_data_0_ndim[0]];
  // FIXME: for (int i = 0; i < input_data_0_ndim; ++i) input_data_0_dims[i] = input_data_0_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_max_v = pNode->outputs()[0];
  m_ATable[output_max_v] = allocate_output_memory(output_max_v);
  void *output_max = m_ATable[output_max_v];
  int32_t output_max_ndim = output_max_v->sizes().size();
  int32_t output_max_dims[output_max_ndim];
  for (int i = 0; i < output_max_ndim; ++i) output_max_dims[i] = output_max_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "max" << std::endl;
  ONNC_RUNTIME_max_float(
    m_pContext
    , reinterpret_cast<float **>(input_data_0)
    , input_data_0_ntensor
    , input_data_0_ndim, input_data_0_dims
    , reinterpret_cast<float *>(output_max)
    , output_max_ndim, output_max_dims
    
  );
};


void Interpreter::visitMaxPool(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  ::onnx::Value *output_Indices_v = NULL;
  void *output_Indices = NULL;
  int32_t output_Indices_ndim = 0;
  if (pNode->outputs().size() > 1) {
    output_Indices_v = pNode->outputs()[1];
    output_Indices = m_ATable[output_Indices_v];
    output_Indices_ndim = output_Indices_v->sizes().size();
  }
  int32_t output_Indices_dims[output_Indices_ndim];
  for (int i = 0; i < output_Indices_ndim; ++i) output_Indices_dims[i] = output_Indices_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol auto_pad_s = ::onnx::Symbol("auto_pad");
  const char * auto_pad;
  if (pNode->hasAttribute(auto_pad_s)) auto_pad = pNode->s(auto_pad_s).c_str();
  ::onnx::Symbol kernel_shape_s = ::onnx::Symbol("kernel_shape");
  int32_t number_of_kernel_shape = pNode->hasAttribute(kernel_shape_s) ? pNode->is(kernel_shape_s).size() : 0;
  int32_t kernel_shape[number_of_kernel_shape];
  if (pNode->hasAttribute(kernel_shape_s)) for (int i = 0; i < number_of_kernel_shape; ++i) kernel_shape[i] = pNode->is(kernel_shape_s)[i];
  ::onnx::Symbol pads_s = ::onnx::Symbol("pads");
  int32_t number_of_pads = pNode->hasAttribute(pads_s) ? pNode->is(pads_s).size() : 0;
  int32_t pads[number_of_pads];
  if (pNode->hasAttribute(pads_s)) for (int i = 0; i < number_of_pads; ++i) pads[i] = pNode->is(pads_s)[i];
  ::onnx::Symbol storage_order_s = ::onnx::Symbol("storage_order");
  int32_t storage_order;
  if (pNode->hasAttribute(storage_order_s)) storage_order = pNode->i(storage_order_s);
  ::onnx::Symbol strides_s = ::onnx::Symbol("strides");
  int32_t number_of_strides = pNode->hasAttribute(strides_s) ? pNode->is(strides_s).size() : 0;
  int32_t strides[number_of_strides];
  if (pNode->hasAttribute(strides_s)) for (int i = 0; i < number_of_strides; ++i) strides[i] = pNode->is(strides_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "maxpool" << std::endl;
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


void Interpreter::visitMaxRoiPool(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_rois_v = pNode->inputs()[1];
  void *input_rois = m_ATable[input_rois_v];
  int32_t input_rois_ndim = input_rois_v->sizes().size();
  int32_t input_rois_dims[input_rois_ndim];
  for (int i = 0; i < input_rois_ndim; ++i) input_rois_dims[i] = input_rois_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol pooled_shape_s = ::onnx::Symbol("pooled_shape");
  int32_t number_of_pooled_shape = pNode->hasAttribute(pooled_shape_s) ? pNode->is(pooled_shape_s).size() : 0;
  int32_t pooled_shape[number_of_pooled_shape];
  if (pNode->hasAttribute(pooled_shape_s)) for (int i = 0; i < number_of_pooled_shape; ++i) pooled_shape[i] = pNode->is(pooled_shape_s)[i];
  ::onnx::Symbol spatial_scale_s = ::onnx::Symbol("spatial_scale");
  float spatial_scale;
  if (pNode->hasAttribute(spatial_scale_s)) spatial_scale = pNode->f(spatial_scale_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "maxroipool" << std::endl;
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


void Interpreter::visitMean(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_0_v = pNode->inputs()[0];
  int32_t input_data_0_ntensor = pNode->inputs().size() - 0;
  void *input_data_0[input_data_0_ntensor]; // FIXME: = m_ATable[input_data_0_v];
  int32_t input_data_0_ndim[input_data_0_ntensor]; // FIXME: = input_data_0_v->sizes().size();
  int32_t *input_data_0_dims[input_data_0_ntensor]; // FIXME: [input_data_0_ndim[0]];
  // FIXME: for (int i = 0; i < input_data_0_ndim; ++i) input_data_0_dims[i] = input_data_0_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_mean_v = pNode->outputs()[0];
  m_ATable[output_mean_v] = allocate_output_memory(output_mean_v);
  void *output_mean = m_ATable[output_mean_v];
  int32_t output_mean_ndim = output_mean_v->sizes().size();
  int32_t output_mean_dims[output_mean_ndim];
  for (int i = 0; i < output_mean_ndim; ++i) output_mean_dims[i] = output_mean_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "mean" << std::endl;
  ONNC_RUNTIME_mean_float(
    m_pContext
    , reinterpret_cast<float **>(input_data_0)
    , input_data_0_ntensor
    , input_data_0_ndim, input_data_0_dims
    , reinterpret_cast<float *>(output_mean)
    , output_mean_ndim, output_mean_dims
    
  );
};


void Interpreter::visitMin(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_0_v = pNode->inputs()[0];
  int32_t input_data_0_ntensor = pNode->inputs().size() - 0;
  void *input_data_0[input_data_0_ntensor]; // FIXME: = m_ATable[input_data_0_v];
  int32_t input_data_0_ndim[input_data_0_ntensor]; // FIXME: = input_data_0_v->sizes().size();
  int32_t *input_data_0_dims[input_data_0_ntensor]; // FIXME: [input_data_0_ndim[0]];
  // FIXME: for (int i = 0; i < input_data_0_ndim; ++i) input_data_0_dims[i] = input_data_0_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_min_v = pNode->outputs()[0];
  m_ATable[output_min_v] = allocate_output_memory(output_min_v);
  void *output_min = m_ATable[output_min_v];
  int32_t output_min_ndim = output_min_v->sizes().size();
  int32_t output_min_dims[output_min_ndim];
  for (int i = 0; i < output_min_ndim; ++i) output_min_dims[i] = output_min_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "min" << std::endl;
  ONNC_RUNTIME_min_float(
    m_pContext
    , reinterpret_cast<float **>(input_data_0)
    , input_data_0_ntensor
    , input_data_0_ndim, input_data_0_dims
    , reinterpret_cast<float *>(output_min)
    , output_min_ndim, output_min_dims
    
  );
};


void Interpreter::visitMul(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "mul" << std::endl;
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


void Interpreter::visitMultinomial(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol dtype_s = ::onnx::Symbol("dtype");
  int32_t dtype;
  if (pNode->hasAttribute(dtype_s)) dtype = pNode->i(dtype_s);
  ::onnx::Symbol sample_size_s = ::onnx::Symbol("sample_size");
  int32_t sample_size;
  if (pNode->hasAttribute(sample_size_s)) sample_size = pNode->i(sample_size_s);
  ::onnx::Symbol seed_s = ::onnx::Symbol("seed");
  float seed;
  if (pNode->hasAttribute(seed_s)) seed = pNode->f(seed_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "multinomial" << std::endl;
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


void Interpreter::visitNeg(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "neg" << std::endl;
  ONNC_RUNTIME_neg_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitNot(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "not" << std::endl;
  ONNC_RUNTIME_not_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitOr(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "or" << std::endl;
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


void Interpreter::visitPRelu(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_slope_v = pNode->inputs()[1];
  void *input_slope = m_ATable[input_slope_v];
  int32_t input_slope_ndim = input_slope_v->sizes().size();
  int32_t input_slope_dims[input_slope_ndim];
  for (int i = 0; i < input_slope_ndim; ++i) input_slope_dims[i] = input_slope_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "prelu" << std::endl;
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


void Interpreter::visitPad(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol mode_s = ::onnx::Symbol("mode");
  const char * mode;
  if (pNode->hasAttribute(mode_s)) mode = pNode->s(mode_s).c_str();
  ::onnx::Symbol pads_s = ::onnx::Symbol("pads");
  int32_t number_of_pads = pNode->hasAttribute(pads_s) ? pNode->is(pads_s).size() : 0;
  int32_t pads[number_of_pads];
  if (pNode->hasAttribute(pads_s)) for (int i = 0; i < number_of_pads; ++i) pads[i] = pNode->is(pads_s)[i];
  ::onnx::Symbol value_s = ::onnx::Symbol("value");
  float value;
  if (pNode->hasAttribute(value_s)) value = pNode->f(value_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "pad" << std::endl;
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


void Interpreter::visitPow(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_Y_v = pNode->inputs()[1];
  void *input_Y = m_ATable[input_Y_v];
  int32_t input_Y_ndim = input_Y_v->sizes().size();
  int32_t input_Y_dims[input_Y_ndim];
  for (int i = 0; i < input_Y_ndim; ++i) input_Y_dims[i] = input_Y_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Z_v = pNode->outputs()[0];
  m_ATable[output_Z_v] = allocate_output_memory(output_Z_v);
  void *output_Z = m_ATable[output_Z_v];
  int32_t output_Z_ndim = output_Z_v->sizes().size();
  int32_t output_Z_dims[output_Z_ndim];
  for (int i = 0; i < output_Z_ndim; ++i) output_Z_dims[i] = output_Z_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "pow" << std::endl;
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


void Interpreter::visitRNN(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  ::onnx::Value *input_W_v = pNode->inputs()[1];
  void *input_W = m_ATable[input_W_v];
  int32_t input_W_ndim = input_W_v->sizes().size();
  int32_t input_W_dims[input_W_ndim];
  for (int i = 0; i < input_W_ndim; ++i) input_W_dims[i] = input_W_v->sizes()[i].dim;
  ::onnx::Value *input_R_v = pNode->inputs()[2];
  void *input_R = m_ATable[input_R_v];
  int32_t input_R_ndim = input_R_v->sizes().size();
  int32_t input_R_dims[input_R_ndim];
  for (int i = 0; i < input_R_ndim; ++i) input_R_dims[i] = input_R_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = NULL;
  void *input_B = NULL;
  int32_t input_B_ndim = 0;
  if (pNode->inputs().size() > 3) {
    input_B_v = pNode->inputs()[3];
    input_B = m_ATable[input_B_v];
    input_B_ndim = input_B_v->sizes().size();
  }
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  ::onnx::Value *input_sequence_lens_v = NULL;
  void *input_sequence_lens = NULL;
  int32_t input_sequence_lens_ndim = 0;
  if (pNode->inputs().size() > 4) {
    input_sequence_lens_v = pNode->inputs()[4];
    input_sequence_lens = m_ATable[input_sequence_lens_v];
    input_sequence_lens_ndim = input_sequence_lens_v->sizes().size();
  }
  int32_t input_sequence_lens_dims[input_sequence_lens_ndim];
  for (int i = 0; i < input_sequence_lens_ndim; ++i) input_sequence_lens_dims[i] = input_sequence_lens_v->sizes()[i].dim;
  ::onnx::Value *input_initial_h_v = NULL;
  void *input_initial_h = NULL;
  int32_t input_initial_h_ndim = 0;
  if (pNode->inputs().size() > 5) {
    input_initial_h_v = pNode->inputs()[5];
    input_initial_h = m_ATable[input_initial_h_v];
    input_initial_h_ndim = input_initial_h_v->sizes().size();
  }
  int32_t input_initial_h_dims[input_initial_h_ndim];
  for (int i = 0; i < input_initial_h_ndim; ++i) input_initial_h_dims[i] = input_initial_h_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = NULL;
  void *output_Y = NULL;
  int32_t output_Y_ndim = 0;
  if (pNode->outputs().size() > 0) {
    output_Y_v = pNode->outputs()[0];
    output_Y = m_ATable[output_Y_v];
    output_Y_ndim = output_Y_v->sizes().size();
  }
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  ::onnx::Value *output_Y_h_v = NULL;
  void *output_Y_h = NULL;
  int32_t output_Y_h_ndim = 0;
  if (pNode->outputs().size() > 1) {
    output_Y_h_v = pNode->outputs()[1];
    output_Y_h = m_ATable[output_Y_h_v];
    output_Y_h_ndim = output_Y_h_v->sizes().size();
  }
  int32_t output_Y_h_dims[output_Y_h_ndim];
  for (int i = 0; i < output_Y_h_ndim; ++i) output_Y_h_dims[i] = output_Y_h_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol activation_alpha_s = ::onnx::Symbol("activation_alpha");
  int32_t number_of_activation_alpha = pNode->hasAttribute(activation_alpha_s) ? pNode->fs(activation_alpha_s).size() : 0;
  float activation_alpha[number_of_activation_alpha];
  if (pNode->hasAttribute(activation_alpha_s)) for (int i = 0; i < number_of_activation_alpha; ++i) activation_alpha[i] = pNode->fs(activation_alpha_s)[i];
  ::onnx::Symbol activation_beta_s = ::onnx::Symbol("activation_beta");
  int32_t number_of_activation_beta = pNode->hasAttribute(activation_beta_s) ? pNode->fs(activation_beta_s).size() : 0;
  float activation_beta[number_of_activation_beta];
  if (pNode->hasAttribute(activation_beta_s)) for (int i = 0; i < number_of_activation_beta; ++i) activation_beta[i] = pNode->fs(activation_beta_s)[i];
  ::onnx::Symbol activations_s = ::onnx::Symbol("activations");
  int32_t number_of_activations = pNode->hasAttribute(activations_s) ? pNode->ss(activations_s).size() : 0;
  const char * activations[number_of_activations];
  if (pNode->hasAttribute(activations_s)) for (int i = 0; i < number_of_activations; ++i) activations[i] = pNode->ss(activations_s)[i].c_str();
  ::onnx::Symbol clip_s = ::onnx::Symbol("clip");
  float clip;
  if (pNode->hasAttribute(clip_s)) clip = pNode->f(clip_s);
  ::onnx::Symbol direction_s = ::onnx::Symbol("direction");
  const char * direction;
  if (pNode->hasAttribute(direction_s)) direction = pNode->s(direction_s).c_str();
  ::onnx::Symbol hidden_size_s = ::onnx::Symbol("hidden_size");
  int32_t hidden_size;
  if (pNode->hasAttribute(hidden_size_s)) hidden_size = pNode->i(hidden_size_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "rnn" << std::endl;
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


void Interpreter::visitRandomNormal(::onnx::Node *pNode) {
  // Prepare input
  
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol dtype_s = ::onnx::Symbol("dtype");
  int32_t dtype;
  if (pNode->hasAttribute(dtype_s)) dtype = pNode->i(dtype_s);
  ::onnx::Symbol mean_s = ::onnx::Symbol("mean");
  float mean;
  if (pNode->hasAttribute(mean_s)) mean = pNode->f(mean_s);
  ::onnx::Symbol scale_s = ::onnx::Symbol("scale");
  float scale;
  if (pNode->hasAttribute(scale_s)) scale = pNode->f(scale_s);
  ::onnx::Symbol seed_s = ::onnx::Symbol("seed");
  float seed;
  if (pNode->hasAttribute(seed_s)) seed = pNode->f(seed_s);
  ::onnx::Symbol shape_s = ::onnx::Symbol("shape");
  int32_t number_of_shape = pNode->hasAttribute(shape_s) ? pNode->is(shape_s).size() : 0;
  int32_t shape[number_of_shape];
  if (pNode->hasAttribute(shape_s)) for (int i = 0; i < number_of_shape; ++i) shape[i] = pNode->is(shape_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "randomnormal" << std::endl;
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


void Interpreter::visitRandomNormalLike(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol dtype_s = ::onnx::Symbol("dtype");
  int32_t dtype;
  if (pNode->hasAttribute(dtype_s)) dtype = pNode->i(dtype_s);
  ::onnx::Symbol mean_s = ::onnx::Symbol("mean");
  float mean;
  if (pNode->hasAttribute(mean_s)) mean = pNode->f(mean_s);
  ::onnx::Symbol scale_s = ::onnx::Symbol("scale");
  float scale;
  if (pNode->hasAttribute(scale_s)) scale = pNode->f(scale_s);
  ::onnx::Symbol seed_s = ::onnx::Symbol("seed");
  float seed;
  if (pNode->hasAttribute(seed_s)) seed = pNode->f(seed_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "randomnormallike" << std::endl;
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


void Interpreter::visitRandomUniform(::onnx::Node *pNode) {
  // Prepare input
  
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol dtype_s = ::onnx::Symbol("dtype");
  int32_t dtype;
  if (pNode->hasAttribute(dtype_s)) dtype = pNode->i(dtype_s);
  ::onnx::Symbol high_s = ::onnx::Symbol("high");
  float high;
  if (pNode->hasAttribute(high_s)) high = pNode->f(high_s);
  ::onnx::Symbol low_s = ::onnx::Symbol("low");
  float low;
  if (pNode->hasAttribute(low_s)) low = pNode->f(low_s);
  ::onnx::Symbol seed_s = ::onnx::Symbol("seed");
  float seed;
  if (pNode->hasAttribute(seed_s)) seed = pNode->f(seed_s);
  ::onnx::Symbol shape_s = ::onnx::Symbol("shape");
  int32_t number_of_shape = pNode->hasAttribute(shape_s) ? pNode->is(shape_s).size() : 0;
  int32_t shape[number_of_shape];
  if (pNode->hasAttribute(shape_s)) for (int i = 0; i < number_of_shape; ++i) shape[i] = pNode->is(shape_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "randomuniform" << std::endl;
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


void Interpreter::visitRandomUniformLike(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol dtype_s = ::onnx::Symbol("dtype");
  int32_t dtype;
  if (pNode->hasAttribute(dtype_s)) dtype = pNode->i(dtype_s);
  ::onnx::Symbol high_s = ::onnx::Symbol("high");
  float high;
  if (pNode->hasAttribute(high_s)) high = pNode->f(high_s);
  ::onnx::Symbol low_s = ::onnx::Symbol("low");
  float low;
  if (pNode->hasAttribute(low_s)) low = pNode->f(low_s);
  ::onnx::Symbol seed_s = ::onnx::Symbol("seed");
  float seed;
  if (pNode->hasAttribute(seed_s)) seed = pNode->f(seed_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "randomuniformlike" << std::endl;
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


void Interpreter::visitReciprocal(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reciprocal" << std::endl;
  ONNC_RUNTIME_reciprocal_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitReduceL1(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducel1" << std::endl;
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


void Interpreter::visitReduceL2(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducel2" << std::endl;
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


void Interpreter::visitReduceLogSum(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducelogsum" << std::endl;
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


void Interpreter::visitReduceLogSumExp(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducelogsumexp" << std::endl;
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


void Interpreter::visitReduceMax(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducemax" << std::endl;
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


void Interpreter::visitReduceMean(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducemean" << std::endl;
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


void Interpreter::visitReduceMin(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducemin" << std::endl;
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


void Interpreter::visitReduceProd(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reduceprod" << std::endl;
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


void Interpreter::visitReduceSum(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducesum" << std::endl;
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


void Interpreter::visitReduceSumSquare(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reduced_v = pNode->outputs()[0];
  m_ATable[output_reduced_v] = allocate_output_memory(output_reduced_v);
  void *output_reduced = m_ATable[output_reduced_v];
  int32_t output_reduced_ndim = output_reduced_v->sizes().size();
  int32_t output_reduced_dims[output_reduced_ndim];
  for (int i = 0; i < output_reduced_ndim; ++i) output_reduced_dims[i] = output_reduced_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol keepdims_s = ::onnx::Symbol("keepdims");
  int32_t keepdims;
  if (pNode->hasAttribute(keepdims_s)) keepdims = pNode->i(keepdims_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reducesumsquare" << std::endl;
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


void Interpreter::visitRelu(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "relu" << std::endl;
  ONNC_RUNTIME_relu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitReshape(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  ::onnx::Value *input_shape_v = pNode->inputs()[1];
  void *input_shape = m_ATable[input_shape_v];
  int32_t input_shape_ndim = input_shape_v->sizes().size();
  int32_t input_shape_dims[input_shape_ndim];
  for (int i = 0; i < input_shape_ndim; ++i) input_shape_dims[i] = input_shape_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_reshaped_v = pNode->outputs()[0];
  m_ATable[output_reshaped_v] = allocate_output_memory(output_reshaped_v);
  void *output_reshaped = m_ATable[output_reshaped_v];
  int32_t output_reshaped_ndim = output_reshaped_v->sizes().size();
  int32_t output_reshaped_dims[output_reshaped_ndim];
  for (int i = 0; i < output_reshaped_ndim; ++i) output_reshaped_dims[i] = output_reshaped_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "reshape" << std::endl;
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


void Interpreter::visitScan(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_sequence_lens_v = NULL;
  void *input_sequence_lens = NULL;
  int32_t input_sequence_lens_ndim = 0;
  if (pNode->inputs().size() > 0) {
    input_sequence_lens_v = pNode->inputs()[0];
    input_sequence_lens = m_ATable[input_sequence_lens_v];
    input_sequence_lens_ndim = input_sequence_lens_v->sizes().size();
  }
  int32_t input_sequence_lens_dims[input_sequence_lens_ndim];
  for (int i = 0; i < input_sequence_lens_ndim; ++i) input_sequence_lens_dims[i] = input_sequence_lens_v->sizes()[i].dim;
  ::onnx::Value *input_initial_state_and_scan_inputs_v = pNode->inputs()[1];
  int32_t input_initial_state_and_scan_inputs_ntensor = pNode->inputs().size() - 1;
  void *input_initial_state_and_scan_inputs[input_initial_state_and_scan_inputs_ntensor]; // FIXME: = m_ATable[input_initial_state_and_scan_inputs_v];
  int32_t input_initial_state_and_scan_inputs_ndim[input_initial_state_and_scan_inputs_ntensor]; // FIXME: = input_initial_state_and_scan_inputs_v->sizes().size();
  int32_t *input_initial_state_and_scan_inputs_dims[input_initial_state_and_scan_inputs_ntensor]; // FIXME: [input_initial_state_and_scan_inputs_ndim[0]];
  // FIXME: for (int i = 0; i < input_initial_state_and_scan_inputs_ndim; ++i) input_initial_state_and_scan_inputs_dims[i] = input_initial_state_and_scan_inputs_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_final_state_and_scan_outputs_v = pNode->outputs()[0];
  m_ATable[output_final_state_and_scan_outputs_v] = allocate_output_memory(output_final_state_and_scan_outputs_v);
  int32_t output_final_state_and_scan_outputs_ntensor = pNode->outputs().size() - 0;
  void *output_final_state_and_scan_outputs[output_final_state_and_scan_outputs_ntensor]; // FIXME: = m_ATable[output_final_state_and_scan_outputs_v];
  int32_t output_final_state_and_scan_outputs_ndim[output_final_state_and_scan_outputs_ntensor]; // FIXME: = output_final_state_and_scan_outputs_v->sizes().size();
  int32_t *output_final_state_and_scan_outputs_dims[output_final_state_and_scan_outputs_ntensor]; // FIXME: [output_final_state_and_scan_outputs_ndim[0]];
  // FIXME: for (int i = 0; i < output_final_state_and_scan_outputs_ndim; ++i) output_final_state_and_scan_outputs_dims[i] = output_final_state_and_scan_outputs_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol body_s = ::onnx::Symbol("body");
  const void * body;
  if (pNode->hasAttribute(body_s)) body = &pNode->g(body_s);
  ::onnx::Symbol directions_s = ::onnx::Symbol("directions");
  int32_t number_of_directions = pNode->hasAttribute(directions_s) ? pNode->is(directions_s).size() : 0;
  int32_t directions[number_of_directions];
  if (pNode->hasAttribute(directions_s)) for (int i = 0; i < number_of_directions; ++i) directions[i] = pNode->is(directions_s)[i];
  ::onnx::Symbol num_scan_inputs_s = ::onnx::Symbol("num_scan_inputs");
  int32_t num_scan_inputs;
  if (pNode->hasAttribute(num_scan_inputs_s)) num_scan_inputs = pNode->i(num_scan_inputs_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "scan" << std::endl;
  ONNC_RUNTIME_scan_float(
    m_pContext
    , reinterpret_cast<float *>(input_sequence_lens)
    , input_sequence_lens_ndim, input_sequence_lens_dims
    , reinterpret_cast<float **>(input_initial_state_and_scan_inputs)
    , input_initial_state_and_scan_inputs_ntensor
    , input_initial_state_and_scan_inputs_ndim, input_initial_state_and_scan_inputs_dims
    , reinterpret_cast<float **>(output_final_state_and_scan_outputs)
    , output_final_state_and_scan_outputs_ntensor
    , output_final_state_and_scan_outputs_ndim, output_final_state_and_scan_outputs_dims
    , body
    , directions
    , number_of_directions
    , num_scan_inputs
  );
};


void Interpreter::visitSelu(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);
  ::onnx::Symbol gamma_s = ::onnx::Symbol("gamma");
  float gamma;
  if (pNode->hasAttribute(gamma_s)) gamma = pNode->f(gamma_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "selu" << std::endl;
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


void Interpreter::visitShape(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_shape_v = pNode->outputs()[0];
  m_ATable[output_shape_v] = allocate_output_memory(output_shape_v);
  void *output_shape = m_ATable[output_shape_v];
  int32_t output_shape_ndim = output_shape_v->sizes().size();
  int32_t output_shape_dims[output_shape_ndim];
  for (int i = 0; i < output_shape_ndim; ++i) output_shape_dims[i] = output_shape_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "shape" << std::endl;
  ONNC_RUNTIME_shape_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_shape)
    , output_shape_ndim, output_shape_dims
    
  );
};


void Interpreter::visitSigmoid(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "sigmoid" << std::endl;
  ONNC_RUNTIME_sigmoid_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitSin(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "sin" << std::endl;
  ONNC_RUNTIME_sin_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitSize(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_size_v = pNode->outputs()[0];
  m_ATable[output_size_v] = allocate_output_memory(output_size_v);
  void *output_size = m_ATable[output_size_v];
  int32_t output_size_ndim = output_size_v->sizes().size();
  int32_t output_size_dims[output_size_ndim];
  for (int i = 0; i < output_size_ndim; ++i) output_size_dims[i] = output_size_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "size" << std::endl;
  ONNC_RUNTIME_size_float(
    m_pContext
    , reinterpret_cast<float *>(input_data)
    , input_data_ndim, input_data_dims
    , reinterpret_cast<float *>(output_size)
    , output_size_ndim, output_size_dims
    
  );
};


void Interpreter::visitSlice(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];
  ::onnx::Symbol ends_s = ::onnx::Symbol("ends");
  int32_t number_of_ends = pNode->hasAttribute(ends_s) ? pNode->is(ends_s).size() : 0;
  int32_t ends[number_of_ends];
  if (pNode->hasAttribute(ends_s)) for (int i = 0; i < number_of_ends; ++i) ends[i] = pNode->is(ends_s)[i];
  ::onnx::Symbol starts_s = ::onnx::Symbol("starts");
  int32_t number_of_starts = pNode->hasAttribute(starts_s) ? pNode->is(starts_s).size() : 0;
  int32_t starts[number_of_starts];
  if (pNode->hasAttribute(starts_s)) for (int i = 0; i < number_of_starts; ++i) starts[i] = pNode->is(starts_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "slice" << std::endl;
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


void Interpreter::visitSoftmax(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "softmax" << std::endl;
  ONNC_RUNTIME_softmax_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , axis
  );
};


void Interpreter::visitSoftplus(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "softplus" << std::endl;
  ONNC_RUNTIME_softplus_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitSoftsign(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "softsign" << std::endl;
  ONNC_RUNTIME_softsign_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitSpaceToDepth(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol blocksize_s = ::onnx::Symbol("blocksize");
  int32_t blocksize;
  if (pNode->hasAttribute(blocksize_s)) blocksize = pNode->i(blocksize_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "spacetodepth" << std::endl;
  ONNC_RUNTIME_spacetodepth_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , blocksize
  );
};


void Interpreter::visitSplit(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_outputs_v = pNode->outputs()[0];
  m_ATable[output_outputs_v] = allocate_output_memory(output_outputs_v);
  int32_t output_outputs_ntensor = pNode->outputs().size() - 0;
  void *output_outputs[output_outputs_ntensor]; // FIXME: = m_ATable[output_outputs_v];
  int32_t output_outputs_ndim[output_outputs_ntensor]; // FIXME: = output_outputs_v->sizes().size();
  int32_t *output_outputs_dims[output_outputs_ntensor]; // FIXME: [output_outputs_ndim[0]];
  // FIXME: for (int i = 0; i < output_outputs_ndim; ++i) output_outputs_dims[i] = output_outputs_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);
  ::onnx::Symbol split_s = ::onnx::Symbol("split");
  int32_t number_of_split = pNode->hasAttribute(split_s) ? pNode->is(split_s).size() : 0;
  int32_t split[number_of_split];
  if (pNode->hasAttribute(split_s)) for (int i = 0; i < number_of_split; ++i) split[i] = pNode->is(split_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "split" << std::endl;
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


void Interpreter::visitSqrt(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "sqrt" << std::endl;
  ONNC_RUNTIME_sqrt_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    
  );
};


void Interpreter::visitSqueeze(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_squeezed_v = pNode->outputs()[0];
  m_ATable[output_squeezed_v] = allocate_output_memory(output_squeezed_v);
  void *output_squeezed = m_ATable[output_squeezed_v];
  int32_t output_squeezed_ndim = output_squeezed_v->sizes().size();
  int32_t output_squeezed_dims[output_squeezed_ndim];
  for (int i = 0; i < output_squeezed_ndim; ++i) output_squeezed_dims[i] = output_squeezed_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "squeeze" << std::endl;
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


void Interpreter::visitSub(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "sub" << std::endl;
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


void Interpreter::visitSum(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_0_v = pNode->inputs()[0];
  int32_t input_data_0_ntensor = pNode->inputs().size() - 0;
  void *input_data_0[input_data_0_ntensor]; // FIXME: = m_ATable[input_data_0_v];
  int32_t input_data_0_ndim[input_data_0_ntensor]; // FIXME: = input_data_0_v->sizes().size();
  int32_t *input_data_0_dims[input_data_0_ntensor]; // FIXME: [input_data_0_ndim[0]];
  // FIXME: for (int i = 0; i < input_data_0_ndim; ++i) input_data_0_dims[i] = input_data_0_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_sum_v = pNode->outputs()[0];
  m_ATable[output_sum_v] = allocate_output_memory(output_sum_v);
  void *output_sum = m_ATable[output_sum_v];
  int32_t output_sum_ndim = output_sum_v->sizes().size();
  int32_t output_sum_dims[output_sum_ndim];
  for (int i = 0; i < output_sum_ndim; ++i) output_sum_dims[i] = output_sum_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "sum" << std::endl;
  ONNC_RUNTIME_sum_float(
    m_pContext
    , reinterpret_cast<float **>(input_data_0)
    , input_data_0_ntensor
    , input_data_0_ndim, input_data_0_dims
    , reinterpret_cast<float *>(output_sum)
    , output_sum_ndim, output_sum_dims
    
  );
};


void Interpreter::visitTan(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "tan" << std::endl;
  ONNC_RUNTIME_tan_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitTanh(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "tanh" << std::endl;
  ONNC_RUNTIME_tanh_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    
  );
};


void Interpreter::visitTile(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  ::onnx::Value *input_repeats_v = pNode->inputs()[1];
  void *input_repeats = m_ATable[input_repeats_v];
  int32_t input_repeats_ndim = input_repeats_v->sizes().size();
  int32_t input_repeats_dims[input_repeats_ndim];
  for (int i = 0; i < input_repeats_ndim; ++i) input_repeats_dims[i] = input_repeats_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "tile" << std::endl;
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


void Interpreter::visitTopK(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Values_v = pNode->outputs()[0];
  m_ATable[output_Values_v] = allocate_output_memory(output_Values_v);
  void *output_Values = m_ATable[output_Values_v];
  int32_t output_Values_ndim = output_Values_v->sizes().size();
  int32_t output_Values_dims[output_Values_ndim];
  for (int i = 0; i < output_Values_ndim; ++i) output_Values_dims[i] = output_Values_v->sizes()[i].dim;
  ::onnx::Value *output_Indices_v = pNode->outputs()[1];
  m_ATable[output_Indices_v] = allocate_output_memory(output_Indices_v);
  void *output_Indices = m_ATable[output_Indices_v];
  int32_t output_Indices_ndim = output_Indices_v->sizes().size();
  int32_t output_Indices_dims[output_Indices_ndim];
  for (int i = 0; i < output_Indices_ndim; ++i) output_Indices_dims[i] = output_Indices_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axis_s = ::onnx::Symbol("axis");
  int32_t axis;
  if (pNode->hasAttribute(axis_s)) axis = pNode->i(axis_s);
  ::onnx::Symbol k_s = ::onnx::Symbol("k");
  int32_t k;
  if (pNode->hasAttribute(k_s)) k = pNode->i(k_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "topk" << std::endl;
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


void Interpreter::visitTranspose(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_transposed_v = pNode->outputs()[0];
  m_ATable[output_transposed_v] = allocate_output_memory(output_transposed_v);
  void *output_transposed = m_ATable[output_transposed_v];
  int32_t output_transposed_ndim = output_transposed_v->sizes().size();
  int32_t output_transposed_dims[output_transposed_ndim];
  for (int i = 0; i < output_transposed_ndim; ++i) output_transposed_dims[i] = output_transposed_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol perm_s = ::onnx::Symbol("perm");
  int32_t number_of_perm = pNode->hasAttribute(perm_s) ? pNode->is(perm_s).size() : 0;
  int32_t perm[number_of_perm];
  if (pNode->hasAttribute(perm_s)) for (int i = 0; i < number_of_perm; ++i) perm[i] = pNode->is(perm_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "transpose" << std::endl;
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


void Interpreter::visitUnsqueeze(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_data_v = pNode->inputs()[0];
  void *input_data = m_ATable[input_data_v];
  int32_t input_data_ndim = input_data_v->sizes().size();
  int32_t input_data_dims[input_data_ndim];
  for (int i = 0; i < input_data_ndim; ++i) input_data_dims[i] = input_data_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_expanded_v = pNode->outputs()[0];
  m_ATable[output_expanded_v] = allocate_output_memory(output_expanded_v);
  void *output_expanded = m_ATable[output_expanded_v];
  int32_t output_expanded_ndim = output_expanded_v->sizes().size();
  int32_t output_expanded_dims[output_expanded_ndim];
  for (int i = 0; i < output_expanded_ndim; ++i) output_expanded_dims[i] = output_expanded_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol axes_s = ::onnx::Symbol("axes");
  int32_t number_of_axes = pNode->hasAttribute(axes_s) ? pNode->is(axes_s).size() : 0;
  int32_t axes[number_of_axes];
  if (pNode->hasAttribute(axes_s)) for (int i = 0; i < number_of_axes; ++i) axes[i] = pNode->is(axes_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "unsqueeze" << std::endl;
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


void Interpreter::visitUpsample(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol mode_s = ::onnx::Symbol("mode");
  const char * mode;
  if (pNode->hasAttribute(mode_s)) mode = pNode->s(mode_s).c_str();
  ::onnx::Symbol scales_s = ::onnx::Symbol("scales");
  int32_t number_of_scales = pNode->hasAttribute(scales_s) ? pNode->fs(scales_s).size() : 0;
  float scales[number_of_scales];
  if (pNode->hasAttribute(scales_s)) for (int i = 0; i < number_of_scales; ++i) scales[i] = pNode->fs(scales_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "upsample" << std::endl;
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


void Interpreter::visitXor(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_A_v = pNode->inputs()[0];
  void *input_A = m_ATable[input_A_v];
  int32_t input_A_ndim = input_A_v->sizes().size();
  int32_t input_A_dims[input_A_ndim];
  for (int i = 0; i < input_A_ndim; ++i) input_A_dims[i] = input_A_v->sizes()[i].dim;
  ::onnx::Value *input_B_v = pNode->inputs()[1];
  void *input_B = m_ATable[input_B_v];
  int32_t input_B_ndim = input_B_v->sizes().size();
  int32_t input_B_dims[input_B_ndim];
  for (int i = 0; i < input_B_ndim; ++i) input_B_dims[i] = input_B_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_C_v = pNode->outputs()[0];
  m_ATable[output_C_v] = allocate_output_memory(output_C_v);
  void *output_C = m_ATable[output_C_v];
  int32_t output_C_ndim = output_C_v->sizes().size();
  int32_t output_C_dims[output_C_ndim];
  for (int i = 0; i < output_C_ndim; ++i) output_C_dims[i] = output_C_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "xor" << std::endl;
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


void Interpreter::visitATen(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  int32_t input_input_ntensor = pNode->inputs().size() - 0;
  void *input_input[input_input_ntensor]; // FIXME: = m_ATable[input_input_v];
  int32_t input_input_ndim[input_input_ntensor]; // FIXME: = input_input_v->sizes().size();
  int32_t *input_input_dims[input_input_ntensor]; // FIXME: [input_input_ndim[0]];
  // FIXME: for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  int32_t output_output_ntensor = pNode->outputs().size() - 0;
  void *output_output[output_output_ntensor]; // FIXME: = m_ATable[output_output_v];
  int32_t output_output_ndim[output_output_ntensor]; // FIXME: = output_output_v->sizes().size();
  int32_t *output_output_dims[output_output_ntensor]; // FIXME: [output_output_ndim[0]];
  // FIXME: for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "aten" << std::endl;
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


void Interpreter::visitAffine(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);
  ::onnx::Symbol beta_s = ::onnx::Symbol("beta");
  float beta;
  if (pNode->hasAttribute(beta_s)) beta = pNode->f(beta_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "affine" << std::endl;
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


void Interpreter::visitConstantFill(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = NULL;
  void *input_input = NULL;
  int32_t input_input_ndim = 0;
  if (pNode->inputs().size() > 0) {
    input_input_v = pNode->inputs()[0];
    input_input = m_ATable[input_input_v];
    input_input_ndim = input_input_v->sizes().size();
  }
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol dtype_s = ::onnx::Symbol("dtype");
  int32_t dtype;
  if (pNode->hasAttribute(dtype_s)) dtype = pNode->i(dtype_s);
  ::onnx::Symbol extra_shape_s = ::onnx::Symbol("extra_shape");
  int32_t number_of_extra_shape = pNode->hasAttribute(extra_shape_s) ? pNode->is(extra_shape_s).size() : 0;
  int32_t extra_shape[number_of_extra_shape];
  if (pNode->hasAttribute(extra_shape_s)) for (int i = 0; i < number_of_extra_shape; ++i) extra_shape[i] = pNode->is(extra_shape_s)[i];
  ::onnx::Symbol input_as_shape_s = ::onnx::Symbol("input_as_shape");
  int32_t input_as_shape;
  if (pNode->hasAttribute(input_as_shape_s)) input_as_shape = pNode->i(input_as_shape_s);
  ::onnx::Symbol shape_s = ::onnx::Symbol("shape");
  int32_t number_of_shape = pNode->hasAttribute(shape_s) ? pNode->is(shape_s).size() : 0;
  int32_t shape[number_of_shape];
  if (pNode->hasAttribute(shape_s)) for (int i = 0; i < number_of_shape; ++i) shape[i] = pNode->is(shape_s)[i];
  ::onnx::Symbol value_s = ::onnx::Symbol("value");
  float value;
  if (pNode->hasAttribute(value_s)) value = pNode->f(value_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "constantfill" << std::endl;
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


void Interpreter::visitCrop(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol border_s = ::onnx::Symbol("border");
  int32_t number_of_border = pNode->hasAttribute(border_s) ? pNode->is(border_s).size() : 0;
  int32_t border[number_of_border];
  if (pNode->hasAttribute(border_s)) for (int i = 0; i < number_of_border; ++i) border[i] = pNode->is(border_s)[i];
  ::onnx::Symbol scale_s = ::onnx::Symbol("scale");
  int32_t number_of_scale = pNode->hasAttribute(scale_s) ? pNode->is(scale_s).size() : 0;
  int32_t scale[number_of_scale];
  if (pNode->hasAttribute(scale_s)) for (int i = 0; i < number_of_scale; ++i) scale[i] = pNode->is(scale_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "crop" << std::endl;
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


void Interpreter::visitGRUUnit(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_hidden_prev_v = pNode->inputs()[0];
  void *input_hidden_prev = m_ATable[input_hidden_prev_v];
  int32_t input_hidden_prev_ndim = input_hidden_prev_v->sizes().size();
  int32_t input_hidden_prev_dims[input_hidden_prev_ndim];
  for (int i = 0; i < input_hidden_prev_ndim; ++i) input_hidden_prev_dims[i] = input_hidden_prev_v->sizes()[i].dim;
  ::onnx::Value *input_gates_v = pNode->inputs()[1];
  void *input_gates = m_ATable[input_gates_v];
  int32_t input_gates_ndim = input_gates_v->sizes().size();
  int32_t input_gates_dims[input_gates_ndim];
  for (int i = 0; i < input_gates_ndim; ++i) input_gates_dims[i] = input_gates_v->sizes()[i].dim;
  ::onnx::Value *input_seq_lengths_v = pNode->inputs()[2];
  void *input_seq_lengths = m_ATable[input_seq_lengths_v];
  int32_t input_seq_lengths_ndim = input_seq_lengths_v->sizes().size();
  int32_t input_seq_lengths_dims[input_seq_lengths_ndim];
  for (int i = 0; i < input_seq_lengths_ndim; ++i) input_seq_lengths_dims[i] = input_seq_lengths_v->sizes()[i].dim;
  ::onnx::Value *input_t_v = pNode->inputs()[3];
  void *input_t = m_ATable[input_t_v];
  int32_t input_t_ndim = input_t_v->sizes().size();
  int32_t input_t_dims[input_t_ndim];
  for (int i = 0; i < input_t_ndim; ++i) input_t_dims[i] = input_t_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_hidden_v = pNode->outputs()[0];
  m_ATable[output_hidden_v] = allocate_output_memory(output_hidden_v);
  void *output_hidden = m_ATable[output_hidden_v];
  int32_t output_hidden_ndim = output_hidden_v->sizes().size();
  int32_t output_hidden_dims[output_hidden_ndim];
  for (int i = 0; i < output_hidden_ndim; ++i) output_hidden_dims[i] = output_hidden_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol drop_states_s = ::onnx::Symbol("drop_states");
  int32_t drop_states;
  if (pNode->hasAttribute(drop_states_s)) drop_states = pNode->i(drop_states_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "gruunit" << std::endl;
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


void Interpreter::visitGivenTensorFill(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_shape_v = NULL;
  void *input_shape = NULL;
  int32_t input_shape_ndim = 0;
  if (pNode->inputs().size() > 0) {
    input_shape_v = pNode->inputs()[0];
    input_shape = m_ATable[input_shape_v];
    input_shape_ndim = input_shape_v->sizes().size();
  }
  int32_t input_shape_dims[input_shape_ndim];
  for (int i = 0; i < input_shape_ndim; ++i) input_shape_dims[i] = input_shape_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_X_v = pNode->outputs()[0];
  m_ATable[output_X_v] = allocate_output_memory(output_X_v);
  void *output_X = m_ATable[output_X_v];
  int32_t output_X_ndim = output_X_v->sizes().size();
  int32_t output_X_dims[output_X_ndim];
  for (int i = 0; i < output_X_ndim; ++i) output_X_dims[i] = output_X_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol extra_shape_s = ::onnx::Symbol("extra_shape");
  int32_t number_of_extra_shape = pNode->hasAttribute(extra_shape_s) ? pNode->is(extra_shape_s).size() : 0;
  int32_t extra_shape[number_of_extra_shape];
  if (pNode->hasAttribute(extra_shape_s)) for (int i = 0; i < number_of_extra_shape; ++i) extra_shape[i] = pNode->is(extra_shape_s)[i];
  ::onnx::Symbol input_as_shape_s = ::onnx::Symbol("input_as_shape");
  int32_t input_as_shape;
  if (pNode->hasAttribute(input_as_shape_s)) input_as_shape = pNode->i(input_as_shape_s);
  ::onnx::Symbol shape_s = ::onnx::Symbol("shape");
  int32_t number_of_shape = pNode->hasAttribute(shape_s) ? pNode->is(shape_s).size() : 0;
  int32_t shape[number_of_shape];
  if (pNode->hasAttribute(shape_s)) for (int i = 0; i < number_of_shape; ++i) shape[i] = pNode->is(shape_s)[i];
  ::onnx::Symbol values_s = ::onnx::Symbol("values");
  int32_t number_of_values = pNode->hasAttribute(values_s) ? pNode->fs(values_s).size() : 0;
  float values[number_of_values];
  if (pNode->hasAttribute(values_s)) for (int i = 0; i < number_of_values; ++i) values[i] = pNode->fs(values_s)[i];

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "giventensorfill" << std::endl;
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


void Interpreter::visitImageScaler(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol bias_s = ::onnx::Symbol("bias");
  int32_t number_of_bias = pNode->hasAttribute(bias_s) ? pNode->fs(bias_s).size() : 0;
  float bias[number_of_bias];
  if (pNode->hasAttribute(bias_s)) for (int i = 0; i < number_of_bias; ++i) bias[i] = pNode->fs(bias_s)[i];
  ::onnx::Symbol scale_s = ::onnx::Symbol("scale");
  float scale;
  if (pNode->hasAttribute(scale_s)) scale = pNode->f(scale_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "imagescaler" << std::endl;
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


void Interpreter::visitMeanVarianceNormalization(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol across_channels_s = ::onnx::Symbol("across_channels");
  int32_t across_channels;
  if (pNode->hasAttribute(across_channels_s)) across_channels = pNode->i(across_channels_s);
  ::onnx::Symbol normalize_variance_s = ::onnx::Symbol("normalize_variance");
  int32_t normalize_variance;
  if (pNode->hasAttribute(normalize_variance_s)) normalize_variance = pNode->i(normalize_variance_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "meanvariancenormalization" << std::endl;
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


void Interpreter::visitParametricSoftplus(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);
  ::onnx::Symbol beta_s = ::onnx::Symbol("beta");
  float beta;
  if (pNode->hasAttribute(beta_s)) beta = pNode->f(beta_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "parametricsoftplus" << std::endl;
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


void Interpreter::visitScale(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol scale_s = ::onnx::Symbol("scale");
  float scale;
  if (pNode->hasAttribute(scale_s)) scale = pNode->f(scale_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "scale" << std::endl;
  ONNC_RUNTIME_scale_float(
    m_pContext
    , reinterpret_cast<float *>(input_input)
    , input_input_ndim, input_input_dims
    , reinterpret_cast<float *>(output_output)
    , output_output_ndim, output_output_dims
    , scale
  );
};


void Interpreter::visitScaledTanh(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_input_v = pNode->inputs()[0];
  void *input_input = m_ATable[input_input_v];
  int32_t input_input_ndim = input_input_v->sizes().size();
  int32_t input_input_dims[input_input_ndim];
  for (int i = 0; i < input_input_ndim; ++i) input_input_dims[i] = input_input_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_output_v = pNode->outputs()[0];
  m_ATable[output_output_v] = allocate_output_memory(output_output_v);
  void *output_output = m_ATable[output_output_v];
  int32_t output_output_ndim = output_output_v->sizes().size();
  int32_t output_output_dims[output_output_ndim];
  for (int i = 0; i < output_output_ndim; ++i) output_output_dims[i] = output_output_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);
  ::onnx::Symbol beta_s = ::onnx::Symbol("beta");
  float beta;
  if (pNode->hasAttribute(beta_s)) beta = pNode->f(beta_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "scaledtanh" << std::endl;
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


void Interpreter::visitThresholdedRelu(::onnx::Node *pNode) {
  // Prepare input
  ::onnx::Value *input_X_v = pNode->inputs()[0];
  void *input_X = m_ATable[input_X_v];
  int32_t input_X_ndim = input_X_v->sizes().size();
  int32_t input_X_dims[input_X_ndim];
  for (int i = 0; i < input_X_ndim; ++i) input_X_dims[i] = input_X_v->sizes()[i].dim;
  // Prepare output
  ::onnx::Value *output_Y_v = pNode->outputs()[0];
  m_ATable[output_Y_v] = allocate_output_memory(output_Y_v);
  void *output_Y = m_ATable[output_Y_v];
  int32_t output_Y_ndim = output_Y_v->sizes().size();
  int32_t output_Y_dims[output_Y_ndim];
  for (int i = 0; i < output_Y_ndim; ++i) output_Y_dims[i] = output_Y_v->sizes()[i].dim;
  // Prepare attributes
  ::onnx::Symbol alpha_s = ::onnx::Symbol("alpha");
  float alpha;
  if (pNode->hasAttribute(alpha_s)) alpha = pNode->f(alpha_s);

  // Call to Runtime
  print_value(pNode->inputs()[0], m_ATable[pNode->inputs()[0]]);
  errs() << "thresholdedrelu" << std::endl;
  ONNC_RUNTIME_thresholdedrelu_float(
    m_pContext
    , reinterpret_cast<float *>(input_X)
    , input_X_ndim, input_X_dims
    , reinterpret_cast<float *>(output_Y)
    , output_Y_ndim, output_Y_dims
    , alpha
  );
};

