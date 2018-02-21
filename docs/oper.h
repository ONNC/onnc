#include<Operator.h>

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
  Add(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Add"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Add() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
};

// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class And: public Operator {
 public:
  And(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("And"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~And() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ArgMax: public Operator {
 public:
  ArgMax(AttrType.INT p_attr_axis, AttrType.INT p_attr_keepdims):
    Operator("ArgMax"), attr_axis(p_attr_axis), attr_keepdims(p_attr_keepdims) {};
  ~ArgMax() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  tensor(int32) output_reduced();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ArgMin: public Operator {
 public:
  ArgMin(AttrType.INT p_attr_axis, AttrType.INT p_attr_keepdims):
    Operator("ArgMin"), attr_axis(p_attr_axis), attr_keepdims(p_attr_keepdims) {};
  ~ArgMin() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  tensor(int32) output_reduced();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class AveragePool: public Operator {
 public:
  AveragePool(AttrType.STRING p_attr_auto_pad, AttrType.INTS p_attr_kernel_shape, AttrType.INTS p_attr_pads, AttrType.INTS p_attr_strides):
    Operator("AveragePool"), attr_auto_pad(p_attr_auto_pad), attr_kernel_shape(p_attr_kernel_shape), attr_pads(p_attr_pads), attr_strides(p_attr_strides) {};
  ~AveragePool() {};

  AttrType.STRING attr_auto_pad();
  AttrType.INTS attr_kernel_shape();
  AttrType.INTS attr_pads();
  AttrType.INTS attr_strides();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.STRING attr_auto_pad;
  AttrType.INTS attr_kernel_shape;
  AttrType.INTS attr_pads;
  AttrType.INTS attr_strides;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class BatchNormalization: public Operator {
 public:
  BatchNormalization(AttrType.FLOAT p_attr_epsilon, AttrType.INT p_attr_is_test, AttrType.FLOAT p_attr_momentum, AttrType.INT p_attr_spatial):
    Operator("BatchNormalization"), attr_epsilon(p_attr_epsilon), attr_is_test(p_attr_is_test), attr_momentum(p_attr_momentum), attr_spatial(p_attr_spatial) {};
  ~BatchNormalization() {};

  AttrType.FLOAT attr_epsilon();
  AttrType.INT attr_is_test();
  AttrType.FLOAT attr_momentum();
  AttrType.INT attr_spatial();

  // TODO: setter
  T input_X();
  T input_scale();
  T input_B();
  T input_mean();
  T input_var();

  T output_Y();
  std::optional<T> output_mean();
  std::optional<T> output_var();
  std::optional<T> output_saved_mean();
  std::optional<T> output_saved_var();

 private:
  AttrType.FLOAT attr_epsilon;
  AttrType.INT attr_is_test;
  AttrType.FLOAT attr_momentum;
  AttrType.INT attr_spatial;
};

// TODO: Type Constraints
// T1: tensor(float16), tensor(float), tensor(double)
// T2: tensor(float16), tensor(float), tensor(double)
template<typename T1, typename T2>
class Cast: public Operator {
 public:
  Cast(AttrType.STRING p_attr_to):
    Operator("Cast"), attr_to(p_attr_to) {};
  ~Cast() {};

  AttrType.STRING attr_to();

  // TODO: setter
  T1 input_input();

  T2 output_output();

 private:
  AttrType.STRING attr_to;
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
  Clip(AttrType.FLOAT p_attr_max, AttrType.FLOAT p_attr_min):
    Operator("Clip"), attr_max(p_attr_max), attr_min(p_attr_min) {};
  ~Clip() {};

  AttrType.FLOAT attr_max();
  AttrType.FLOAT attr_min();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.FLOAT attr_max;
  AttrType.FLOAT attr_min;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Concat: public Operator {
 public:
  Concat(AttrType.INT p_attr_axis):
    Operator("Concat"), attr_axis(p_attr_axis) {};
  ~Concat() {};

  AttrType.INT attr_axis();

  // TODO: setter
  std::list<T> input_inputs();

  T output_concat_result();

 private:
  AttrType.INT attr_axis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Constant: public Operator {
 public:
  Constant(AttrType.TENSOR p_attr_value):
    Operator("Constant"), attr_value(p_attr_value) {};
  ~Constant() {};

  AttrType.TENSOR attr_value();

  // TODO: setter

  T output_output();

 private:
  AttrType.TENSOR attr_value;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Conv: public Operator {
 public:
  Conv(AttrType.STRING p_attr_auto_pad, AttrType.INTS p_attr_dilations, AttrType.INT p_attr_group, AttrType.INTS p_attr_kernel_shape, AttrType.INTS p_attr_pads, AttrType.INTS p_attr_strides):
    Operator("Conv"), attr_auto_pad(p_attr_auto_pad), attr_dilations(p_attr_dilations), attr_group(p_attr_group), attr_kernel_shape(p_attr_kernel_shape), attr_pads(p_attr_pads), attr_strides(p_attr_strides) {};
  ~Conv() {};

  AttrType.STRING attr_auto_pad();
  AttrType.INTS attr_dilations();
  AttrType.INT attr_group();
  AttrType.INTS attr_kernel_shape();
  AttrType.INTS attr_pads();
  AttrType.INTS attr_strides();

  // TODO: setter
  T input_X();
  T input_W();
  std::optional<T> input_B();

  T output_Y();

 private:
  AttrType.STRING attr_auto_pad;
  AttrType.INTS attr_dilations;
  AttrType.INT attr_group;
  AttrType.INTS attr_kernel_shape;
  AttrType.INTS attr_pads;
  AttrType.INTS attr_strides;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ConvTranspose: public Operator {
 public:
  ConvTranspose(AttrType.STRING p_attr_auto_pad, AttrType.INTS p_attr_dilations, AttrType.INT p_attr_group, AttrType.INTS p_attr_kernel_shape, AttrType.INTS p_attr_output_padding, AttrType.INTS p_attr_output_shape, AttrType.INTS p_attr_pads, AttrType.INTS p_attr_strides):
    Operator("ConvTranspose"), attr_auto_pad(p_attr_auto_pad), attr_dilations(p_attr_dilations), attr_group(p_attr_group), attr_kernel_shape(p_attr_kernel_shape), attr_output_padding(p_attr_output_padding), attr_output_shape(p_attr_output_shape), attr_pads(p_attr_pads), attr_strides(p_attr_strides) {};
  ~ConvTranspose() {};

  AttrType.STRING attr_auto_pad();
  AttrType.INTS attr_dilations();
  AttrType.INT attr_group();
  AttrType.INTS attr_kernel_shape();
  AttrType.INTS attr_output_padding();
  AttrType.INTS attr_output_shape();
  AttrType.INTS attr_pads();
  AttrType.INTS attr_strides();

  // TODO: setter
  T input_X();
  T input_W();
  std::optional<T> input_B();

  T output_Y();

 private:
  AttrType.STRING attr_auto_pad;
  AttrType.INTS attr_dilations;
  AttrType.INT attr_group;
  AttrType.INTS attr_kernel_shape;
  AttrType.INTS attr_output_padding;
  AttrType.INTS attr_output_shape;
  AttrType.INTS attr_pads;
  AttrType.INTS attr_strides;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class DepthToSpace: public Operator {
 public:
  DepthToSpace(AttrType.INT p_attr_blocksize):
    Operator("DepthToSpace"), attr_blocksize(p_attr_blocksize) {};
  ~DepthToSpace() {};

  AttrType.INT attr_blocksize();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INT attr_blocksize;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Div: public Operator {
 public:
  Div(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Div"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Div() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Dropout: public Operator {
 public:
  Dropout(AttrType.INT p_attr_is_test, AttrType.FLOAT p_attr_ratio):
    Operator("Dropout"), attr_is_test(p_attr_is_test), attr_ratio(p_attr_ratio) {};
  ~Dropout() {};

  AttrType.INT attr_is_test();
  AttrType.FLOAT attr_ratio();

  // TODO: setter
  T input_data();

  T output_output();
  std::optional<T> output_mask();

 private:
  AttrType.INT attr_is_test;
  AttrType.FLOAT attr_ratio;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Elu: public Operator {
 public:
  Elu(AttrType.FLOAT p_attr_alpha):
    Operator("Elu"), attr_alpha(p_attr_alpha) {};
  ~Elu() {};

  AttrType.FLOAT attr_alpha();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
};

// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64)
// T1: tensor(bool)
template<typename T, typename T1>
class Equal: public Operator {
 public:
  Equal(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Equal"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Equal() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
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
  Flatten(AttrType.INT p_attr_axis):
    Operator("Flatten"), attr_axis(p_attr_axis) {};
  ~Flatten() {};

  AttrType.INT attr_axis();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INT attr_axis;
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
  GRU(AttrType.FLOATS p_attr_activation_alpha, AttrType.FLOATS p_attr_activation_beta, AttrType.STRINGS p_attr_activations, AttrType.FLOAT p_attr_clip, AttrType.STRING p_attr_direction, AttrType.INT p_attr_hidden_size, AttrType.INT p_attr_linear_before_reset, AttrType.INT p_attr_output_sequence):
    Operator("GRU"), attr_activation_alpha(p_attr_activation_alpha), attr_activation_beta(p_attr_activation_beta), attr_activations(p_attr_activations), attr_clip(p_attr_clip), attr_direction(p_attr_direction), attr_hidden_size(p_attr_hidden_size), attr_linear_before_reset(p_attr_linear_before_reset), attr_output_sequence(p_attr_output_sequence) {};
  ~GRU() {};

  AttrType.FLOATS attr_activation_alpha();
  AttrType.FLOATS attr_activation_beta();
  AttrType.STRINGS attr_activations();
  AttrType.FLOAT attr_clip();
  AttrType.STRING attr_direction();
  AttrType.INT attr_hidden_size();
  AttrType.INT attr_linear_before_reset();
  AttrType.INT attr_output_sequence();

  // TODO: setter
  T input_X();
  T input_W();
  T input_R();
  std::optional<T> input_B();
  std::optional<T1> input_sequence_lens();
  std::optional<T> input_initial_h();

  std::optional<T> output_Y();
  std::optional<T> output_Y_h();

 private:
  AttrType.FLOATS attr_activation_alpha;
  AttrType.FLOATS attr_activation_beta;
  AttrType.STRINGS attr_activations;
  AttrType.FLOAT attr_clip;
  AttrType.STRING attr_direction;
  AttrType.INT attr_hidden_size;
  AttrType.INT attr_linear_before_reset;
  AttrType.INT attr_output_sequence;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// Tind: tensor(int32), tensor(int64)
template<typename T, typename Tind>
class Gather: public Operator {
 public:
  Gather(AttrType.INT p_attr_axis):
    Operator("Gather"), attr_axis(p_attr_axis) {};
  ~Gather() {};

  AttrType.INT attr_axis();

  // TODO: setter
  T input_data();
  Tind input_indices();

  T output_output();

 private:
  AttrType.INT attr_axis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Gemm: public Operator {
 public:
  Gemm(AttrType.FLOAT p_attr_alpha, AttrType.FLOAT p_attr_beta, AttrType.INT p_attr_broadcast, AttrType.INT p_attr_transA, AttrType.INT p_attr_transB):
    Operator("Gemm"), attr_alpha(p_attr_alpha), attr_beta(p_attr_beta), attr_broadcast(p_attr_broadcast), attr_transA(p_attr_transA), attr_transB(p_attr_transB) {};
  ~Gemm() {};

  AttrType.FLOAT attr_alpha();
  AttrType.FLOAT attr_beta();
  AttrType.INT attr_broadcast();
  AttrType.INT attr_transA();
  AttrType.INT attr_transB();

  // TODO: setter
  T input_A();
  T input_B();
  T input_C();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
  AttrType.FLOAT attr_beta;
  AttrType.INT attr_broadcast;
  AttrType.INT attr_transA;
  AttrType.INT attr_transB;
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
  GlobalLpPool(AttrType.INT p_attr_p):
    Operator("GlobalLpPool"), attr_p(p_attr_p) {};
  ~GlobalLpPool() {};

  AttrType.INT attr_p();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.INT attr_p;
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
  Greater(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Greater"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Greater() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class HardSigmoid: public Operator {
 public:
  HardSigmoid(AttrType.FLOAT p_attr_alpha, AttrType.FLOAT p_attr_beta):
    Operator("HardSigmoid"), attr_alpha(p_attr_alpha), attr_beta(p_attr_beta) {};
  ~HardSigmoid() {};

  AttrType.FLOAT attr_alpha();
  AttrType.FLOAT attr_beta();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
  AttrType.FLOAT attr_beta;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Hardmax: public Operator {
 public:
  Hardmax(AttrType.INT p_attr_axis):
    Operator("Hardmax"), attr_axis(p_attr_axis) {};
  ~Hardmax() {};

  AttrType.INT attr_axis();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INT attr_axis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class InstanceNormalization: public Operator {
 public:
  InstanceNormalization(AttrType.FLOAT p_attr_epsilon):
    Operator("InstanceNormalization"), attr_epsilon(p_attr_epsilon) {};
  ~InstanceNormalization() {};

  AttrType.FLOAT attr_epsilon();

  // TODO: setter
  T input_input();
  T input_scale();
  T input_B();

  T output_output();

 private:
  AttrType.FLOAT attr_epsilon;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LRN: public Operator {
 public:
  LRN(AttrType.FLOAT p_attr_alpha, AttrType.FLOAT p_attr_beta, AttrType.FLOAT p_attr_bias, AttrType.INT p_attr_size):
    Operator("LRN"), attr_alpha(p_attr_alpha), attr_beta(p_attr_beta), attr_bias(p_attr_bias), attr_size(p_attr_size) {};
  ~LRN() {};

  AttrType.FLOAT attr_alpha();
  AttrType.FLOAT attr_beta();
  AttrType.FLOAT attr_bias();
  AttrType.INT attr_size();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
  AttrType.FLOAT attr_beta;
  AttrType.FLOAT attr_bias;
  AttrType.INT attr_size;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class LSTM: public Operator {
 public:
  LSTM(AttrType.FLOATS p_attr_activation_alpha, AttrType.FLOATS p_attr_activation_beta, AttrType.STRINGS p_attr_activations, AttrType.FLOAT p_attr_clip, AttrType.STRING p_attr_direction, AttrType.INT p_attr_hidden_size, AttrType.INT p_attr_input_forget, AttrType.INT p_attr_output_sequence):
    Operator("LSTM"), attr_activation_alpha(p_attr_activation_alpha), attr_activation_beta(p_attr_activation_beta), attr_activations(p_attr_activations), attr_clip(p_attr_clip), attr_direction(p_attr_direction), attr_hidden_size(p_attr_hidden_size), attr_input_forget(p_attr_input_forget), attr_output_sequence(p_attr_output_sequence) {};
  ~LSTM() {};

  AttrType.FLOATS attr_activation_alpha();
  AttrType.FLOATS attr_activation_beta();
  AttrType.STRINGS attr_activations();
  AttrType.FLOAT attr_clip();
  AttrType.STRING attr_direction();
  AttrType.INT attr_hidden_size();
  AttrType.INT attr_input_forget();
  AttrType.INT attr_output_sequence();

  // TODO: setter
  T input_X();
  T input_W();
  T input_R();
  std::optional<T> input_B();
  std::optional<T1> input_sequence_lens();
  std::optional<T> input_initial_h();
  std::optional<T> input_initial_c();
  std::optional<T> input_P();

  std::optional<T> output_Y();
  std::optional<T> output_Y_h();
  std::optional<T> output_Y_c();

 private:
  AttrType.FLOATS attr_activation_alpha;
  AttrType.FLOATS attr_activation_beta;
  AttrType.STRINGS attr_activations;
  AttrType.FLOAT attr_clip;
  AttrType.STRING attr_direction;
  AttrType.INT attr_hidden_size;
  AttrType.INT attr_input_forget;
  AttrType.INT attr_output_sequence;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LeakyRelu: public Operator {
 public:
  LeakyRelu(AttrType.FLOAT p_attr_alpha):
    Operator("LeakyRelu"), attr_alpha(p_attr_alpha) {};
  ~LeakyRelu() {};

  AttrType.FLOAT attr_alpha();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(bool)
template<typename T, typename T1>
class Less: public Operator {
 public:
  Less(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Less"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Less() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
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
  LogSoftmax(AttrType.INT p_attr_axis):
    Operator("LogSoftmax"), attr_axis(p_attr_axis) {};
  ~LogSoftmax() {};

  AttrType.INT attr_axis();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INT attr_axis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LpNormalization: public Operator {
 public:
  LpNormalization(AttrType.INT p_attr_axis, AttrType.INT p_attr_p):
    Operator("LpNormalization"), attr_axis(p_attr_axis), attr_p(p_attr_p) {};
  ~LpNormalization() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_p();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_p;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LpPool: public Operator {
 public:
  LpPool(AttrType.STRING p_attr_auto_pad, AttrType.INTS p_attr_kernel_shape, AttrType.INT p_attr_p, AttrType.INTS p_attr_pads, AttrType.INTS p_attr_strides):
    Operator("LpPool"), attr_auto_pad(p_attr_auto_pad), attr_kernel_shape(p_attr_kernel_shape), attr_p(p_attr_p), attr_pads(p_attr_pads), attr_strides(p_attr_strides) {};
  ~LpPool() {};

  AttrType.STRING attr_auto_pad();
  AttrType.INTS attr_kernel_shape();
  AttrType.INT attr_p();
  AttrType.INTS attr_pads();
  AttrType.INTS attr_strides();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.STRING attr_auto_pad;
  AttrType.INTS attr_kernel_shape;
  AttrType.INT attr_p;
  AttrType.INTS attr_pads;
  AttrType.INTS attr_strides;
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
  MaxPool(AttrType.STRING p_attr_auto_pad, AttrType.INTS p_attr_kernel_shape, AttrType.INTS p_attr_pads, AttrType.INTS p_attr_strides):
    Operator("MaxPool"), attr_auto_pad(p_attr_auto_pad), attr_kernel_shape(p_attr_kernel_shape), attr_pads(p_attr_pads), attr_strides(p_attr_strides) {};
  ~MaxPool() {};

  AttrType.STRING attr_auto_pad();
  AttrType.INTS attr_kernel_shape();
  AttrType.INTS attr_pads();
  AttrType.INTS attr_strides();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.STRING attr_auto_pad;
  AttrType.INTS attr_kernel_shape;
  AttrType.INTS attr_pads;
  AttrType.INTS attr_strides;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MaxRoiPool: public Operator {
 public:
  MaxRoiPool(AttrType.INTS p_attr_pooled_shape, AttrType.FLOAT p_attr_spatial_scale):
    Operator("MaxRoiPool"), attr_pooled_shape(p_attr_pooled_shape), attr_spatial_scale(p_attr_spatial_scale) {};
  ~MaxRoiPool() {};

  AttrType.INTS attr_pooled_shape();
  AttrType.FLOAT attr_spatial_scale();

  // TODO: setter
  T input_X();
  T input_rois();

  T output_Y();

 private:
  AttrType.INTS attr_pooled_shape;
  AttrType.FLOAT attr_spatial_scale;
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
  Mul(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Mul"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Mul() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
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
  Or(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Or"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Or() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
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
  Pad(AttrType.STRING p_attr_mode, AttrType.INTS p_attr_pads, AttrType.FLOAT p_attr_value):
    Operator("Pad"), attr_mode(p_attr_mode), attr_pads(p_attr_pads), attr_value(p_attr_value) {};
  ~Pad() {};

  AttrType.STRING attr_mode();
  AttrType.INTS attr_pads();
  AttrType.FLOAT attr_value();

  // TODO: setter
  T input_data();

  T output_output();

 private:
  AttrType.STRING attr_mode;
  AttrType.INTS attr_pads;
  AttrType.FLOAT attr_value;
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
  RNN(AttrType.FLOATS p_attr_activation_alpha, AttrType.FLOATS p_attr_activation_beta, AttrType.STRINGS p_attr_activations, AttrType.FLOAT p_attr_clip, AttrType.STRING p_attr_direction, AttrType.INT p_attr_hidden_size, AttrType.INT p_attr_output_sequence):
    Operator("RNN"), attr_activation_alpha(p_attr_activation_alpha), attr_activation_beta(p_attr_activation_beta), attr_activations(p_attr_activations), attr_clip(p_attr_clip), attr_direction(p_attr_direction), attr_hidden_size(p_attr_hidden_size), attr_output_sequence(p_attr_output_sequence) {};
  ~RNN() {};

  AttrType.FLOATS attr_activation_alpha();
  AttrType.FLOATS attr_activation_beta();
  AttrType.STRINGS attr_activations();
  AttrType.FLOAT attr_clip();
  AttrType.STRING attr_direction();
  AttrType.INT attr_hidden_size();
  AttrType.INT attr_output_sequence();

  // TODO: setter
  T input_X();
  T input_W();
  T input_R();
  std::optional<T> input_B();
  std::optional<T1> input_sequence_lens();
  std::optional<T> input_initial_h();

  std::optional<T> output_Y();
  std::optional<T> output_Y_h();

 private:
  AttrType.FLOATS attr_activation_alpha;
  AttrType.FLOATS attr_activation_beta;
  AttrType.STRINGS attr_activations;
  AttrType.FLOAT attr_clip;
  AttrType.STRING attr_direction;
  AttrType.INT attr_hidden_size;
  AttrType.INT attr_output_sequence;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomNormal: public Operator {
 public:
  RandomNormal(AttrType.INT p_attr_dtype, AttrType.FLOAT p_attr_mean, AttrType.FLOAT p_attr_scale, AttrType.FLOAT p_attr_seed, AttrType.INTS p_attr_shape):
    Operator("RandomNormal"), attr_dtype(p_attr_dtype), attr_mean(p_attr_mean), attr_scale(p_attr_scale), attr_seed(p_attr_seed), attr_shape(p_attr_shape) {};
  ~RandomNormal() {};

  AttrType.INT attr_dtype();
  AttrType.FLOAT attr_mean();
  AttrType.FLOAT attr_scale();
  AttrType.FLOAT attr_seed();
  AttrType.INTS attr_shape();

  // TODO: setter

  T output_output();

 private:
  AttrType.INT attr_dtype;
  AttrType.FLOAT attr_mean;
  AttrType.FLOAT attr_scale;
  AttrType.FLOAT attr_seed;
  AttrType.INTS attr_shape;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomNormalLike: public Operator {
 public:
  RandomNormalLike(AttrType.INT p_attr_dtype, AttrType.FLOAT p_attr_mean, AttrType.FLOAT p_attr_scale, AttrType.FLOAT p_attr_seed):
    Operator("RandomNormalLike"), attr_dtype(p_attr_dtype), attr_mean(p_attr_mean), attr_scale(p_attr_scale), attr_seed(p_attr_seed) {};
  ~RandomNormalLike() {};

  AttrType.INT attr_dtype();
  AttrType.FLOAT attr_mean();
  AttrType.FLOAT attr_scale();
  AttrType.FLOAT attr_seed();

  // TODO: setter
  tensor(int32) input_input();

  T output_output();

 private:
  AttrType.INT attr_dtype;
  AttrType.FLOAT attr_mean;
  AttrType.FLOAT attr_scale;
  AttrType.FLOAT attr_seed;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomUniform: public Operator {
 public:
  RandomUniform(AttrType.INT p_attr_dtype, AttrType.FLOAT p_attr_high, AttrType.FLOAT p_attr_low, AttrType.FLOAT p_attr_seed, AttrType.INTS p_attr_shape):
    Operator("RandomUniform"), attr_dtype(p_attr_dtype), attr_high(p_attr_high), attr_low(p_attr_low), attr_seed(p_attr_seed), attr_shape(p_attr_shape) {};
  ~RandomUniform() {};

  AttrType.INT attr_dtype();
  AttrType.FLOAT attr_high();
  AttrType.FLOAT attr_low();
  AttrType.FLOAT attr_seed();
  AttrType.INTS attr_shape();

  // TODO: setter

  T output_output();

 private:
  AttrType.INT attr_dtype;
  AttrType.FLOAT attr_high;
  AttrType.FLOAT attr_low;
  AttrType.FLOAT attr_seed;
  AttrType.INTS attr_shape;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomUniformLike: public Operator {
 public:
  RandomUniformLike(AttrType.INT p_attr_dtype, AttrType.FLOAT p_attr_high, AttrType.FLOAT p_attr_low, AttrType.FLOAT p_attr_seed):
    Operator("RandomUniformLike"), attr_dtype(p_attr_dtype), attr_high(p_attr_high), attr_low(p_attr_low), attr_seed(p_attr_seed) {};
  ~RandomUniformLike() {};

  AttrType.INT attr_dtype();
  AttrType.FLOAT attr_high();
  AttrType.FLOAT attr_low();
  AttrType.FLOAT attr_seed();

  // TODO: setter
  tensor(int32) input_input();

  T output_output();

 private:
  AttrType.INT attr_dtype;
  AttrType.FLOAT attr_high;
  AttrType.FLOAT attr_low;
  AttrType.FLOAT attr_seed;
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
  ReduceL1(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceL1"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceL1() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceL2: public Operator {
 public:
  ReduceL2(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceL2"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceL2() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceLogSum: public Operator {
 public:
  ReduceLogSum(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceLogSum"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceLogSum() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceLogSumExp: public Operator {
 public:
  ReduceLogSumExp(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceLogSumExp"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceLogSumExp() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMax: public Operator {
 public:
  ReduceMax(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceMax"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceMax() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMean: public Operator {
 public:
  ReduceMean(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceMean"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceMean() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMin: public Operator {
 public:
  ReduceMin(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceMin"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceMin() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceProd: public Operator {
 public:
  ReduceProd(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceProd"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceProd() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceSum: public Operator {
 public:
  ReduceSum(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceSum"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceSum() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceSumSquare: public Operator {
 public:
  ReduceSumSquare(AttrType.INTS p_attr_axes, AttrType.INT p_attr_keepdims):
    Operator("ReduceSumSquare"), attr_axes(p_attr_axes), attr_keepdims(p_attr_keepdims) {};
  ~ReduceSumSquare() {};

  AttrType.INTS attr_axes();
  AttrType.INT attr_keepdims();

  // TODO: setter
  T input_data();

  T output_reduced();

 private:
  AttrType.INTS attr_axes;
  AttrType.INT attr_keepdims;
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
  Reshape(AttrType.INTS p_attr_shape):
    Operator("Reshape"), attr_shape(p_attr_shape) {};
  ~Reshape() {};

  AttrType.INTS attr_shape();

  // TODO: setter
  T input_data();

  T output_reshaped();

 private:
  AttrType.INTS attr_shape;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Selu: public Operator {
 public:
  Selu(AttrType.FLOAT p_attr_alpha, AttrType.FLOAT p_attr_gamma):
    Operator("Selu"), attr_alpha(p_attr_alpha), attr_gamma(p_attr_gamma) {};
  ~Selu() {};

  AttrType.FLOAT attr_alpha();
  AttrType.FLOAT attr_gamma();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
  AttrType.FLOAT attr_gamma;
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
  Slice(AttrType.INTS p_attr_axes, AttrType.INTS p_attr_ends, AttrType.INTS p_attr_starts):
    Operator("Slice"), attr_axes(p_attr_axes), attr_ends(p_attr_ends), attr_starts(p_attr_starts) {};
  ~Slice() {};

  AttrType.INTS attr_axes();
  AttrType.INTS attr_ends();
  AttrType.INTS attr_starts();

  // TODO: setter
  T input_data();

  T output_output();

 private:
  AttrType.INTS attr_axes;
  AttrType.INTS attr_ends;
  AttrType.INTS attr_starts;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Softmax: public Operator {
 public:
  Softmax(AttrType.INT p_attr_axis):
    Operator("Softmax"), attr_axis(p_attr_axis) {};
  ~Softmax() {};

  AttrType.INT attr_axis();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INT attr_axis;
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
  SpaceToDepth(AttrType.INT p_attr_blocksize):
    Operator("SpaceToDepth"), attr_blocksize(p_attr_blocksize) {};
  ~SpaceToDepth() {};

  AttrType.INT attr_blocksize();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INT attr_blocksize;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Split: public Operator {
 public:
  Split(AttrType.INT p_attr_axis, AttrType.INTS p_attr_split):
    Operator("Split"), attr_axis(p_attr_axis), attr_split(p_attr_split) {};
  ~Split() {};

  AttrType.INT attr_axis();
  AttrType.INTS attr_split();

  // TODO: setter
  T input_input();

  std::list<T> output_outputs();

 private:
  AttrType.INT attr_axis;
  AttrType.INTS attr_split;
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
  Squeeze(AttrType.INTS p_attr_axes):
    Operator("Squeeze"), attr_axes(p_attr_axes) {};
  ~Squeeze() {};

  AttrType.INTS attr_axes();

  // TODO: setter
  T input_data();

  T output_squeezed();

 private:
  AttrType.INTS attr_axes;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Sub: public Operator {
 public:
  Sub(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Sub"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Sub() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
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
  Transpose(AttrType.INTS p_attr_perm):
    Operator("Transpose"), attr_perm(p_attr_perm) {};
  ~Transpose() {};

  AttrType.INTS attr_perm();

  // TODO: setter
  T input_data();

  T output_transposed();

 private:
  AttrType.INTS attr_perm;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Unsqueeze: public Operator {
 public:
  Unsqueeze(AttrType.INTS p_attr_axes):
    Operator("Unsqueeze"), attr_axes(p_attr_axes) {};
  ~Unsqueeze() {};

  AttrType.INTS attr_axes();

  // TODO: setter
  T input_data();

  T output_expanded();

 private:
  AttrType.INTS attr_axes;
};

// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class Xor: public Operator {
 public:
  Xor(AttrType.INT p_attr_axis, AttrType.INT p_attr_broadcast):
    Operator("Xor"), attr_axis(p_attr_axis), attr_broadcast(p_attr_broadcast) {};
  ~Xor() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_broadcast();

  // TODO: setter
  T input_A();
  T input_B();

  T1 output_C();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_broadcast;
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
  Affine(AttrType.FLOAT p_attr_alpha, AttrType.FLOAT p_attr_beta):
    Operator("Affine"), attr_alpha(p_attr_alpha), attr_beta(p_attr_beta) {};
  ~Affine() {};

  AttrType.FLOAT attr_alpha();
  AttrType.FLOAT attr_beta();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
  AttrType.FLOAT attr_beta;
};

// TODO: Type Constraints
// T1: tensor(float), tensor(int32), tensor(int64), tensor(bool)
// T2: tensor(float), tensor(int32), tensor(int64), tensor(bool)
template<typename T1, typename T2>
class ConstantFill: public Operator {
 public:
  ConstantFill(AttrType.INT p_attr_dtype, AttrType.INTS p_attr_extra_shape, AttrType.INT p_attr_input_as_shape, AttrType.INTS p_attr_shape, AttrType.FLOAT p_attr_value):
    Operator("ConstantFill"), attr_dtype(p_attr_dtype), attr_extra_shape(p_attr_extra_shape), attr_input_as_shape(p_attr_input_as_shape), attr_shape(p_attr_shape), attr_value(p_attr_value) {};
  ~ConstantFill() {};

  AttrType.INT attr_dtype();
  AttrType.INTS attr_extra_shape();
  AttrType.INT attr_input_as_shape();
  AttrType.INTS attr_shape();
  AttrType.FLOAT attr_value();

  // TODO: setter
  std::optional<T1> input_input();

  T2 output_output();

 private:
  AttrType.INT attr_dtype;
  AttrType.INTS attr_extra_shape;
  AttrType.INT attr_input_as_shape;
  AttrType.INTS attr_shape;
  AttrType.FLOAT attr_value;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Crop: public Operator {
 public:
  Crop(AttrType.INTS p_attr_border, AttrType.INTS p_attr_scale):
    Operator("Crop"), attr_border(p_attr_border), attr_scale(p_attr_scale) {};
  ~Crop() {};

  AttrType.INTS attr_border();
  AttrType.INTS attr_scale();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INTS attr_border;
  AttrType.INTS attr_scale;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class FC: public Operator {
 public:
  FC(AttrType.INT p_attr_axis, AttrType.INT p_attr_axis_w):
    Operator("FC"), attr_axis(p_attr_axis), attr_axis_w(p_attr_axis_w) {};
  ~FC() {};

  AttrType.INT attr_axis();
  AttrType.INT attr_axis_w();

  // TODO: setter
  T input_X();
  T input_W();
  T input_B();

  T output_Y();

 private:
  AttrType.INT attr_axis;
  AttrType.INT attr_axis_w;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GRUUnit: public Operator {
 public:
  GRUUnit(AttrType.INT p_attr_drop_states):
    Operator("GRUUnit"), attr_drop_states(p_attr_drop_states) {};
  ~GRUUnit() {};

  AttrType.INT attr_drop_states();

  // TODO: setter
  T input_hidden_prev();
  T input_gates();
  T input_seq_lengths();
  T input_t();

  T output_hidden();

 private:
  AttrType.INT attr_drop_states;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GivenTensorFill: public Operator {
 public:
  GivenTensorFill(AttrType.INTS p_attr_extra_shape, AttrType.INT p_attr_input_as_shape, AttrType.INTS p_attr_shape, AttrType.FLOATS p_attr_values):
    Operator("GivenTensorFill"), attr_extra_shape(p_attr_extra_shape), attr_input_as_shape(p_attr_input_as_shape), attr_shape(p_attr_shape), attr_values(p_attr_values) {};
  ~GivenTensorFill() {};

  AttrType.INTS attr_extra_shape();
  AttrType.INT attr_input_as_shape();
  AttrType.INTS attr_shape();
  AttrType.FLOATS attr_values();

  // TODO: setter
  std::optional<T> input_shape();

  T output_X();

 private:
  AttrType.INTS attr_extra_shape;
  AttrType.INT attr_input_as_shape;
  AttrType.INTS attr_shape;
  AttrType.FLOATS attr_values;
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
  If(AttrType.GRAPH p_attr_else_branch, AttrType.GRAPH p_attr_then_branch):
    Operator("If"), attr_else_branch(p_attr_else_branch), attr_then_branch(p_attr_then_branch) {};
  ~If() {};

  AttrType.GRAPH attr_else_branch();
  AttrType.GRAPH attr_then_branch();

  // TODO: setter
  B input_cond();

  std::list<V> output_outputs();

 private:
  AttrType.GRAPH attr_else_branch;
  AttrType.GRAPH attr_then_branch;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ImageScaler: public Operator {
 public:
  ImageScaler(AttrType.FLOATS p_attr_bias, AttrType.FLOAT p_attr_scale):
    Operator("ImageScaler"), attr_bias(p_attr_bias), attr_scale(p_attr_scale) {};
  ~ImageScaler() {};

  AttrType.FLOATS attr_bias();
  AttrType.FLOAT attr_scale();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.FLOATS attr_bias;
  AttrType.FLOAT attr_scale;
};

// TODO: Type Constraints
// V: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// I: int64
// B: bool
template<typename V, typename I, typename B>
class Loop: public Operator {
 public:
  Loop(AttrType.GRAPH p_attr_body):
    Operator("Loop"), attr_body(p_attr_body) {};
  ~Loop() {};

  AttrType.GRAPH attr_body();

  // TODO: setter
  I input_M();
  B input_cond();
  std::list<V> input_v_initial();

  std::list<V> output_v_final_and_scan_outputs();

 private:
  AttrType.GRAPH attr_body;
};

// TODO: Type Constraints
// T: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// I: int32
template<typename T, typename I>
class LoopIndexTensor: public Operator {
 public:
  LoopIndexTensor(AttrType.INT p_attr_axis):
    Operator("LoopIndexTensor"), attr_axis(p_attr_axis) {};
  ~LoopIndexTensor() {};

  AttrType.INT attr_axis();

  // TODO: setter
  T input_T();
  I input_loop_idx();

  T output_O();

 private:
  AttrType.INT attr_axis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MeanVarianceNormalization: public Operator {
 public:
  MeanVarianceNormalization(AttrType.INT p_attr_across_channels, AttrType.INT p_attr_normalize_variance):
    Operator("MeanVarianceNormalization"), attr_across_channels(p_attr_across_channels), attr_normalize_variance(p_attr_normalize_variance) {};
  ~MeanVarianceNormalization() {};

  AttrType.INT attr_across_channels();
  AttrType.INT attr_normalize_variance();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.INT attr_across_channels;
  AttrType.INT attr_normalize_variance;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ParametricSoftplus: public Operator {
 public:
  ParametricSoftplus(AttrType.FLOAT p_attr_alpha, AttrType.FLOAT p_attr_beta):
    Operator("ParametricSoftplus"), attr_alpha(p_attr_alpha), attr_beta(p_attr_beta) {};
  ~ParametricSoftplus() {};

  AttrType.FLOAT attr_alpha();
  AttrType.FLOAT attr_beta();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
  AttrType.FLOAT attr_beta;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Scale: public Operator {
 public:
  Scale(AttrType.FLOAT p_attr_scale):
    Operator("Scale"), attr_scale(p_attr_scale) {};
  ~Scale() {};

  AttrType.FLOAT attr_scale();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.FLOAT attr_scale;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ScaledTanh: public Operator {
 public:
  ScaledTanh(AttrType.FLOAT p_attr_alpha, AttrType.FLOAT p_attr_beta):
    Operator("ScaledTanh"), attr_alpha(p_attr_alpha), attr_beta(p_attr_beta) {};
  ~ScaledTanh() {};

  AttrType.FLOAT attr_alpha();
  AttrType.FLOAT attr_beta();

  // TODO: setter
  T input_input();

  T output_output();

 private:
  AttrType.FLOAT attr_alpha;
  AttrType.FLOAT attr_beta;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ThresholdedRelu: public Operator {
 public:
  ThresholdedRelu(AttrType.FLOAT p_attr_alpha):
    Operator("ThresholdedRelu"), attr_alpha(p_attr_alpha) {};
  ~ThresholdedRelu() {};

  AttrType.FLOAT attr_alpha();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_alpha;
};

// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64), tensor(float16), tensor(float), tensor(double)
template<typename T>
class Upsample: public Operator {
 public:
  Upsample(AttrType.FLOAT p_attr_height_scale, AttrType.STRING p_attr_mode, AttrType.FLOAT p_attr_width_scale):
    Operator("Upsample"), attr_height_scale(p_attr_height_scale), attr_mode(p_attr_mode), attr_width_scale(p_attr_width_scale) {};
  ~Upsample() {};

  AttrType.FLOAT attr_height_scale();
  AttrType.STRING attr_mode();
  AttrType.FLOAT attr_width_scale();

  // TODO: setter
  T input_X();

  T output_Y();

 private:
  AttrType.FLOAT attr_height_scale;
  AttrType.STRING attr_mode;
  AttrType.FLOAT attr_width_scale;
};

