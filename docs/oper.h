// Auto-generated
#include <list>
#include <vector>
#include <cinttypes>
#include <experimental/optional>
#include "Operator.h"

namespace onnc {
namespace onnx {

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Abs: public Operator {
 public:
  Abs():
    Operator("Abs") {};
  ~Abs() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Add: public Operator {
 public:
  Add(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Add"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Add() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class And: public Operator {
 public:
  And(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("And"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~And() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ArgMax: public Operator {
 public:
  ArgMax(AttrType::INT attr_axis, AttrType::INT attr_keepdims):
    Operator("ArgMax"), attr_axis_(attr_axis), attr_keepdims_(attr_keepdims) {};
  ~ArgMax() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  Tensor<int32_t> output_reduced();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ArgMin: public Operator {
 public:
  ArgMin(AttrType::INT attr_axis, AttrType::INT attr_keepdims):
    Operator("ArgMin"), attr_axis_(attr_axis), attr_keepdims_(attr_keepdims) {};
  ~ArgMin() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  Tensor<int32_t> output_reduced();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class AveragePool: public Operator {
 public:
  AveragePool(AttrType::STRING attr_auto_pad, AttrType::INTS attr_kernel_shape, AttrType::INTS attr_pads, AttrType::INTS attr_strides):
    Operator("AveragePool"), attr_auto_pad_(attr_auto_pad), attr_kernel_shape_(attr_kernel_shape), attr_pads_(attr_pads), attr_strides_(attr_strides) {};
  ~AveragePool() {};

  AttrType::STRING attr_auto_pad();
  AttrType::INTS attr_kernel_shape();
  AttrType::INTS attr_pads();
  AttrType::INTS attr_strides();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::STRING attr_auto_pad_;
  AttrType::INTS attr_kernel_shape_;
  AttrType::INTS attr_pads_;
  AttrType::INTS attr_strides_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class BatchNormalization: public Operator {
 public:
  BatchNormalization(AttrType::FLOAT attr_epsilon, AttrType::INT attr_is_test, AttrType::FLOAT attr_momentum, AttrType::INT attr_spatial):
    Operator("BatchNormalization"), attr_epsilon_(attr_epsilon), attr_is_test_(attr_is_test), attr_momentum_(attr_momentum), attr_spatial_(attr_spatial) {};
  ~BatchNormalization() {};

  AttrType::FLOAT attr_epsilon();
  AttrType::INT attr_is_test();
  AttrType::FLOAT attr_momentum();
  AttrType::INT attr_spatial();

  // TODO: setter
  T input_X();
  T input_scale();
  T input_B();
  T input_mean();
  T input_var();

  T output_Y();
  std::experimental::optional<T> output_mean();
  std::experimental::optional<T> output_var();
  std::experimental::optional<T> output_saved_mean();
  std::experimental::optional<T> output_saved_var();

 private:
  AttrType::FLOAT attr_epsilon_;
  AttrType::INT attr_is_test_;
  AttrType::FLOAT attr_momentum_;
  AttrType::INT attr_spatial_;
};

// TODO: Type Constraints
// T1: tensor(float16), tensor(float), tensor(double)
// T2: tensor(float16), tensor(float), tensor(double)
template<typename T1, typename T2>
class Cast: public Operator {
 public:
  Cast(AttrType::STRING attr_to):
    Operator("Cast"), attr_to_(attr_to) {};
  ~Cast() {};

  AttrType::STRING attr_to();

  // TODO: setter
  T1 input_input();

  T2 output_output();

 private:
  AttrType::STRING attr_to_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Ceil: public Operator {
 public:
  Ceil():
    Operator("Ceil") {};
  ~Ceil() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Clip: public Operator {
 public:
  Clip(AttrType::FLOAT attr_max, AttrType::FLOAT attr_min):
    Operator("Clip"), attr_max_(attr_max), attr_min_(attr_min) {};
  ~Clip() {};

  AttrType::FLOAT attr_max();
  AttrType::FLOAT attr_min();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::FLOAT attr_max_;
  AttrType::FLOAT attr_min_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Concat: public Operator {
 public:
  Concat(AttrType::INT attr_axis):
    Operator("Concat"), attr_axis_(attr_axis) {};
  ~Concat() {};

  AttrType::INT attr_axis();

  // TODO: setter
  std::list<T> input_inputs();

  T output_concat_result();

 private:
  AttrType::INT attr_axis_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Constant: public Operator {
 public:
  Constant(AttrType::TENSOR attr_value):
    Operator("Constant"), attr_value_(attr_value) {};
  ~Constant() {};

  AttrType::TENSOR attr_value();

  // TODO: setter

  T output_output();

 private:
  AttrType::TENSOR attr_value_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Conv: public Operator {
 public:
  Conv(AttrType::STRING attr_auto_pad, AttrType::INTS attr_dilations, AttrType::INT attr_group, AttrType::INTS attr_kernel_shape, AttrType::INTS attr_pads, AttrType::INTS attr_strides):
    Operator("Conv"), attr_auto_pad_(attr_auto_pad), attr_dilations_(attr_dilations), attr_group_(attr_group), attr_kernel_shape_(attr_kernel_shape), attr_pads_(attr_pads), attr_strides_(attr_strides) {};
  ~Conv() {};

  AttrType::STRING attr_auto_pad();
  AttrType::INTS attr_dilations();
  AttrType::INT attr_group();
  AttrType::INTS attr_kernel_shape();
  AttrType::INTS attr_pads();
  AttrType::INTS attr_strides();

  // TODO: setter
  T input_X();
  T input_W();
  std::experimental::optional<T> input_B();

  T output_Y();

 private:
  AttrType::STRING attr_auto_pad_;
  AttrType::INTS attr_dilations_;
  AttrType::INT attr_group_;
  AttrType::INTS attr_kernel_shape_;
  AttrType::INTS attr_pads_;
  AttrType::INTS attr_strides_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ConvTranspose: public Operator {
 public:
  ConvTranspose(AttrType::STRING attr_auto_pad, AttrType::INTS attr_dilations, AttrType::INT attr_group, AttrType::INTS attr_kernel_shape, AttrType::INTS attr_output_padding, AttrType::INTS attr_output_shape, AttrType::INTS attr_pads, AttrType::INTS attr_strides):
    Operator("ConvTranspose"), attr_auto_pad_(attr_auto_pad), attr_dilations_(attr_dilations), attr_group_(attr_group), attr_kernel_shape_(attr_kernel_shape), attr_output_padding_(attr_output_padding), attr_output_shape_(attr_output_shape), attr_pads_(attr_pads), attr_strides_(attr_strides) {};
  ~ConvTranspose() {};

  AttrType::STRING attr_auto_pad();
  AttrType::INTS attr_dilations();
  AttrType::INT attr_group();
  AttrType::INTS attr_kernel_shape();
  AttrType::INTS attr_output_padding();
  AttrType::INTS attr_output_shape();
  AttrType::INTS attr_pads();
  AttrType::INTS attr_strides();

  // TODO: setter
  T input_X();
  T input_W();
  std::experimental::optional<T> input_B();

  T output_Y();

 private:
  AttrType::STRING attr_auto_pad_;
  AttrType::INTS attr_dilations_;
  AttrType::INT attr_group_;
  AttrType::INTS attr_kernel_shape_;
  AttrType::INTS attr_output_padding_;
  AttrType::INTS attr_output_shape_;
  AttrType::INTS attr_pads_;
  AttrType::INTS attr_strides_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class DepthToSpace: public Operator {
 public:
  DepthToSpace(AttrType::INT attr_blocksize):
    Operator("DepthToSpace"), attr_blocksize_(attr_blocksize) {};
  ~DepthToSpace() {};

  AttrType::INT attr_blocksize();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INT attr_blocksize_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Div: public Operator {
 public:
  Div(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Div"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Div() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Dropout: public Operator {
 public:
  Dropout(AttrType::INT attr_is_test, AttrType::FLOAT attr_ratio):
    Operator("Dropout"), attr_is_test_(attr_is_test), attr_ratio_(attr_ratio) {};
  ~Dropout() {};

  AttrType::INT attr_is_test();
  AttrType::FLOAT attr_ratio();

  // TODO: setter
  T input_data();

  T output_output();
  std::experimental::optional<T> output_mask();

 private:
  AttrType::INT attr_is_test_;
  AttrType::FLOAT attr_ratio_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Elu: public Operator {
 public:
  Elu(AttrType::FLOAT attr_alpha):
    Operator("Elu"), attr_alpha_(attr_alpha) {};
  ~Elu() {};

  AttrType::FLOAT attr_alpha();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
};

// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64)
// T1: tensor(bool)
template<typename T, typename T1>
class Equal: public Operator {
 public:
  Equal(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Equal"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Equal() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Exp: public Operator {
 public:
  Exp():
    Operator("Exp") {};
  ~Exp() {};


  // TODO: setter
  T input_input();

  T output_output();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Flatten: public Operator {
 public:
  Flatten(AttrType::INT attr_axis):
    Operator("Flatten"), attr_axis_(attr_axis) {};
  ~Flatten() {};

  AttrType::INT attr_axis();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INT attr_axis_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Floor: public Operator {
 public:
  Floor():
    Operator("Floor") {};
  ~Floor() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class GRU: public Operator {
 public:
  GRU(AttrType::FLOATS attr_activation_alpha, AttrType::FLOATS attr_activation_beta, AttrType::STRINGS attr_activations, AttrType::FLOAT attr_clip, AttrType::STRING attr_direction, AttrType::INT attr_hidden_size, AttrType::INT attr_linear_before_reset, AttrType::INT attr_output_sequence):
    Operator("GRU"), attr_activation_alpha_(attr_activation_alpha), attr_activation_beta_(attr_activation_beta), attr_activations_(attr_activations), attr_clip_(attr_clip), attr_direction_(attr_direction), attr_hidden_size_(attr_hidden_size), attr_linear_before_reset_(attr_linear_before_reset), attr_output_sequence_(attr_output_sequence) {};
  ~GRU() {};

  AttrType::FLOATS attr_activation_alpha();
  AttrType::FLOATS attr_activation_beta();
  AttrType::STRINGS attr_activations();
  AttrType::FLOAT attr_clip();
  AttrType::STRING attr_direction();
  AttrType::INT attr_hidden_size();
  AttrType::INT attr_linear_before_reset();
  AttrType::INT attr_output_sequence();

  // TODO: setter
  T input_X();
  T input_W();
  T input_R();
  std::experimental::optional<T> input_B();
  std::experimental::optional<T1> input_sequence_lens();
  std::experimental::optional<T> input_initial_h();

  std::experimental::optional<T> output_Y();
  std::experimental::optional<T> output_Y_h();

 private:
  AttrType::FLOATS attr_activation_alpha_;
  AttrType::FLOATS attr_activation_beta_;
  AttrType::STRINGS attr_activations_;
  AttrType::FLOAT attr_clip_;
  AttrType::STRING attr_direction_;
  AttrType::INT attr_hidden_size_;
  AttrType::INT attr_linear_before_reset_;
  AttrType::INT attr_output_sequence_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// Tind: tensor(int32), tensor(int64)
template<typename T, typename Tind>
class Gather: public Operator {
 public:
  Gather(AttrType::INT attr_axis):
    Operator("Gather"), attr_axis_(attr_axis) {};
  ~Gather() {};

  AttrType::INT attr_axis();

  // TODO: setter
  T input_data();
  Tind input_indices();

  T output_output();

 private:
  AttrType::INT attr_axis_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Gemm: public Operator {
 public:
  Gemm(AttrType::FLOAT attr_alpha, AttrType::FLOAT attr_beta, AttrType::INT attr_broadcast, AttrType::INT attr_transA, AttrType::INT attr_transB):
    Operator("Gemm"), attr_alpha_(attr_alpha), attr_beta_(attr_beta), attr_broadcast_(attr_broadcast), attr_transA_(attr_transA), attr_transB_(attr_transB) {};
  ~Gemm() {};

  AttrType::FLOAT attr_alpha();
  AttrType::FLOAT attr_beta();
  AttrType::INT attr_broadcast();
  AttrType::INT attr_transA();
  AttrType::INT attr_transB();

  // TODO: setter
  T input_A();
  T input_B();
  T input_C();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
  AttrType::FLOAT attr_beta_;
  AttrType::INT attr_broadcast_;
  AttrType::INT attr_transA_;
  AttrType::INT attr_transB_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GlobalAveragePool: public Operator {
 public:
  GlobalAveragePool():
    Operator("GlobalAveragePool") {};
  ~GlobalAveragePool() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GlobalLpPool: public Operator {
 public:
  GlobalLpPool(AttrType::INT attr_p):
    Operator("GlobalLpPool"), attr_p_(attr_p) {};
  ~GlobalLpPool() {};

  AttrType::INT attr_p();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::INT attr_p_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GlobalMaxPool: public Operator {
 public:
  GlobalMaxPool():
    Operator("GlobalMaxPool") {};
  ~GlobalMaxPool() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(bool)
template<typename T, typename T1>
class Greater: public Operator {
 public:
  Greater(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Greater"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Greater() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class HardSigmoid: public Operator {
 public:
  HardSigmoid(AttrType::FLOAT attr_alpha, AttrType::FLOAT attr_beta):
    Operator("HardSigmoid"), attr_alpha_(attr_alpha), attr_beta_(attr_beta) {};
  ~HardSigmoid() {};

  AttrType::FLOAT attr_alpha();
  AttrType::FLOAT attr_beta();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
  AttrType::FLOAT attr_beta_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Hardmax: public Operator {
 public:
  Hardmax(AttrType::INT attr_axis):
    Operator("Hardmax"), attr_axis_(attr_axis) {};
  ~Hardmax() {};

  AttrType::INT attr_axis();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INT attr_axis_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class InstanceNormalization: public Operator {
 public:
  InstanceNormalization(AttrType::FLOAT attr_epsilon):
    Operator("InstanceNormalization"), attr_epsilon_(attr_epsilon) {};
  ~InstanceNormalization() {};

  AttrType::FLOAT attr_epsilon();

  // TODO: setter
  T input_input();
  T input_scale();
  T input_B();

  T output_output();

 private:
  AttrType::FLOAT attr_epsilon_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LRN: public Operator {
 public:
  LRN(AttrType::FLOAT attr_alpha, AttrType::FLOAT attr_beta, AttrType::FLOAT attr_bias, AttrType::INT attr_size):
    Operator("LRN"), attr_alpha_(attr_alpha), attr_beta_(attr_beta), attr_bias_(attr_bias), attr_size_(attr_size) {};
  ~LRN() {};

  AttrType::FLOAT attr_alpha();
  AttrType::FLOAT attr_beta();
  AttrType::FLOAT attr_bias();
  AttrType::INT attr_size();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
  AttrType::FLOAT attr_beta_;
  AttrType::FLOAT attr_bias_;
  AttrType::INT attr_size_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class LSTM: public Operator {
 public:
  LSTM(AttrType::FLOATS attr_activation_alpha, AttrType::FLOATS attr_activation_beta, AttrType::STRINGS attr_activations, AttrType::FLOAT attr_clip, AttrType::STRING attr_direction, AttrType::INT attr_hidden_size, AttrType::INT attr_input_forget, AttrType::INT attr_output_sequence):
    Operator("LSTM"), attr_activation_alpha_(attr_activation_alpha), attr_activation_beta_(attr_activation_beta), attr_activations_(attr_activations), attr_clip_(attr_clip), attr_direction_(attr_direction), attr_hidden_size_(attr_hidden_size), attr_input_forget_(attr_input_forget), attr_output_sequence_(attr_output_sequence) {};
  ~LSTM() {};

  AttrType::FLOATS attr_activation_alpha();
  AttrType::FLOATS attr_activation_beta();
  AttrType::STRINGS attr_activations();
  AttrType::FLOAT attr_clip();
  AttrType::STRING attr_direction();
  AttrType::INT attr_hidden_size();
  AttrType::INT attr_input_forget();
  AttrType::INT attr_output_sequence();

  // TODO: setter
  T input_X();
  T input_W();
  T input_R();
  std::experimental::optional<T> input_B();
  std::experimental::optional<T1> input_sequence_lens();
  std::experimental::optional<T> input_initial_h();
  std::experimental::optional<T> input_initial_c();
  std::experimental::optional<T> input_P();

  std::experimental::optional<T> output_Y();
  std::experimental::optional<T> output_Y_h();
  std::experimental::optional<T> output_Y_c();

 private:
  AttrType::FLOATS attr_activation_alpha_;
  AttrType::FLOATS attr_activation_beta_;
  AttrType::STRINGS attr_activations_;
  AttrType::FLOAT attr_clip_;
  AttrType::STRING attr_direction_;
  AttrType::INT attr_hidden_size_;
  AttrType::INT attr_input_forget_;
  AttrType::INT attr_output_sequence_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LeakyRelu: public Operator {
 public:
  LeakyRelu(AttrType::FLOAT attr_alpha):
    Operator("LeakyRelu"), attr_alpha_(attr_alpha) {};
  ~LeakyRelu() {};

  AttrType::FLOAT attr_alpha();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(bool)
template<typename T, typename T1>
class Less: public Operator {
 public:
  Less(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Less"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Less() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Log: public Operator {
 public:
  Log():
    Operator("Log") {};
  ~Log() {};


  // TODO: setter
  T input_input();

  T output_output();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LogSoftmax: public Operator {
 public:
  LogSoftmax(AttrType::INT attr_axis):
    Operator("LogSoftmax"), attr_axis_(attr_axis) {};
  ~LogSoftmax() {};

  AttrType::INT attr_axis();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INT attr_axis_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LpNormalization: public Operator {
 public:
  LpNormalization(AttrType::INT attr_axis, AttrType::INT attr_p):
    Operator("LpNormalization"), attr_axis_(attr_axis), attr_p_(attr_p) {};
  ~LpNormalization() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_p();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_p_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LpPool: public Operator {
 public:
  LpPool(AttrType::STRING attr_auto_pad, AttrType::INTS attr_kernel_shape, AttrType::INT attr_p, AttrType::INTS attr_pads, AttrType::INTS attr_strides):
    Operator("LpPool"), attr_auto_pad_(attr_auto_pad), attr_kernel_shape_(attr_kernel_shape), attr_p_(attr_p), attr_pads_(attr_pads), attr_strides_(attr_strides) {};
  ~LpPool() {};

  AttrType::STRING attr_auto_pad();
  AttrType::INTS attr_kernel_shape();
  AttrType::INT attr_p();
  AttrType::INTS attr_pads();
  AttrType::INTS attr_strides();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::STRING attr_auto_pad_;
  AttrType::INTS attr_kernel_shape_;
  AttrType::INT attr_p_;
  AttrType::INTS attr_pads_;
  AttrType::INTS attr_strides_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MatMul: public Operator {
 public:
  MatMul():
    Operator("MatMul") {};
  ~MatMul() {};


  // TODO: setter
  T input_A();
  T input_B();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Max: public Operator {
 public:
  Max():
    Operator("Max") {};
  ~Max() {};


  // TODO: setter
  std::list<T> input_data_0();

  T output_max();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MaxPool: public Operator {
 public:
  MaxPool(AttrType::STRING attr_auto_pad, AttrType::INTS attr_kernel_shape, AttrType::INTS attr_pads, AttrType::INTS attr_strides):
    Operator("MaxPool"), attr_auto_pad_(attr_auto_pad), attr_kernel_shape_(attr_kernel_shape), attr_pads_(attr_pads), attr_strides_(attr_strides) {};
  ~MaxPool() {};

  AttrType::STRING attr_auto_pad();
  AttrType::INTS attr_kernel_shape();
  AttrType::INTS attr_pads();
  AttrType::INTS attr_strides();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::STRING attr_auto_pad_;
  AttrType::INTS attr_kernel_shape_;
  AttrType::INTS attr_pads_;
  AttrType::INTS attr_strides_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MaxRoiPool: public Operator {
 public:
  MaxRoiPool(AttrType::INTS attr_pooled_shape, AttrType::FLOAT attr_spatial_scale):
    Operator("MaxRoiPool"), attr_pooled_shape_(attr_pooled_shape), attr_spatial_scale_(attr_spatial_scale) {};
  ~MaxRoiPool() {};

  AttrType::INTS attr_pooled_shape();
  AttrType::FLOAT attr_spatial_scale();

  // TODO: setter
  T input_X();
  T input_rois();

  T output_Y();

 private:
  AttrType::INTS attr_pooled_shape_;
  AttrType::FLOAT attr_spatial_scale_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Mean: public Operator {
 public:
  Mean():
    Operator("Mean") {};
  ~Mean() {};


  // TODO: setter
  std::list<T> input_data_0();

  T output_mean();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Min: public Operator {
 public:
  Min():
    Operator("Min") {};
  ~Min() {};


  // TODO: setter
  std::list<T> input_data_0();

  T output_min();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Mul: public Operator {
 public:
  Mul(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Mul"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Mul() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Neg: public Operator {
 public:
  Neg():
    Operator("Neg") {};
  ~Neg() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(bool)
template<typename T>
class Not: public Operator {
 public:
  Not():
    Operator("Not") {};
  ~Not() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class Or: public Operator {
 public:
  Or(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Or"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Or() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class PRelu: public Operator {
 public:
  PRelu():
    Operator("PRelu") {};
  ~PRelu() {};


  // TODO: setter
  T input_X();
  T input_slope();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Pad: public Operator {
 public:
  Pad(AttrType::STRING attr_mode, AttrType::INTS attr_pads, AttrType::FLOAT attr_value):
    Operator("Pad"), attr_mode_(attr_mode), attr_pads_(attr_pads), attr_value_(attr_value) {};
  ~Pad() {};

  AttrType::STRING attr_mode();
  AttrType::INTS attr_pads();
  AttrType::FLOAT attr_value();

  // TODO: setter
  T input_data();

  T output_output();

 private:
  AttrType::STRING attr_mode_;
  AttrType::INTS attr_pads_;
  AttrType::FLOAT attr_value_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Pow: public Operator {
 public:
  Pow():
    Operator("Pow") {};
  ~Pow() {};


  // TODO: setter
  T input_X();
  T input_Y();

  T output_Z();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class RNN: public Operator {
 public:
  RNN(AttrType::FLOATS attr_activation_alpha, AttrType::FLOATS attr_activation_beta, AttrType::STRINGS attr_activations, AttrType::FLOAT attr_clip, AttrType::STRING attr_direction, AttrType::INT attr_hidden_size, AttrType::INT attr_output_sequence):
    Operator("RNN"), attr_activation_alpha_(attr_activation_alpha), attr_activation_beta_(attr_activation_beta), attr_activations_(attr_activations), attr_clip_(attr_clip), attr_direction_(attr_direction), attr_hidden_size_(attr_hidden_size), attr_output_sequence_(attr_output_sequence) {};
  ~RNN() {};

  AttrType::FLOATS attr_activation_alpha();
  AttrType::FLOATS attr_activation_beta();
  AttrType::STRINGS attr_activations();
  AttrType::FLOAT attr_clip();
  AttrType::STRING attr_direction();
  AttrType::INT attr_hidden_size();
  AttrType::INT attr_output_sequence();

  // TODO: setter
  T input_X();
  T input_W();
  T input_R();
  std::experimental::optional<T> input_B();
  std::experimental::optional<T1> input_sequence_lens();
  std::experimental::optional<T> input_initial_h();

  std::experimental::optional<T> output_Y();
  std::experimental::optional<T> output_Y_h();

 private:
  AttrType::FLOATS attr_activation_alpha_;
  AttrType::FLOATS attr_activation_beta_;
  AttrType::STRINGS attr_activations_;
  AttrType::FLOAT attr_clip_;
  AttrType::STRING attr_direction_;
  AttrType::INT attr_hidden_size_;
  AttrType::INT attr_output_sequence_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomNormal: public Operator {
 public:
  RandomNormal(AttrType::INT attr_dtype, AttrType::FLOAT attr_mean, AttrType::FLOAT attr_scale, AttrType::FLOAT attr_seed, AttrType::INTS attr_shape):
    Operator("RandomNormal"), attr_dtype_(attr_dtype), attr_mean_(attr_mean), attr_scale_(attr_scale), attr_seed_(attr_seed), attr_shape_(attr_shape) {};
  ~RandomNormal() {};

  AttrType::INT attr_dtype();
  AttrType::FLOAT attr_mean();
  AttrType::FLOAT attr_scale();
  AttrType::FLOAT attr_seed();
  AttrType::INTS attr_shape();

  // TODO: setter

  T output_output();

 private:
  AttrType::INT attr_dtype_;
  AttrType::FLOAT attr_mean_;
  AttrType::FLOAT attr_scale_;
  AttrType::FLOAT attr_seed_;
  AttrType::INTS attr_shape_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomNormalLike: public Operator {
 public:
  RandomNormalLike(AttrType::INT attr_dtype, AttrType::FLOAT attr_mean, AttrType::FLOAT attr_scale, AttrType::FLOAT attr_seed):
    Operator("RandomNormalLike"), attr_dtype_(attr_dtype), attr_mean_(attr_mean), attr_scale_(attr_scale), attr_seed_(attr_seed) {};
  ~RandomNormalLike() {};

  AttrType::INT attr_dtype();
  AttrType::FLOAT attr_mean();
  AttrType::FLOAT attr_scale();
  AttrType::FLOAT attr_seed();

  // TODO: setter
  Tensor<int32_t> input_input();

  T output_output();

 private:
  AttrType::INT attr_dtype_;
  AttrType::FLOAT attr_mean_;
  AttrType::FLOAT attr_scale_;
  AttrType::FLOAT attr_seed_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomUniform: public Operator {
 public:
  RandomUniform(AttrType::INT attr_dtype, AttrType::FLOAT attr_high, AttrType::FLOAT attr_low, AttrType::FLOAT attr_seed, AttrType::INTS attr_shape):
    Operator("RandomUniform"), attr_dtype_(attr_dtype), attr_high_(attr_high), attr_low_(attr_low), attr_seed_(attr_seed), attr_shape_(attr_shape) {};
  ~RandomUniform() {};

  AttrType::INT attr_dtype();
  AttrType::FLOAT attr_high();
  AttrType::FLOAT attr_low();
  AttrType::FLOAT attr_seed();
  AttrType::INTS attr_shape();

  // TODO: setter

  T output_output();

 private:
  AttrType::INT attr_dtype_;
  AttrType::FLOAT attr_high_;
  AttrType::FLOAT attr_low_;
  AttrType::FLOAT attr_seed_;
  AttrType::INTS attr_shape_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomUniformLike: public Operator {
 public:
  RandomUniformLike(AttrType::INT attr_dtype, AttrType::FLOAT attr_high, AttrType::FLOAT attr_low, AttrType::FLOAT attr_seed):
    Operator("RandomUniformLike"), attr_dtype_(attr_dtype), attr_high_(attr_high), attr_low_(attr_low), attr_seed_(attr_seed) {};
  ~RandomUniformLike() {};

  AttrType::INT attr_dtype();
  AttrType::FLOAT attr_high();
  AttrType::FLOAT attr_low();
  AttrType::FLOAT attr_seed();

  // TODO: setter
  Tensor<int32_t> input_input();

  T output_output();

 private:
  AttrType::INT attr_dtype_;
  AttrType::FLOAT attr_high_;
  AttrType::FLOAT attr_low_;
  AttrType::FLOAT attr_seed_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Reciprocal: public Operator {
 public:
  Reciprocal():
    Operator("Reciprocal") {};
  ~Reciprocal() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceL1: public Operator {
 public:
  ReduceL1(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceL1"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceL1() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceL2: public Operator {
 public:
  ReduceL2(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceL2"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceL2() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceLogSum: public Operator {
 public:
  ReduceLogSum(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceLogSum"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceLogSum() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceLogSumExp: public Operator {
 public:
  ReduceLogSumExp(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceLogSumExp"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceLogSumExp() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMax: public Operator {
 public:
  ReduceMax(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceMax"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceMax() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMean: public Operator {
 public:
  ReduceMean(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceMean"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceMean() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMin: public Operator {
 public:
  ReduceMin(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceMin"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceMin() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceProd: public Operator {
 public:
  ReduceProd(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceProd"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceProd() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceSum: public Operator {
 public:
  ReduceSum(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceSum"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceSum() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceSumSquare: public Operator {
 public:
  ReduceSumSquare(AttrType::INTS attr_axes, AttrType::INT attr_keepdims):
    Operator("ReduceSumSquare"), attr_axes_(attr_axes), attr_keepdims_(attr_keepdims) {};
  ~ReduceSumSquare() {};

  AttrType::INTS attr_axes();
  AttrType::INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INT attr_keepdims_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Relu: public Operator {
 public:
  Relu():
    Operator("Relu") {};
  ~Relu() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Reshape: public Operator {
 public:
  Reshape(AttrType::INTS attr_shape):
    Operator("Reshape"), attr_shape_(attr_shape) {};
  ~Reshape() {};

  AttrType::INTS attr_shape();

  // TODO: setter
  T input_data();

  T output_reshaped();

 private:
  AttrType::INTS attr_shape_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Selu: public Operator {
 public:
  Selu(AttrType::FLOAT attr_alpha, AttrType::FLOAT attr_gamma):
    Operator("Selu"), attr_alpha_(attr_alpha), attr_gamma_(attr_gamma) {};
  ~Selu() {};

  AttrType::FLOAT attr_alpha();
  AttrType::FLOAT attr_gamma();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
  AttrType::FLOAT attr_gamma_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Sigmoid: public Operator {
 public:
  Sigmoid():
    Operator("Sigmoid") {};
  ~Sigmoid() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Slice: public Operator {
 public:
  Slice(AttrType::INTS attr_axes, AttrType::INTS attr_ends, AttrType::INTS attr_starts):
    Operator("Slice"), attr_axes_(attr_axes), attr_ends_(attr_ends), attr_starts_(attr_starts) {};
  ~Slice() {};

  AttrType::INTS attr_axes();
  AttrType::INTS attr_ends();
  AttrType::INTS attr_starts();

  // TODO: setter
  T input_data();

  T output_output();

 private:
  AttrType::INTS attr_axes_;
  AttrType::INTS attr_ends_;
  AttrType::INTS attr_starts_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Softmax: public Operator {
 public:
  Softmax(AttrType::INT attr_axis):
    Operator("Softmax"), attr_axis_(attr_axis) {};
  ~Softmax() {};

  AttrType::INT attr_axis();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INT attr_axis_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Softplus: public Operator {
 public:
  Softplus():
    Operator("Softplus") {};
  ~Softplus() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Softsign: public Operator {
 public:
  Softsign():
    Operator("Softsign") {};
  ~Softsign() {};


  // TODO: setter
  T input_input();

  T output_output();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SpaceToDepth: public Operator {
 public:
  SpaceToDepth(AttrType::INT attr_blocksize):
    Operator("SpaceToDepth"), attr_blocksize_(attr_blocksize) {};
  ~SpaceToDepth() {};

  AttrType::INT attr_blocksize();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INT attr_blocksize_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Split: public Operator {
 public:
  Split(AttrType::INT attr_axis, AttrType::INTS attr_split):
    Operator("Split"), attr_axis_(attr_axis), attr_split_(attr_split) {};
  ~Split() {};

  AttrType::INT attr_axis();
  AttrType::INTS attr_split();

  // TODO: setter
  T input_input();

  std::list<T> output_outputs();

 private:
  AttrType::INT attr_axis_;
  AttrType::INTS attr_split_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Sqrt: public Operator {
 public:
  Sqrt():
    Operator("Sqrt") {};
  ~Sqrt() {};


  // TODO: setter
  T input_X();

  T output_Y();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Squeeze: public Operator {
 public:
  Squeeze(AttrType::INTS attr_axes):
    Operator("Squeeze"), attr_axes_(attr_axes) {};
  ~Squeeze() {};

  AttrType::INTS attr_axes();

  // TODO: setter
  T input_data();

  T output_squeezed();

 private:
  AttrType::INTS attr_axes_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Sub: public Operator {
 public:
  Sub(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Sub"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Sub() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Sum: public Operator {
 public:
  Sum():
    Operator("Sum") {};
  ~Sum() {};


  // TODO: setter
  std::list<T> input_data_0();

  T output_sum();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Tanh: public Operator {
 public:
  Tanh():
    Operator("Tanh") {};
  ~Tanh() {};


  // TODO: setter
  T input_input();

  T output_output();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Tile: public Operator {
 public:
  Tile():
    Operator("Tile") {};
  ~Tile() {};


  // TODO: setter
  T input_input();
  T input_tiles();
  T input_axis();

  T output_output();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Transpose: public Operator {
 public:
  Transpose(AttrType::INTS attr_perm):
    Operator("Transpose"), attr_perm_(attr_perm) {};
  ~Transpose() {};

  AttrType::INTS attr_perm();

  // TODO: setter
  T input_data();

  T output_transposed();

 private:
  AttrType::INTS attr_perm_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Unsqueeze: public Operator {
 public:
  Unsqueeze(AttrType::INTS attr_axes):
    Operator("Unsqueeze"), attr_axes_(attr_axes) {};
  ~Unsqueeze() {};

  AttrType::INTS attr_axes();

  // TODO: setter
  T input_data();

  T output_expanded();

 private:
  AttrType::INTS attr_axes_;
};

// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class Xor: public Operator {
 public:
  Xor(AttrType::INT attr_axis, AttrType::INT attr_broadcast):
    Operator("Xor"), attr_axis_(attr_axis), attr_broadcast_(attr_broadcast) {};
  ~Xor() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_broadcast_;
};

// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64), tensor(float16), tensor(float), tensor(double)
template<typename T>
class ATen: public Operator {
 public:
  ATen():
    Operator("ATen") {};
  ~ATen() {};


  // TODO: setter
  std::list<T> input_input();

  std::list<T> output_output();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Affine: public Operator {
 public:
  Affine(AttrType::FLOAT attr_alpha, AttrType::FLOAT attr_beta):
    Operator("Affine"), attr_alpha_(attr_alpha), attr_beta_(attr_beta) {};
  ~Affine() {};

  AttrType::FLOAT attr_alpha();
  AttrType::FLOAT attr_beta();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
  AttrType::FLOAT attr_beta_;
};

// TODO: Type Constraints
// T1: tensor(float), tensor(int32), tensor(int64), tensor(bool)
// T2: tensor(float), tensor(int32), tensor(int64), tensor(bool)
template<typename T1, typename T2>
class ConstantFill: public Operator {
 public:
  ConstantFill(AttrType::INT attr_dtype, AttrType::INTS attr_extra_shape, AttrType::INT attr_input_as_shape, AttrType::INTS attr_shape, AttrType::FLOAT attr_value):
    Operator("ConstantFill"), attr_dtype_(attr_dtype), attr_extra_shape_(attr_extra_shape), attr_input_as_shape_(attr_input_as_shape), attr_shape_(attr_shape), attr_value_(attr_value) {};
  ~ConstantFill() {};

  AttrType::INT attr_dtype();
  AttrType::INTS attr_extra_shape();
  AttrType::INT attr_input_as_shape();
  AttrType::INTS attr_shape();
  AttrType::FLOAT attr_value();

  // TODO: setter
  std::experimental::optional<T1> input_input();

  T2 output_output();

 private:
  AttrType::INT attr_dtype_;
  AttrType::INTS attr_extra_shape_;
  AttrType::INT attr_input_as_shape_;
  AttrType::INTS attr_shape_;
  AttrType::FLOAT attr_value_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Crop: public Operator {
 public:
  Crop(AttrType::INTS attr_border, AttrType::INTS attr_scale):
    Operator("Crop"), attr_border_(attr_border), attr_scale_(attr_scale) {};
  ~Crop() {};

  AttrType::INTS attr_border();
  AttrType::INTS attr_scale();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INTS attr_border_;
  AttrType::INTS attr_scale_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class FC: public Operator {
 public:
  FC(AttrType::INT attr_axis, AttrType::INT attr_axis_w):
    Operator("FC"), attr_axis_(attr_axis), attr_axis_w_(attr_axis_w) {};
  ~FC() {};

  AttrType::INT attr_axis();
  AttrType::INT attr_axis_w();

  // TODO: setter
  T input_X();
  T input_W();
  T input_B();

  T output_Y();

 private:
  AttrType::INT attr_axis_;
  AttrType::INT attr_axis_w_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GRUUnit: public Operator {
 public:
  GRUUnit(AttrType::INT attr_drop_states):
    Operator("GRUUnit"), attr_drop_states_(attr_drop_states) {};
  ~GRUUnit() {};

  AttrType::INT attr_drop_states();

  // TODO: setter
  T input_hidden_prev();
  T input_gates();
  T input_seq_lengths();
  T input_t();

  T output_hidden();

 private:
  AttrType::INT attr_drop_states_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GivenTensorFill: public Operator {
 public:
  GivenTensorFill(AttrType::INTS attr_extra_shape, AttrType::INT attr_input_as_shape, AttrType::INTS attr_shape, AttrType::FLOATS attr_values):
    Operator("GivenTensorFill"), attr_extra_shape_(attr_extra_shape), attr_input_as_shape_(attr_input_as_shape), attr_shape_(attr_shape), attr_values_(attr_values) {};
  ~GivenTensorFill() {};

  AttrType::INTS attr_extra_shape();
  AttrType::INT attr_input_as_shape();
  AttrType::INTS attr_shape();
  AttrType::FLOATS attr_values();

  // TODO: setter
  std::experimental::optional<T> input_shape();

  T output_X();

 private:
  AttrType::INTS attr_extra_shape_;
  AttrType::INT attr_input_as_shape_;
  AttrType::INTS attr_shape_;
  AttrType::FLOATS attr_values_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Identity: public Operator {
 public:
  Identity():
    Operator("Identity") {};
  ~Identity() {};


  // TODO: setter
  T input_input();

  T output_output();

 private:
};

// TODO: Type Constraints
// V: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// B: tensor(bool)
template<typename V, typename B>
class If: public Operator {
 public:
  If(AttrType::GRAPH attr_else_branch, AttrType::GRAPH attr_then_branch):
    Operator("If"), attr_else_branch_(attr_else_branch), attr_then_branch_(attr_then_branch) {};
  ~If() {};

  AttrType::GRAPH attr_else_branch();
  AttrType::GRAPH attr_then_branch();

  // TODO: setter
  B input_cond();

  std::list<V> output_outputs();

 private:
  AttrType::GRAPH attr_else_branch_;
  AttrType::GRAPH attr_then_branch_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ImageScaler: public Operator {
 public:
  ImageScaler(AttrType::FLOATS attr_bias, AttrType::FLOAT attr_scale):
    Operator("ImageScaler"), attr_bias_(attr_bias), attr_scale_(attr_scale) {};
  ~ImageScaler() {};

  AttrType::FLOATS attr_bias();
  AttrType::FLOAT attr_scale();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::FLOATS attr_bias_;
  AttrType::FLOAT attr_scale_;
};

// TODO: Type Constraints
// V: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// I: int64
// B: bool
template<typename V, typename I, typename B>
class Loop: public Operator {
 public:
  Loop(AttrType::GRAPH attr_body):
    Operator("Loop"), attr_body_(attr_body) {};
  ~Loop() {};

  AttrType::GRAPH attr_body();

  // TODO: setter
  I input_M();
  B input_cond();
  std::list<V> input_v_initial();

  std::list<V> output_v_final_and_scan_outputs();

 private:
  AttrType::GRAPH attr_body_;
};

// TODO: Type Constraints
// T: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// I: int32
template<typename T, typename I>
class LoopIndexTensor: public Operator {
 public:
  LoopIndexTensor(AttrType::INT attr_axis):
    Operator("LoopIndexTensor"), attr_axis_(attr_axis) {};
  ~LoopIndexTensor() {};

  AttrType::INT attr_axis();

  // TODO: setter
  T input_T();
  I input_loop_idx();

  T output_O();

 private:
  AttrType::INT attr_axis_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MeanVarianceNormalization: public Operator {
 public:
  MeanVarianceNormalization(AttrType::INT attr_across_channels, AttrType::INT attr_normalize_variance):
    Operator("MeanVarianceNormalization"), attr_across_channels_(attr_across_channels), attr_normalize_variance_(attr_normalize_variance) {};
  ~MeanVarianceNormalization() {};

  AttrType::INT attr_across_channels();
  AttrType::INT attr_normalize_variance();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::INT attr_across_channels_;
  AttrType::INT attr_normalize_variance_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ParametricSoftplus: public Operator {
 public:
  ParametricSoftplus(AttrType::FLOAT attr_alpha, AttrType::FLOAT attr_beta):
    Operator("ParametricSoftplus"), attr_alpha_(attr_alpha), attr_beta_(attr_beta) {};
  ~ParametricSoftplus() {};

  AttrType::FLOAT attr_alpha();
  AttrType::FLOAT attr_beta();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
  AttrType::FLOAT attr_beta_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Scale: public Operator {
 public:
  Scale(AttrType::FLOAT attr_scale):
    Operator("Scale"), attr_scale_(attr_scale) {};
  ~Scale() {};

  AttrType::FLOAT attr_scale();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::FLOAT attr_scale_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ScaledTanh: public Operator {
 public:
  ScaledTanh(AttrType::FLOAT attr_alpha, AttrType::FLOAT attr_beta):
    Operator("ScaledTanh"), attr_alpha_(attr_alpha), attr_beta_(attr_beta) {};
  ~ScaledTanh() {};

  AttrType::FLOAT attr_alpha();
  AttrType::FLOAT attr_beta();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType::FLOAT attr_alpha_;
  AttrType::FLOAT attr_beta_;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ThresholdedRelu: public Operator {
 public:
  ThresholdedRelu(AttrType::FLOAT attr_alpha):
    Operator("ThresholdedRelu"), attr_alpha_(attr_alpha) {};
  ~ThresholdedRelu() {};

  AttrType::FLOAT attr_alpha();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_alpha_;
};

// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64), tensor(float16), tensor(float), tensor(double)
template<typename T>
class Upsample: public Operator {
 public:
  Upsample(AttrType::FLOAT attr_height_scale, AttrType::STRING attr_mode, AttrType::FLOAT attr_width_scale):
    Operator("Upsample"), attr_height_scale_(attr_height_scale), attr_mode_(attr_mode), attr_width_scale_(attr_width_scale) {};
  ~Upsample() {};

  AttrType::FLOAT attr_height_scale();
  AttrType::STRING attr_mode();
  AttrType::FLOAT attr_width_scale();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType::FLOAT attr_height_scale_;
  AttrType::STRING attr_mode_;
  AttrType::FLOAT attr_width_scale_;
};


} // namespace onnx
} // namespace onnc

