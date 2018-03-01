// Auto-generated
#include <list>
#include <vector>
#include <cinttypes>
#include <experimental/optional>
#include "Operator.h"

namespace onnc {
namespace tensor {

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Abs: public Operator {
 public:
  Abs():
    Operator("Abs") {};
  ~Abs() {};


  // TODO: setter
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Add: public Operator {
 public:
  Add(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Add"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Add() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
};

// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class And: public Operator {
 public:
  And(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("And"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~And() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T1 OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ArgMax: public Operator {
 public:
  ArgMax(AttrType::INT AttrAxis_, AttrType::INT AttrKeepdims_):
    Operator("ArgMax"), AttrAxis(AttrAxis_), AttrKeepdims(AttrKeepdims_) {};
  ~ArgMax() {};

  AttrType::INT getAxis();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  Tensor<int32_t> OutputReduced();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ArgMin: public Operator {
 public:
  ArgMin(AttrType::INT AttrAxis_, AttrType::INT AttrKeepdims_):
    Operator("ArgMin"), AttrAxis(AttrAxis_), AttrKeepdims(AttrKeepdims_) {};
  ~ArgMin() {};

  AttrType::INT getAxis();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  Tensor<int32_t> OutputReduced();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class AveragePool: public Operator {
 public:
  AveragePool(AttrType::STRING AttrAutoPad_, AttrType::INTS AttrKernelShape_, AttrType::INTS AttrPads_, AttrType::INTS AttrStrides_):
    Operator("AveragePool"), AttrAutoPad(AttrAutoPad_), AttrKernelShape(AttrKernelShape_), AttrPads(AttrPads_), AttrStrides(AttrStrides_) {};
  ~AveragePool() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::STRING AttrAutoPad;
  AttrType::INTS AttrKernelShape;
  AttrType::INTS AttrPads;
  AttrType::INTS AttrStrides;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class BatchNormalization: public Operator {
 public:
  BatchNormalization(AttrType::FLOAT AttrEpsilon_, AttrType::INT AttrIsTest_, AttrType::FLOAT AttrMomentum_, AttrType::INT AttrSpatial_):
    Operator("BatchNormalization"), AttrEpsilon(AttrEpsilon_), AttrIsTest(AttrIsTest_), AttrMomentum(AttrMomentum_), AttrSpatial(AttrSpatial_) {};
  ~BatchNormalization() {};

  AttrType::FLOAT getEpsilon();
  AttrType::INT getIsTest();
  AttrType::FLOAT getMomentum();
  AttrType::INT getSpatial();

  // TODO: setter
  T InputX();
  T InputScale();
  T InputB();
  T InputMean();
  T InputVar();

  T OutputY();
  std::experimental::optional<T> OutputMean();
  std::experimental::optional<T> OutputVar();
  std::experimental::optional<T> OutputSavedMean();
  std::experimental::optional<T> OutputSavedVar();

 private:
  AttrType::FLOAT AttrEpsilon;
  AttrType::INT AttrIsTest;
  AttrType::FLOAT AttrMomentum;
  AttrType::INT AttrSpatial;
};

// TODO: Type Constraints
// T1: tensor(float16), tensor(float), tensor(double)
// T2: tensor(float16), tensor(float), tensor(double)
template<typename T1, typename T2>
class Cast: public Operator {
 public:
  Cast(AttrType::STRING AttrTo_):
    Operator("Cast"), AttrTo(AttrTo_) {};
  ~Cast() {};

  AttrType::STRING getTo();

  // TODO: setter
  T1 InputInput();

  T2 OutputOutput();

 private:
  AttrType::STRING AttrTo;
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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Clip: public Operator {
 public:
  Clip(AttrType::FLOAT AttrMax_, AttrType::FLOAT AttrMin_):
    Operator("Clip"), AttrMax(AttrMax_), AttrMin(AttrMin_) {};
  ~Clip() {};

  AttrType::FLOAT getMax();
  AttrType::FLOAT getMin();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::FLOAT AttrMax;
  AttrType::FLOAT AttrMin;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Concat: public Operator {
 public:
  Concat(AttrType::INT AttrAxis_):
    Operator("Concat"), AttrAxis(AttrAxis_) {};
  ~Concat() {};

  AttrType::INT getAxis();

  // TODO: setter
  std::list<T> InputInputs();

  T OutputConcatResult();

 private:
  AttrType::INT AttrAxis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Constant: public Operator {
 public:
  Constant(AttrType::TENSOR AttrValue_):
    Operator("Constant"), AttrValue(AttrValue_) {};
  ~Constant() {};

  AttrType::TENSOR getValue();

  // TODO: setter

  T OutputOutput();

 private:
  AttrType::TENSOR AttrValue;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Conv: public Operator {
 public:
  Conv(AttrType::STRING AttrAutoPad_, AttrType::INTS AttrDilations_, AttrType::INT AttrGroup_, AttrType::INTS AttrKernelShape_, AttrType::INTS AttrPads_, AttrType::INTS AttrStrides_):
    Operator("Conv"), AttrAutoPad(AttrAutoPad_), AttrDilations(AttrDilations_), AttrGroup(AttrGroup_), AttrKernelShape(AttrKernelShape_), AttrPads(AttrPads_), AttrStrides(AttrStrides_) {};
  ~Conv() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getDilations();
  AttrType::INT getGroup();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  T InputX();
  T InputW();
  std::experimental::optional<T> InputB();

  T OutputY();

 private:
  AttrType::STRING AttrAutoPad;
  AttrType::INTS AttrDilations;
  AttrType::INT AttrGroup;
  AttrType::INTS AttrKernelShape;
  AttrType::INTS AttrPads;
  AttrType::INTS AttrStrides;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ConvTranspose: public Operator {
 public:
  ConvTranspose(AttrType::STRING AttrAutoPad_, AttrType::INTS AttrDilations_, AttrType::INT AttrGroup_, AttrType::INTS AttrKernelShape_, AttrType::INTS AttrOutputPadding_, AttrType::INTS AttrOutputShape_, AttrType::INTS AttrPads_, AttrType::INTS AttrStrides_):
    Operator("ConvTranspose"), AttrAutoPad(AttrAutoPad_), AttrDilations(AttrDilations_), AttrGroup(AttrGroup_), AttrKernelShape(AttrKernelShape_), AttrOutputPadding(AttrOutputPadding_), AttrOutputShape(AttrOutputShape_), AttrPads(AttrPads_), AttrStrides(AttrStrides_) {};
  ~ConvTranspose() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getDilations();
  AttrType::INT getGroup();
  AttrType::INTS getKernelShape();
  AttrType::INTS getOutputPadding();
  AttrType::INTS getOutputShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  T InputX();
  T InputW();
  std::experimental::optional<T> InputB();

  T OutputY();

 private:
  AttrType::STRING AttrAutoPad;
  AttrType::INTS AttrDilations;
  AttrType::INT AttrGroup;
  AttrType::INTS AttrKernelShape;
  AttrType::INTS AttrOutputPadding;
  AttrType::INTS AttrOutputShape;
  AttrType::INTS AttrPads;
  AttrType::INTS AttrStrides;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class DepthToSpace: public Operator {
 public:
  DepthToSpace(AttrType::INT AttrBlocksize_):
    Operator("DepthToSpace"), AttrBlocksize(AttrBlocksize_) {};
  ~DepthToSpace() {};

  AttrType::INT getBlocksize();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrBlocksize;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Div: public Operator {
 public:
  Div(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Div"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Div() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Dropout: public Operator {
 public:
  Dropout(AttrType::INT AttrIsTest_, AttrType::FLOAT AttrRatio_):
    Operator("Dropout"), AttrIsTest(AttrIsTest_), AttrRatio(AttrRatio_) {};
  ~Dropout() {};

  AttrType::INT getIsTest();
  AttrType::FLOAT getRatio();

  // TODO: setter
  T InputData();

  T OutputOutput();
  std::experimental::optional<T> OutputMask();

 private:
  AttrType::INT AttrIsTest;
  AttrType::FLOAT AttrRatio;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Elu: public Operator {
 public:
  Elu(AttrType::FLOAT AttrAlpha_):
    Operator("Elu"), AttrAlpha(AttrAlpha_) {};
  ~Elu() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
};

// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64)
// T1: tensor(bool)
template<typename T, typename T1>
class Equal: public Operator {
 public:
  Equal(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Equal"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Equal() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T1 OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
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
  T InputInput();

  T OutputOutput();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Flatten: public Operator {
 public:
  Flatten(AttrType::INT AttrAxis_):
    Operator("Flatten"), AttrAxis(AttrAxis_) {};
  ~Flatten() {};

  AttrType::INT getAxis();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrAxis;
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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class GRU: public Operator {
 public:
  GRU(AttrType::FLOATS AttrActivationAlpha_, AttrType::FLOATS AttrActivationBeta_, AttrType::STRINGS AttrActivations_, AttrType::FLOAT AttrClip_, AttrType::STRING AttrDirection_, AttrType::INT AttrHiddenSize_, AttrType::INT AttrLinearBeforeReset_, AttrType::INT AttrOutputSequence_):
    Operator("GRU"), AttrActivationAlpha(AttrActivationAlpha_), AttrActivationBeta(AttrActivationBeta_), AttrActivations(AttrActivations_), AttrClip(AttrClip_), AttrDirection(AttrDirection_), AttrHiddenSize(AttrHiddenSize_), AttrLinearBeforeReset(AttrLinearBeforeReset_), AttrOutputSequence(AttrOutputSequence_) {};
  ~GRU() {};

  AttrType::FLOATS getActivationAlpha();
  AttrType::FLOATS getActivationBeta();
  AttrType::STRINGS getActivations();
  AttrType::FLOAT getClip();
  AttrType::STRING getDirection();
  AttrType::INT getHiddenSize();
  AttrType::INT getLinearBeforeReset();
  AttrType::INT getOutputSequence();

  // TODO: setter
  T InputX();
  T InputW();
  T InputR();
  std::experimental::optional<T> InputB();
  std::experimental::optional<T1> InputSequenceLens();
  std::experimental::optional<T> InputInitialH();

  std::experimental::optional<T> OutputY();
  std::experimental::optional<T> OutputYH();

 private:
  AttrType::FLOATS AttrActivationAlpha;
  AttrType::FLOATS AttrActivationBeta;
  AttrType::STRINGS AttrActivations;
  AttrType::FLOAT AttrClip;
  AttrType::STRING AttrDirection;
  AttrType::INT AttrHiddenSize;
  AttrType::INT AttrLinearBeforeReset;
  AttrType::INT AttrOutputSequence;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// Tind: tensor(int32), tensor(int64)
template<typename T, typename Tind>
class Gather: public Operator {
 public:
  Gather(AttrType::INT AttrAxis_):
    Operator("Gather"), AttrAxis(AttrAxis_) {};
  ~Gather() {};

  AttrType::INT getAxis();

  // TODO: setter
  T InputData();
  Tind InputIndices();

  T OutputOutput();

 private:
  AttrType::INT AttrAxis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Gemm: public Operator {
 public:
  Gemm(AttrType::FLOAT AttrAlpha_, AttrType::FLOAT AttrBeta_, AttrType::INT AttrBroadcast_, AttrType::INT AttrTransa_, AttrType::INT AttrTransb_):
    Operator("Gemm"), AttrAlpha(AttrAlpha_), AttrBeta(AttrBeta_), AttrBroadcast(AttrBroadcast_), AttrTransa(AttrTransa_), AttrTransb(AttrTransb_) {};
  ~Gemm() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();
  AttrType::INT getBroadcast();
  AttrType::INT getTransa();
  AttrType::INT getTransb();

  // TODO: setter
  T InputA();
  T InputB();
  T InputC();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
  AttrType::FLOAT AttrBeta;
  AttrType::INT AttrBroadcast;
  AttrType::INT AttrTransa;
  AttrType::INT AttrTransb;
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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GlobalLpPool: public Operator {
 public:
  GlobalLpPool(AttrType::INT AttrP_):
    Operator("GlobalLpPool"), AttrP(AttrP_) {};
  ~GlobalLpPool() {};

  AttrType::INT getP();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::INT AttrP;
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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(bool)
template<typename T, typename T1>
class Greater: public Operator {
 public:
  Greater(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Greater"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Greater() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T1 OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class HardSigmoid: public Operator {
 public:
  HardSigmoid(AttrType::FLOAT AttrAlpha_, AttrType::FLOAT AttrBeta_):
    Operator("HardSigmoid"), AttrAlpha(AttrAlpha_), AttrBeta(AttrBeta_) {};
  ~HardSigmoid() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
  AttrType::FLOAT AttrBeta;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Hardmax: public Operator {
 public:
  Hardmax(AttrType::INT AttrAxis_):
    Operator("Hardmax"), AttrAxis(AttrAxis_) {};
  ~Hardmax() {};

  AttrType::INT getAxis();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrAxis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class InstanceNormalization: public Operator {
 public:
  InstanceNormalization(AttrType::FLOAT AttrEpsilon_):
    Operator("InstanceNormalization"), AttrEpsilon(AttrEpsilon_) {};
  ~InstanceNormalization() {};

  AttrType::FLOAT getEpsilon();

  // TODO: setter
  T InputInput();
  T InputScale();
  T InputB();

  T OutputOutput();

 private:
  AttrType::FLOAT AttrEpsilon;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LRN: public Operator {
 public:
  LRN(AttrType::FLOAT AttrAlpha_, AttrType::FLOAT AttrBeta_, AttrType::FLOAT AttrBias_, AttrType::INT AttrSize_):
    Operator("LRN"), AttrAlpha(AttrAlpha_), AttrBeta(AttrBeta_), AttrBias(AttrBias_), AttrSize(AttrSize_) {};
  ~LRN() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();
  AttrType::FLOAT getBias();
  AttrType::INT getSize();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
  AttrType::FLOAT AttrBeta;
  AttrType::FLOAT AttrBias;
  AttrType::INT AttrSize;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class LSTM: public Operator {
 public:
  LSTM(AttrType::FLOATS AttrActivationAlpha_, AttrType::FLOATS AttrActivationBeta_, AttrType::STRINGS AttrActivations_, AttrType::FLOAT AttrClip_, AttrType::STRING AttrDirection_, AttrType::INT AttrHiddenSize_, AttrType::INT AttrInputForget_, AttrType::INT AttrOutputSequence_):
    Operator("LSTM"), AttrActivationAlpha(AttrActivationAlpha_), AttrActivationBeta(AttrActivationBeta_), AttrActivations(AttrActivations_), AttrClip(AttrClip_), AttrDirection(AttrDirection_), AttrHiddenSize(AttrHiddenSize_), AttrInputForget(AttrInputForget_), AttrOutputSequence(AttrOutputSequence_) {};
  ~LSTM() {};

  AttrType::FLOATS getActivationAlpha();
  AttrType::FLOATS getActivationBeta();
  AttrType::STRINGS getActivations();
  AttrType::FLOAT getClip();
  AttrType::STRING getDirection();
  AttrType::INT getHiddenSize();
  AttrType::INT getInputForget();
  AttrType::INT getOutputSequence();

  // TODO: setter
  T InputX();
  T InputW();
  T InputR();
  std::experimental::optional<T> InputB();
  std::experimental::optional<T1> InputSequenceLens();
  std::experimental::optional<T> InputInitialH();
  std::experimental::optional<T> InputInitialC();
  std::experimental::optional<T> InputP();

  std::experimental::optional<T> OutputY();
  std::experimental::optional<T> OutputYH();
  std::experimental::optional<T> OutputYC();

 private:
  AttrType::FLOATS AttrActivationAlpha;
  AttrType::FLOATS AttrActivationBeta;
  AttrType::STRINGS AttrActivations;
  AttrType::FLOAT AttrClip;
  AttrType::STRING AttrDirection;
  AttrType::INT AttrHiddenSize;
  AttrType::INT AttrInputForget;
  AttrType::INT AttrOutputSequence;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LeakyRelu: public Operator {
 public:
  LeakyRelu(AttrType::FLOAT AttrAlpha_):
    Operator("LeakyRelu"), AttrAlpha(AttrAlpha_) {};
  ~LeakyRelu() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(bool)
template<typename T, typename T1>
class Less: public Operator {
 public:
  Less(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Less"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Less() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T1 OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
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
  T InputInput();

  T OutputOutput();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LogSoftmax: public Operator {
 public:
  LogSoftmax(AttrType::INT AttrAxis_):
    Operator("LogSoftmax"), AttrAxis(AttrAxis_) {};
  ~LogSoftmax() {};

  AttrType::INT getAxis();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrAxis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LpNormalization: public Operator {
 public:
  LpNormalization(AttrType::INT AttrAxis_, AttrType::INT AttrP_):
    Operator("LpNormalization"), AttrAxis(AttrAxis_), AttrP(AttrP_) {};
  ~LpNormalization() {};

  AttrType::INT getAxis();
  AttrType::INT getP();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrP;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LpPool: public Operator {
 public:
  LpPool(AttrType::STRING AttrAutoPad_, AttrType::INTS AttrKernelShape_, AttrType::INT AttrP_, AttrType::INTS AttrPads_, AttrType::INTS AttrStrides_):
    Operator("LpPool"), AttrAutoPad(AttrAutoPad_), AttrKernelShape(AttrKernelShape_), AttrP(AttrP_), AttrPads(AttrPads_), AttrStrides(AttrStrides_) {};
  ~LpPool() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INT getP();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::STRING AttrAutoPad;
  AttrType::INTS AttrKernelShape;
  AttrType::INT AttrP;
  AttrType::INTS AttrPads;
  AttrType::INTS AttrStrides;
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
  T InputA();
  T InputB();

  T OutputY();

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
  std::list<T> InputData0();

  T OutputMax();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MaxPool: public Operator {
 public:
  MaxPool(AttrType::STRING AttrAutoPad_, AttrType::INTS AttrKernelShape_, AttrType::INTS AttrPads_, AttrType::INTS AttrStrides_):
    Operator("MaxPool"), AttrAutoPad(AttrAutoPad_), AttrKernelShape(AttrKernelShape_), AttrPads(AttrPads_), AttrStrides(AttrStrides_) {};
  ~MaxPool() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::STRING AttrAutoPad;
  AttrType::INTS AttrKernelShape;
  AttrType::INTS AttrPads;
  AttrType::INTS AttrStrides;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MaxRoiPool: public Operator {
 public:
  MaxRoiPool(AttrType::INTS AttrPooledShape_, AttrType::FLOAT AttrSpatialScale_):
    Operator("MaxRoiPool"), AttrPooledShape(AttrPooledShape_), AttrSpatialScale(AttrSpatialScale_) {};
  ~MaxRoiPool() {};

  AttrType::INTS getPooledShape();
  AttrType::FLOAT getSpatialScale();

  // TODO: setter
  T InputX();
  T InputRois();

  T OutputY();

 private:
  AttrType::INTS AttrPooledShape;
  AttrType::FLOAT AttrSpatialScale;
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
  std::list<T> InputData0();

  T OutputMean();

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
  std::list<T> InputData0();

  T OutputMin();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Mul: public Operator {
 public:
  Mul(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Mul"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Mul() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
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
  T InputX();

  T OutputY();

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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class Or: public Operator {
 public:
  Or(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Or"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Or() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T1 OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
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
  T InputX();
  T InputSlope();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Pad: public Operator {
 public:
  Pad(AttrType::STRING AttrMode_, AttrType::INTS AttrPads_, AttrType::FLOAT AttrValue_):
    Operator("Pad"), AttrMode(AttrMode_), AttrPads(AttrPads_), AttrValue(AttrValue_) {};
  ~Pad() {};

  AttrType::STRING getMode();
  AttrType::INTS getPads();
  AttrType::FLOAT getValue();

  // TODO: setter
  T InputData();

  T OutputOutput();

 private:
  AttrType::STRING AttrMode;
  AttrType::INTS AttrPads;
  AttrType::FLOAT AttrValue;
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
  T InputX();
  T InputY();

  T OutputZ();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class RNN: public Operator {
 public:
  RNN(AttrType::FLOATS AttrActivationAlpha_, AttrType::FLOATS AttrActivationBeta_, AttrType::STRINGS AttrActivations_, AttrType::FLOAT AttrClip_, AttrType::STRING AttrDirection_, AttrType::INT AttrHiddenSize_, AttrType::INT AttrOutputSequence_):
    Operator("RNN"), AttrActivationAlpha(AttrActivationAlpha_), AttrActivationBeta(AttrActivationBeta_), AttrActivations(AttrActivations_), AttrClip(AttrClip_), AttrDirection(AttrDirection_), AttrHiddenSize(AttrHiddenSize_), AttrOutputSequence(AttrOutputSequence_) {};
  ~RNN() {};

  AttrType::FLOATS getActivationAlpha();
  AttrType::FLOATS getActivationBeta();
  AttrType::STRINGS getActivations();
  AttrType::FLOAT getClip();
  AttrType::STRING getDirection();
  AttrType::INT getHiddenSize();
  AttrType::INT getOutputSequence();

  // TODO: setter
  T InputX();
  T InputW();
  T InputR();
  std::experimental::optional<T> InputB();
  std::experimental::optional<T1> InputSequenceLens();
  std::experimental::optional<T> InputInitialH();

  std::experimental::optional<T> OutputY();
  std::experimental::optional<T> OutputYH();

 private:
  AttrType::FLOATS AttrActivationAlpha;
  AttrType::FLOATS AttrActivationBeta;
  AttrType::STRINGS AttrActivations;
  AttrType::FLOAT AttrClip;
  AttrType::STRING AttrDirection;
  AttrType::INT AttrHiddenSize;
  AttrType::INT AttrOutputSequence;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomNormal: public Operator {
 public:
  RandomNormal(AttrType::INT AttrDtype_, AttrType::FLOAT AttrMean_, AttrType::FLOAT AttrScale_, AttrType::FLOAT AttrSeed_, AttrType::INTS AttrShape_):
    Operator("RandomNormal"), AttrDtype(AttrDtype_), AttrMean(AttrMean_), AttrScale(AttrScale_), AttrSeed(AttrSeed_), AttrShape(AttrShape_) {};
  ~RandomNormal() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getMean();
  AttrType::FLOAT getScale();
  AttrType::FLOAT getSeed();
  AttrType::INTS getShape();

  // TODO: setter

  T OutputOutput();

 private:
  AttrType::INT AttrDtype;
  AttrType::FLOAT AttrMean;
  AttrType::FLOAT AttrScale;
  AttrType::FLOAT AttrSeed;
  AttrType::INTS AttrShape;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomNormalLike: public Operator {
 public:
  RandomNormalLike(AttrType::INT AttrDtype_, AttrType::FLOAT AttrMean_, AttrType::FLOAT AttrScale_, AttrType::FLOAT AttrSeed_):
    Operator("RandomNormalLike"), AttrDtype(AttrDtype_), AttrMean(AttrMean_), AttrScale(AttrScale_), AttrSeed(AttrSeed_) {};
  ~RandomNormalLike() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getMean();
  AttrType::FLOAT getScale();
  AttrType::FLOAT getSeed();

  // TODO: setter
  Tensor<int32_t> InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrDtype;
  AttrType::FLOAT AttrMean;
  AttrType::FLOAT AttrScale;
  AttrType::FLOAT AttrSeed;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomUniform: public Operator {
 public:
  RandomUniform(AttrType::INT AttrDtype_, AttrType::FLOAT AttrHigh_, AttrType::FLOAT AttrLow_, AttrType::FLOAT AttrSeed_, AttrType::INTS AttrShape_):
    Operator("RandomUniform"), AttrDtype(AttrDtype_), AttrHigh(AttrHigh_), AttrLow(AttrLow_), AttrSeed(AttrSeed_), AttrShape(AttrShape_) {};
  ~RandomUniform() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getHigh();
  AttrType::FLOAT getLow();
  AttrType::FLOAT getSeed();
  AttrType::INTS getShape();

  // TODO: setter

  T OutputOutput();

 private:
  AttrType::INT AttrDtype;
  AttrType::FLOAT AttrHigh;
  AttrType::FLOAT AttrLow;
  AttrType::FLOAT AttrSeed;
  AttrType::INTS AttrShape;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomUniformLike: public Operator {
 public:
  RandomUniformLike(AttrType::INT AttrDtype_, AttrType::FLOAT AttrHigh_, AttrType::FLOAT AttrLow_, AttrType::FLOAT AttrSeed_):
    Operator("RandomUniformLike"), AttrDtype(AttrDtype_), AttrHigh(AttrHigh_), AttrLow(AttrLow_), AttrSeed(AttrSeed_) {};
  ~RandomUniformLike() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getHigh();
  AttrType::FLOAT getLow();
  AttrType::FLOAT getSeed();

  // TODO: setter
  Tensor<int32_t> InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrDtype;
  AttrType::FLOAT AttrHigh;
  AttrType::FLOAT AttrLow;
  AttrType::FLOAT AttrSeed;
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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceL1: public Operator {
 public:
  ReduceL1(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceL1"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceL1() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceL2: public Operator {
 public:
  ReduceL2(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceL2"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceL2() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceLogSum: public Operator {
 public:
  ReduceLogSum(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceLogSum"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceLogSum() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceLogSumExp: public Operator {
 public:
  ReduceLogSumExp(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceLogSumExp"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceLogSumExp() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMax: public Operator {
 public:
  ReduceMax(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceMax"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceMax() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMean: public Operator {
 public:
  ReduceMean(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceMean"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceMean() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMin: public Operator {
 public:
  ReduceMin(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceMin"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceMin() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceProd: public Operator {
 public:
  ReduceProd(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceProd"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceProd() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceSum: public Operator {
 public:
  ReduceSum(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceSum"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceSum() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceSumSquare: public Operator {
 public:
  ReduceSumSquare(AttrType::INTS AttrAxes_, AttrType::INT AttrKeepdims_):
    Operator("ReduceSumSquare"), AttrAxes(AttrAxes_), AttrKeepdims(AttrKeepdims_) {};
  ~ReduceSumSquare() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  T InputData();

  T OutputReduced();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INT AttrKeepdims;
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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Reshape: public Operator {
 public:
  Reshape(AttrType::INTS AttrShape_):
    Operator("Reshape"), AttrShape(AttrShape_) {};
  ~Reshape() {};

  AttrType::INTS getShape();

  // TODO: setter
  T InputData();

  T OutputReshaped();

 private:
  AttrType::INTS AttrShape;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Selu: public Operator {
 public:
  Selu(AttrType::FLOAT AttrAlpha_, AttrType::FLOAT AttrGamma_):
    Operator("Selu"), AttrAlpha(AttrAlpha_), AttrGamma(AttrGamma_) {};
  ~Selu() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getGamma();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
  AttrType::FLOAT AttrGamma;
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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Slice: public Operator {
 public:
  Slice(AttrType::INTS AttrAxes_, AttrType::INTS AttrEnds_, AttrType::INTS AttrStarts_):
    Operator("Slice"), AttrAxes(AttrAxes_), AttrEnds(AttrEnds_), AttrStarts(AttrStarts_) {};
  ~Slice() {};

  AttrType::INTS getAxes();
  AttrType::INTS getEnds();
  AttrType::INTS getStarts();

  // TODO: setter
  T InputData();

  T OutputOutput();

 private:
  AttrType::INTS AttrAxes;
  AttrType::INTS AttrEnds;
  AttrType::INTS AttrStarts;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Softmax: public Operator {
 public:
  Softmax(AttrType::INT AttrAxis_):
    Operator("Softmax"), AttrAxis(AttrAxis_) {};
  ~Softmax() {};

  AttrType::INT getAxis();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrAxis;
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
  T InputX();

  T OutputY();

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
  T InputInput();

  T OutputOutput();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SpaceToDepth: public Operator {
 public:
  SpaceToDepth(AttrType::INT AttrBlocksize_):
    Operator("SpaceToDepth"), AttrBlocksize(AttrBlocksize_) {};
  ~SpaceToDepth() {};

  AttrType::INT getBlocksize();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrBlocksize;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Split: public Operator {
 public:
  Split(AttrType::INT AttrAxis_, AttrType::INTS AttrSplit_):
    Operator("Split"), AttrAxis(AttrAxis_), AttrSplit(AttrSplit_) {};
  ~Split() {};

  AttrType::INT getAxis();
  AttrType::INTS getSplit();

  // TODO: setter
  T InputInput();

  std::list<T> OutputOutputs();

 private:
  AttrType::INT AttrAxis;
  AttrType::INTS AttrSplit;
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
  T InputX();

  T OutputY();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Squeeze: public Operator {
 public:
  Squeeze(AttrType::INTS AttrAxes_):
    Operator("Squeeze"), AttrAxes(AttrAxes_) {};
  ~Squeeze() {};

  AttrType::INTS getAxes();

  // TODO: setter
  T InputData();

  T OutputSqueezed();

 private:
  AttrType::INTS AttrAxes;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Sub: public Operator {
 public:
  Sub(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Sub"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Sub() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
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
  std::list<T> InputData0();

  T OutputSum();

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
  T InputInput();

  T OutputOutput();

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
  T InputInput();
  T InputTiles();
  T InputAxis();

  T OutputOutput();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Transpose: public Operator {
 public:
  Transpose(AttrType::INTS AttrPerm_):
    Operator("Transpose"), AttrPerm(AttrPerm_) {};
  ~Transpose() {};

  AttrType::INTS getPerm();

  // TODO: setter
  T InputData();

  T OutputTransposed();

 private:
  AttrType::INTS AttrPerm;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Unsqueeze: public Operator {
 public:
  Unsqueeze(AttrType::INTS AttrAxes_):
    Operator("Unsqueeze"), AttrAxes(AttrAxes_) {};
  ~Unsqueeze() {};

  AttrType::INTS getAxes();

  // TODO: setter
  T InputData();

  T OutputExpanded();

 private:
  AttrType::INTS AttrAxes;
};

// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class Xor: public Operator {
 public:
  Xor(AttrType::INT AttrAxis_, AttrType::INT AttrBroadcast_):
    Operator("Xor"), AttrAxis(AttrAxis_), AttrBroadcast(AttrBroadcast_) {};
  ~Xor() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  T InputA();
  T InputB();

  T1 OutputC();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrBroadcast;
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
  std::list<T> InputInput();

  std::list<T> OutputOutput();

 private:
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Affine: public Operator {
 public:
  Affine(AttrType::FLOAT AttrAlpha_, AttrType::FLOAT AttrBeta_):
    Operator("Affine"), AttrAlpha(AttrAlpha_), AttrBeta(AttrBeta_) {};
  ~Affine() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
  AttrType::FLOAT AttrBeta;
};

// TODO: Type Constraints
// T1: tensor(float), tensor(int32), tensor(int64), tensor(bool)
// T2: tensor(float), tensor(int32), tensor(int64), tensor(bool)
template<typename T1, typename T2>
class ConstantFill: public Operator {
 public:
  ConstantFill(AttrType::INT AttrDtype_, AttrType::INTS AttrExtraShape_, AttrType::INT AttrInputAsShape_, AttrType::INTS AttrShape_, AttrType::FLOAT AttrValue_):
    Operator("ConstantFill"), AttrDtype(AttrDtype_), AttrExtraShape(AttrExtraShape_), AttrInputAsShape(AttrInputAsShape_), AttrShape(AttrShape_), AttrValue(AttrValue_) {};
  ~ConstantFill() {};

  AttrType::INT getDtype();
  AttrType::INTS getExtraShape();
  AttrType::INT getInputAsShape();
  AttrType::INTS getShape();
  AttrType::FLOAT getValue();

  // TODO: setter
  std::experimental::optional<T1> InputInput();

  T2 OutputOutput();

 private:
  AttrType::INT AttrDtype;
  AttrType::INTS AttrExtraShape;
  AttrType::INT AttrInputAsShape;
  AttrType::INTS AttrShape;
  AttrType::FLOAT AttrValue;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Crop: public Operator {
 public:
  Crop(AttrType::INTS AttrBorder_, AttrType::INTS AttrScale_):
    Operator("Crop"), AttrBorder(AttrBorder_), AttrScale(AttrScale_) {};
  ~Crop() {};

  AttrType::INTS getBorder();
  AttrType::INTS getScale();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INTS AttrBorder;
  AttrType::INTS AttrScale;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class FC: public Operator {
 public:
  FC(AttrType::INT AttrAxis_, AttrType::INT AttrAxisW_):
    Operator("FC"), AttrAxis(AttrAxis_), AttrAxisW(AttrAxisW_) {};
  ~FC() {};

  AttrType::INT getAxis();
  AttrType::INT getAxisW();

  // TODO: setter
  T InputX();
  T InputW();
  T InputB();

  T OutputY();

 private:
  AttrType::INT AttrAxis;
  AttrType::INT AttrAxisW;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GRUUnit: public Operator {
 public:
  GRUUnit(AttrType::INT AttrDropStates_):
    Operator("GRUUnit"), AttrDropStates(AttrDropStates_) {};
  ~GRUUnit() {};

  AttrType::INT getDropStates();

  // TODO: setter
  T InputHiddenPrev();
  T InputGates();
  T InputSeqLengths();
  T InputT();

  T OutputHidden();

 private:
  AttrType::INT AttrDropStates;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GivenTensorFill: public Operator {
 public:
  GivenTensorFill(AttrType::INTS AttrExtraShape_, AttrType::INT AttrInputAsShape_, AttrType::INTS AttrShape_, AttrType::FLOATS AttrValues_):
    Operator("GivenTensorFill"), AttrExtraShape(AttrExtraShape_), AttrInputAsShape(AttrInputAsShape_), AttrShape(AttrShape_), AttrValues(AttrValues_) {};
  ~GivenTensorFill() {};

  AttrType::INTS getExtraShape();
  AttrType::INT getInputAsShape();
  AttrType::INTS getShape();
  AttrType::FLOATS getValues();

  // TODO: setter
  std::experimental::optional<T> InputShape();

  T OutputX();

 private:
  AttrType::INTS AttrExtraShape;
  AttrType::INT AttrInputAsShape;
  AttrType::INTS AttrShape;
  AttrType::FLOATS AttrValues;
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
  T InputInput();

  T OutputOutput();

 private:
};

// TODO: Type Constraints
// V: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// B: tensor(bool)
template<typename V, typename B>
class If: public Operator {
 public:
  If(AttrType::GRAPH AttrElseBranch_, AttrType::GRAPH AttrThenBranch_):
    Operator("If"), AttrElseBranch(AttrElseBranch_), AttrThenBranch(AttrThenBranch_) {};
  ~If() {};

  AttrType::GRAPH getElseBranch();
  AttrType::GRAPH getThenBranch();

  // TODO: setter
  B InputCond();

  std::list<V> OutputOutputs();

 private:
  AttrType::GRAPH AttrElseBranch;
  AttrType::GRAPH AttrThenBranch;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ImageScaler: public Operator {
 public:
  ImageScaler(AttrType::FLOATS AttrBias_, AttrType::FLOAT AttrScale_):
    Operator("ImageScaler"), AttrBias(AttrBias_), AttrScale(AttrScale_) {};
  ~ImageScaler() {};

  AttrType::FLOATS getBias();
  AttrType::FLOAT getScale();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::FLOATS AttrBias;
  AttrType::FLOAT AttrScale;
};

// TODO: Type Constraints
// V: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// I: int64
// B: bool
template<typename V, typename I, typename B>
class Loop: public Operator {
 public:
  Loop(AttrType::GRAPH AttrBody_):
    Operator("Loop"), AttrBody(AttrBody_) {};
  ~Loop() {};

  AttrType::GRAPH getBody();

  // TODO: setter
  I InputM();
  B InputCond();
  std::list<V> InputVInitial();

  std::list<V> OutputVFinalAndScanOutputs();

 private:
  AttrType::GRAPH AttrBody;
};

// TODO: Type Constraints
// T: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// I: int32
template<typename T, typename I>
class LoopIndexTensor: public Operator {
 public:
  LoopIndexTensor(AttrType::INT AttrAxis_):
    Operator("LoopIndexTensor"), AttrAxis(AttrAxis_) {};
  ~LoopIndexTensor() {};

  AttrType::INT getAxis();

  // TODO: setter
  T InputT();
  I InputLoopIdx();

  T OutputO();

 private:
  AttrType::INT AttrAxis;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MeanVarianceNormalization: public Operator {
 public:
  MeanVarianceNormalization(AttrType::INT AttrAcrossChannels_, AttrType::INT AttrNormalizeVariance_):
    Operator("MeanVarianceNormalization"), AttrAcrossChannels(AttrAcrossChannels_), AttrNormalizeVariance(AttrNormalizeVariance_) {};
  ~MeanVarianceNormalization() {};

  AttrType::INT getAcrossChannels();
  AttrType::INT getNormalizeVariance();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::INT AttrAcrossChannels;
  AttrType::INT AttrNormalizeVariance;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ParametricSoftplus: public Operator {
 public:
  ParametricSoftplus(AttrType::FLOAT AttrAlpha_, AttrType::FLOAT AttrBeta_):
    Operator("ParametricSoftplus"), AttrAlpha(AttrAlpha_), AttrBeta(AttrBeta_) {};
  ~ParametricSoftplus() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
  AttrType::FLOAT AttrBeta;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class Scale: public Operator {
 public:
  Scale(AttrType::FLOAT AttrScale_):
    Operator("Scale"), AttrScale(AttrScale_) {};
  ~Scale() {};

  AttrType::FLOAT getScale();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::FLOAT AttrScale;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ScaledTanh: public Operator {
 public:
  ScaledTanh(AttrType::FLOAT AttrAlpha_, AttrType::FLOAT AttrBeta_):
    Operator("ScaledTanh"), AttrAlpha(AttrAlpha_), AttrBeta(AttrBeta_) {};
  ~ScaledTanh() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  T InputInput();

  T OutputOutput();

 private:
  AttrType::FLOAT AttrAlpha;
  AttrType::FLOAT AttrBeta;
};

// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ThresholdedRelu: public Operator {
 public:
  ThresholdedRelu(AttrType::FLOAT AttrAlpha_):
    Operator("ThresholdedRelu"), AttrAlpha(AttrAlpha_) {};
  ~ThresholdedRelu() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrAlpha;
};

// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64), tensor(float16), tensor(float), tensor(double)
template<typename T>
class Upsample: public Operator {
 public:
  Upsample(AttrType::FLOAT AttrHeightScale_, AttrType::STRING AttrMode_, AttrType::FLOAT AttrWidthScale_):
    Operator("Upsample"), AttrHeightScale(AttrHeightScale_), AttrMode(AttrMode_), AttrWidthScale(AttrWidthScale_) {};
  ~Upsample() {};

  AttrType::FLOAT getHeightScale();
  AttrType::STRING getMode();
  AttrType::FLOAT getWidthScale();

  // TODO: setter
  T InputX();

  T OutputY();

 private:
  AttrType::FLOAT AttrHeightScale;
  AttrType::STRING AttrMode;
  AttrType::FLOAT AttrWidthScale;
};


} // namespace tensor
} // namespace onnc

