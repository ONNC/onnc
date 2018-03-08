// Auto-generated
#include <list>
#include <vector>
#include <cinttypes>
#include <experimental/optional>
#include "Operator.h"

namespace onnc {
namespace tensor {

class Abs: public Operator {
protected:
  Abs():
    Operator("Abs") {};
public:
  ~Abs() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class AbsT: public Abs {
public:
  AbsT():
    Abs() {};
  ~AbsT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Add: public Operator {
protected:
  Add(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Add"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Add() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class AddT: public Add {
public:
  AddT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Add(pAttrAxis, pAttrBroadcast) {};
  ~AddT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class And: public Operator {
protected:
  And(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("And"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~And() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class AndT: public And {
public:
  AndT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    And(pAttrAxis, pAttrBroadcast) {};
  ~AndT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T1> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class ArgMax: public Operator {
protected:
  ArgMax(AttrType::INT pAttrAxis, AttrType::INT pAttrKeepdims):
    Operator("ArgMax"), m_AttrAxis(pAttrAxis), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ArgMax() {};

  AttrType::INT getAxis();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ArgMaxT: public ArgMax {
public:
  ArgMaxT(AttrType::INT pAttrAxis, AttrType::INT pAttrKeepdims):
    ArgMax(pAttrAxis, pAttrKeepdims) {};
  ~ArgMaxT() {};

  AttrType::INT getAxis();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<int32_t> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ArgMin: public Operator {
protected:
  ArgMin(AttrType::INT pAttrAxis, AttrType::INT pAttrKeepdims):
    Operator("ArgMin"), m_AttrAxis(pAttrAxis), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ArgMin() {};

  AttrType::INT getAxis();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ArgMinT: public ArgMin {
public:
  ArgMinT(AttrType::INT pAttrAxis, AttrType::INT pAttrKeepdims):
    ArgMin(pAttrAxis, pAttrKeepdims) {};
  ~ArgMinT() {};

  AttrType::INT getAxis();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<int32_t> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class AveragePool: public Operator {
protected:
  AveragePool(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrKernelShape, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    Operator("AveragePool"), m_AttrAutoPad(pAttrAutoPad), m_AttrKernelShape(pAttrKernelShape), m_AttrPads(pAttrPads), m_AttrStrides(pAttrStrides) {};
public:
  ~AveragePool() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::STRING m_AttrAutoPad;
  AttrType::INTS m_AttrKernelShape;
  AttrType::INTS m_AttrPads;
  AttrType::INTS m_AttrStrides;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class AveragePoolT: public AveragePool {
public:
  AveragePoolT(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrKernelShape, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    AveragePool(pAttrAutoPad, pAttrKernelShape, pAttrPads, pAttrStrides) {};
  ~AveragePoolT() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class BatchNormalization: public Operator {
protected:
  BatchNormalization(AttrType::FLOAT pAttrEpsilon, AttrType::INT pAttrIsTest, AttrType::FLOAT pAttrMomentum, AttrType::INT pAttrSpatial):
    Operator("BatchNormalization"), m_AttrEpsilon(pAttrEpsilon), m_AttrIsTest(pAttrIsTest), m_AttrMomentum(pAttrMomentum), m_AttrSpatial(pAttrSpatial) {};
public:
  ~BatchNormalization() {};

  AttrType::FLOAT getEpsilon();
  AttrType::INT getIsTest();
  AttrType::FLOAT getMomentum();
  AttrType::INT getSpatial();

  // TODO: setter
  Tensor *InputX();
  Tensor *InputScale();
  Tensor *InputB();
  Tensor *InputMean();
  Tensor *InputVar();

  Tensor *OutputY();
  std::experimental::optional<Tensor *> OutputMean();
  std::experimental::optional<Tensor *> OutputVar();
  std::experimental::optional<Tensor *> OutputSavedMean();
  std::experimental::optional<Tensor *> OutputSavedVar();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrEpsilon;
  AttrType::INT m_AttrIsTest;
  AttrType::FLOAT m_AttrMomentum;
  AttrType::INT m_AttrSpatial;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class BatchNormalizationT: public BatchNormalization {
public:
  BatchNormalizationT(AttrType::FLOAT pAttrEpsilon, AttrType::INT pAttrIsTest, AttrType::FLOAT pAttrMomentum, AttrType::INT pAttrSpatial):
    BatchNormalization(pAttrEpsilon, pAttrIsTest, pAttrMomentum, pAttrSpatial) {};
  ~BatchNormalizationT() {};

  AttrType::FLOAT getEpsilon();
  AttrType::INT getIsTest();
  AttrType::FLOAT getMomentum();
  AttrType::INT getSpatial();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputScale();
  TensorT<T> *InputB();
  TensorT<T> *InputMean();
  TensorT<T> *InputVar();

  TensorT<T> *OutputY();
  std::experimental::optional<TensorT<T> *> OutputMean();
  std::experimental::optional<TensorT<T> *> OutputVar();
  std::experimental::optional<TensorT<T> *> OutputSavedMean();
  std::experimental::optional<TensorT<T> *> OutputSavedVar();

  void accept(OperatorVisitor& pV) override;
};

class Cast: public Operator {
protected:
  Cast(AttrType::STRING pAttrTo):
    Operator("Cast"), m_AttrTo(pAttrTo) {};
public:
  ~Cast() {};

  AttrType::STRING getTo();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::STRING m_AttrTo;
};
// TODO: Type Constraints
// T1: tensor(float16), tensor(float), tensor(double), tensor(int8), tensor(int16), tensor(int32), tensor(int64), tensor(uint8), tensor(uint16), tensor(uint32), tensor(uint64), tensor(bool)
// T2: tensor(float16), tensor(float), tensor(double), tensor(int8), tensor(int16), tensor(int32), tensor(int64), tensor(uint8), tensor(uint16), tensor(uint32), tensor(uint64), tensor(bool)
template<typename T1, typename T2>
class CastT: public Cast {
public:
  CastT(AttrType::STRING pAttrTo):
    Cast(pAttrTo) {};
  ~CastT() {};

  AttrType::STRING getTo();

  // TODO: setter
  TensorT<T1> *InputInput();

  TensorT<T2> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Ceil: public Operator {
protected:
  Ceil():
    Operator("Ceil") {};
public:
  ~Ceil() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class CeilT: public Ceil {
public:
  CeilT():
    Ceil() {};
  ~CeilT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Clip: public Operator {
protected:
  Clip(AttrType::FLOAT pAttrMax, AttrType::FLOAT pAttrMin):
    Operator("Clip"), m_AttrMax(pAttrMax), m_AttrMin(pAttrMin) {};
public:
  ~Clip() {};

  AttrType::FLOAT getMax();
  AttrType::FLOAT getMin();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrMax;
  AttrType::FLOAT m_AttrMin;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ClipT: public Clip {
public:
  ClipT(AttrType::FLOAT pAttrMax, AttrType::FLOAT pAttrMin):
    Clip(pAttrMax, pAttrMin) {};
  ~ClipT() {};

  AttrType::FLOAT getMax();
  AttrType::FLOAT getMin();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Concat: public Operator {
protected:
  Concat(AttrType::INT pAttrAxis):
    Operator("Concat"), m_AttrAxis(pAttrAxis) {};
public:
  ~Concat() {};

  AttrType::INT getAxis();

  // TODO: setter
  std::list<Tensor *> InputInputs();

  Tensor *OutputConcatResult();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ConcatT: public Concat {
public:
  ConcatT(AttrType::INT pAttrAxis):
    Concat(pAttrAxis) {};
  ~ConcatT() {};

  AttrType::INT getAxis();

  // TODO: setter
  std::list<TensorT<T> *> InputInputs();

  TensorT<T> *OutputConcatResult();

  void accept(OperatorVisitor& pV) override;
};

class Constant: public Operator {
protected:
  Constant(AttrType::TENSOR pAttrValue):
    Operator("Constant"), m_AttrValue(pAttrValue) {};
public:
  ~Constant() {};

  AttrType::TENSOR getValue();

  // TODO: setter

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::TENSOR m_AttrValue;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ConstantT: public Constant {
public:
  ConstantT(AttrType::TENSOR pAttrValue):
    Constant(pAttrValue) {};
  ~ConstantT() {};

  AttrType::TENSOR getValue();

  // TODO: setter

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Conv: public Operator {
protected:
  Conv(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrDilations, AttrType::INT pAttrGroup, AttrType::INTS pAttrKernelShape, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    Operator("Conv"), m_AttrAutoPad(pAttrAutoPad), m_AttrDilations(pAttrDilations), m_AttrGroup(pAttrGroup), m_AttrKernelShape(pAttrKernelShape), m_AttrPads(pAttrPads), m_AttrStrides(pAttrStrides) {};
public:
  ~Conv() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getDilations();
  AttrType::INT getGroup();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  Tensor *InputX();
  Tensor *InputW();
  std::experimental::optional<Tensor *> InputB();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::STRING m_AttrAutoPad;
  AttrType::INTS m_AttrDilations;
  AttrType::INT m_AttrGroup;
  AttrType::INTS m_AttrKernelShape;
  AttrType::INTS m_AttrPads;
  AttrType::INTS m_AttrStrides;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ConvT: public Conv {
public:
  ConvT(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrDilations, AttrType::INT pAttrGroup, AttrType::INTS pAttrKernelShape, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    Conv(pAttrAutoPad, pAttrDilations, pAttrGroup, pAttrKernelShape, pAttrPads, pAttrStrides) {};
  ~ConvT() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getDilations();
  AttrType::INT getGroup();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputW();
  std::experimental::optional<TensorT<T> *> InputB();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class ConvTranspose: public Operator {
protected:
  ConvTranspose(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrDilations, AttrType::INT pAttrGroup, AttrType::INTS pAttrKernelShape, AttrType::INTS pAttrOutputPadding, AttrType::INTS pAttrOutputShape, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    Operator("ConvTranspose"), m_AttrAutoPad(pAttrAutoPad), m_AttrDilations(pAttrDilations), m_AttrGroup(pAttrGroup), m_AttrKernelShape(pAttrKernelShape), m_AttrOutputPadding(pAttrOutputPadding), m_AttrOutputShape(pAttrOutputShape), m_AttrPads(pAttrPads), m_AttrStrides(pAttrStrides) {};
public:
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
  Tensor *InputX();
  Tensor *InputW();
  std::experimental::optional<Tensor *> InputB();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::STRING m_AttrAutoPad;
  AttrType::INTS m_AttrDilations;
  AttrType::INT m_AttrGroup;
  AttrType::INTS m_AttrKernelShape;
  AttrType::INTS m_AttrOutputPadding;
  AttrType::INTS m_AttrOutputShape;
  AttrType::INTS m_AttrPads;
  AttrType::INTS m_AttrStrides;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ConvTransposeT: public ConvTranspose {
public:
  ConvTransposeT(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrDilations, AttrType::INT pAttrGroup, AttrType::INTS pAttrKernelShape, AttrType::INTS pAttrOutputPadding, AttrType::INTS pAttrOutputShape, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    ConvTranspose(pAttrAutoPad, pAttrDilations, pAttrGroup, pAttrKernelShape, pAttrOutputPadding, pAttrOutputShape, pAttrPads, pAttrStrides) {};
  ~ConvTransposeT() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getDilations();
  AttrType::INT getGroup();
  AttrType::INTS getKernelShape();
  AttrType::INTS getOutputPadding();
  AttrType::INTS getOutputShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputW();
  std::experimental::optional<TensorT<T> *> InputB();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class DepthToSpace: public Operator {
protected:
  DepthToSpace(AttrType::INT pAttrBlocksize):
    Operator("DepthToSpace"), m_AttrBlocksize(pAttrBlocksize) {};
public:
  ~DepthToSpace() {};

  AttrType::INT getBlocksize();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrBlocksize;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class DepthToSpaceT: public DepthToSpace {
public:
  DepthToSpaceT(AttrType::INT pAttrBlocksize):
    DepthToSpace(pAttrBlocksize) {};
  ~DepthToSpaceT() {};

  AttrType::INT getBlocksize();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Div: public Operator {
protected:
  Div(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Div"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Div() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class DivT: public Div {
public:
  DivT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Div(pAttrAxis, pAttrBroadcast) {};
  ~DivT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class Dropout: public Operator {
protected:
  Dropout(AttrType::INT pAttrIsTest, AttrType::FLOAT pAttrRatio):
    Operator("Dropout"), m_AttrIsTest(pAttrIsTest), m_AttrRatio(pAttrRatio) {};
public:
  ~Dropout() {};

  AttrType::INT getIsTest();
  AttrType::FLOAT getRatio();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputOutput();
  std::experimental::optional<Tensor *> OutputMask();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrIsTest;
  AttrType::FLOAT m_AttrRatio;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class DropoutT: public Dropout {
public:
  DropoutT(AttrType::INT pAttrIsTest, AttrType::FLOAT pAttrRatio):
    Dropout(pAttrIsTest, pAttrRatio) {};
  ~DropoutT() {};

  AttrType::INT getIsTest();
  AttrType::FLOAT getRatio();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputOutput();
  std::experimental::optional<TensorT<T> *> OutputMask();

  void accept(OperatorVisitor& pV) override;
};

class Elu: public Operator {
protected:
  Elu(AttrType::FLOAT pAttrAlpha):
    Operator("Elu"), m_AttrAlpha(pAttrAlpha) {};
public:
  ~Elu() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class EluT: public Elu {
public:
  EluT(AttrType::FLOAT pAttrAlpha):
    Elu(pAttrAlpha) {};
  ~EluT() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Equal: public Operator {
protected:
  Equal(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Equal"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Equal() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64)
// T1: tensor(bool)
template<typename T, typename T1>
class EqualT: public Equal {
public:
  EqualT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Equal(pAttrAxis, pAttrBroadcast) {};
  ~EqualT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T1> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class Exp: public Operator {
protected:
  Exp():
    Operator("Exp") {};
public:
  ~Exp() {};

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ExpT: public Exp {
public:
  ExpT():
    Exp() {};
  ~ExpT() {};

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Flatten: public Operator {
protected:
  Flatten(AttrType::INT pAttrAxis):
    Operator("Flatten"), m_AttrAxis(pAttrAxis) {};
public:
  ~Flatten() {};

  AttrType::INT getAxis();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class FlattenT: public Flatten {
public:
  FlattenT(AttrType::INT pAttrAxis):
    Flatten(pAttrAxis) {};
  ~FlattenT() {};

  AttrType::INT getAxis();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Floor: public Operator {
protected:
  Floor():
    Operator("Floor") {};
public:
  ~Floor() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class FloorT: public Floor {
public:
  FloorT():
    Floor() {};
  ~FloorT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class GRU: public Operator {
protected:
  GRU(AttrType::FLOATS pAttrActivationAlpha, AttrType::FLOATS pAttrActivationBeta, AttrType::STRINGS pAttrActivations, AttrType::FLOAT pAttrClip, AttrType::STRING pAttrDirection, AttrType::INT pAttrHiddenSize, AttrType::INT pAttrLinearBeforeReset, AttrType::INT pAttrOutputSequence):
    Operator("GRU"), m_AttrActivationAlpha(pAttrActivationAlpha), m_AttrActivationBeta(pAttrActivationBeta), m_AttrActivations(pAttrActivations), m_AttrClip(pAttrClip), m_AttrDirection(pAttrDirection), m_AttrHiddenSize(pAttrHiddenSize), m_AttrLinearBeforeReset(pAttrLinearBeforeReset), m_AttrOutputSequence(pAttrOutputSequence) {};
public:
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
  Tensor *InputX();
  Tensor *InputW();
  Tensor *InputR();
  std::experimental::optional<Tensor *> InputB();
  std::experimental::optional<Tensor *> InputSequenceLens();
  std::experimental::optional<Tensor *> InputInitialH();

  std::experimental::optional<Tensor *> OutputY();
  std::experimental::optional<Tensor *> OutputYH();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOATS m_AttrActivationAlpha;
  AttrType::FLOATS m_AttrActivationBeta;
  AttrType::STRINGS m_AttrActivations;
  AttrType::FLOAT m_AttrClip;
  AttrType::STRING m_AttrDirection;
  AttrType::INT m_AttrHiddenSize;
  AttrType::INT m_AttrLinearBeforeReset;
  AttrType::INT m_AttrOutputSequence;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class GRUT: public GRU {
public:
  GRUT(AttrType::FLOATS pAttrActivationAlpha, AttrType::FLOATS pAttrActivationBeta, AttrType::STRINGS pAttrActivations, AttrType::FLOAT pAttrClip, AttrType::STRING pAttrDirection, AttrType::INT pAttrHiddenSize, AttrType::INT pAttrLinearBeforeReset, AttrType::INT pAttrOutputSequence):
    GRU(pAttrActivationAlpha, pAttrActivationBeta, pAttrActivations, pAttrClip, pAttrDirection, pAttrHiddenSize, pAttrLinearBeforeReset, pAttrOutputSequence) {};
  ~GRUT() {};

  AttrType::FLOATS getActivationAlpha();
  AttrType::FLOATS getActivationBeta();
  AttrType::STRINGS getActivations();
  AttrType::FLOAT getClip();
  AttrType::STRING getDirection();
  AttrType::INT getHiddenSize();
  AttrType::INT getLinearBeforeReset();
  AttrType::INT getOutputSequence();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputW();
  TensorT<T> *InputR();
  std::experimental::optional<TensorT<T> *> InputB();
  std::experimental::optional<TensorT<T1> *> InputSequenceLens();
  std::experimental::optional<TensorT<T> *> InputInitialH();

  std::experimental::optional<TensorT<T> *> OutputY();
  std::experimental::optional<TensorT<T> *> OutputYH();

  void accept(OperatorVisitor& pV) override;
};

class Gather: public Operator {
protected:
  Gather(AttrType::INT pAttrAxis):
    Operator("Gather"), m_AttrAxis(pAttrAxis) {};
public:
  ~Gather() {};

  AttrType::INT getAxis();

  // TODO: setter
  Tensor *InputData();
  Tensor *InputIndices();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// Tind: tensor(int32), tensor(int64)
template<typename T, typename Tind>
class GatherT: public Gather {
public:
  GatherT(AttrType::INT pAttrAxis):
    Gather(pAttrAxis) {};
  ~GatherT() {};

  AttrType::INT getAxis();

  // TODO: setter
  TensorT<T> *InputData();
  TensorT<Tind> *InputIndices();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Gemm: public Operator {
protected:
  Gemm(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta, AttrType::INT pAttrBroadcast, AttrType::INT pAttrTransa, AttrType::INT pAttrTransb):
    Operator("Gemm"), m_AttrAlpha(pAttrAlpha), m_AttrBeta(pAttrBeta), m_AttrBroadcast(pAttrBroadcast), m_AttrTransa(pAttrTransa), m_AttrTransb(pAttrTransb) {};
public:
  ~Gemm() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();
  AttrType::INT getBroadcast();
  AttrType::INT getTransa();
  AttrType::INT getTransb();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();
  Tensor *InputC();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
  AttrType::FLOAT m_AttrBeta;
  AttrType::INT m_AttrBroadcast;
  AttrType::INT m_AttrTransa;
  AttrType::INT m_AttrTransb;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GemmT: public Gemm {
public:
  GemmT(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta, AttrType::INT pAttrBroadcast, AttrType::INT pAttrTransa, AttrType::INT pAttrTransb):
    Gemm(pAttrAlpha, pAttrBeta, pAttrBroadcast, pAttrTransa, pAttrTransb) {};
  ~GemmT() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();
  AttrType::INT getBroadcast();
  AttrType::INT getTransa();
  AttrType::INT getTransb();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();
  TensorT<T> *InputC();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class GlobalAveragePool: public Operator {
protected:
  GlobalAveragePool():
    Operator("GlobalAveragePool") {};
public:
  ~GlobalAveragePool() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GlobalAveragePoolT: public GlobalAveragePool {
public:
  GlobalAveragePoolT():
    GlobalAveragePool() {};
  ~GlobalAveragePoolT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class GlobalLpPool: public Operator {
protected:
  GlobalLpPool(AttrType::INT pAttrP):
    Operator("GlobalLpPool"), m_AttrP(pAttrP) {};
public:
  ~GlobalLpPool() {};

  AttrType::INT getP();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrP;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GlobalLpPoolT: public GlobalLpPool {
public:
  GlobalLpPoolT(AttrType::INT pAttrP):
    GlobalLpPool(pAttrP) {};
  ~GlobalLpPoolT() {};

  AttrType::INT getP();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class GlobalMaxPool: public Operator {
protected:
  GlobalMaxPool():
    Operator("GlobalMaxPool") {};
public:
  ~GlobalMaxPool() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GlobalMaxPoolT: public GlobalMaxPool {
public:
  GlobalMaxPoolT():
    GlobalMaxPool() {};
  ~GlobalMaxPoolT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Greater: public Operator {
protected:
  Greater(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Greater"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Greater() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(bool)
template<typename T, typename T1>
class GreaterT: public Greater {
public:
  GreaterT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Greater(pAttrAxis, pAttrBroadcast) {};
  ~GreaterT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T1> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class HardSigmoid: public Operator {
protected:
  HardSigmoid(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta):
    Operator("HardSigmoid"), m_AttrAlpha(pAttrAlpha), m_AttrBeta(pAttrBeta) {};
public:
  ~HardSigmoid() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
  AttrType::FLOAT m_AttrBeta;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class HardSigmoidT: public HardSigmoid {
public:
  HardSigmoidT(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta):
    HardSigmoid(pAttrAlpha, pAttrBeta) {};
  ~HardSigmoidT() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Hardmax: public Operator {
protected:
  Hardmax(AttrType::INT pAttrAxis):
    Operator("Hardmax"), m_AttrAxis(pAttrAxis) {};
public:
  ~Hardmax() {};

  AttrType::INT getAxis();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class HardmaxT: public Hardmax {
public:
  HardmaxT(AttrType::INT pAttrAxis):
    Hardmax(pAttrAxis) {};
  ~HardmaxT() {};

  AttrType::INT getAxis();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class InstanceNormalization: public Operator {
protected:
  InstanceNormalization(AttrType::FLOAT pAttrEpsilon):
    Operator("InstanceNormalization"), m_AttrEpsilon(pAttrEpsilon) {};
public:
  ~InstanceNormalization() {};

  AttrType::FLOAT getEpsilon();

  // TODO: setter
  Tensor *InputInput();
  Tensor *InputScale();
  Tensor *InputB();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrEpsilon;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class InstanceNormalizationT: public InstanceNormalization {
public:
  InstanceNormalizationT(AttrType::FLOAT pAttrEpsilon):
    InstanceNormalization(pAttrEpsilon) {};
  ~InstanceNormalizationT() {};

  AttrType::FLOAT getEpsilon();

  // TODO: setter
  TensorT<T> *InputInput();
  TensorT<T> *InputScale();
  TensorT<T> *InputB();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class LRN: public Operator {
protected:
  LRN(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta, AttrType::FLOAT pAttrBias, AttrType::INT pAttrSize):
    Operator("LRN"), m_AttrAlpha(pAttrAlpha), m_AttrBeta(pAttrBeta), m_AttrBias(pAttrBias), m_AttrSize(pAttrSize) {};
public:
  ~LRN() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();
  AttrType::FLOAT getBias();
  AttrType::INT getSize();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
  AttrType::FLOAT m_AttrBeta;
  AttrType::FLOAT m_AttrBias;
  AttrType::INT m_AttrSize;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LRNT: public LRN {
public:
  LRNT(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta, AttrType::FLOAT pAttrBias, AttrType::INT pAttrSize):
    LRN(pAttrAlpha, pAttrBeta, pAttrBias, pAttrSize) {};
  ~LRNT() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();
  AttrType::FLOAT getBias();
  AttrType::INT getSize();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class LSTM: public Operator {
protected:
  LSTM(AttrType::FLOATS pAttrActivationAlpha, AttrType::FLOATS pAttrActivationBeta, AttrType::STRINGS pAttrActivations, AttrType::FLOAT pAttrClip, AttrType::STRING pAttrDirection, AttrType::INT pAttrHiddenSize, AttrType::INT pAttrInputForget, AttrType::INT pAttrOutputSequence):
    Operator("LSTM"), m_AttrActivationAlpha(pAttrActivationAlpha), m_AttrActivationBeta(pAttrActivationBeta), m_AttrActivations(pAttrActivations), m_AttrClip(pAttrClip), m_AttrDirection(pAttrDirection), m_AttrHiddenSize(pAttrHiddenSize), m_AttrInputForget(pAttrInputForget), m_AttrOutputSequence(pAttrOutputSequence) {};
public:
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
  Tensor *InputX();
  Tensor *InputW();
  Tensor *InputR();
  std::experimental::optional<Tensor *> InputB();
  std::experimental::optional<Tensor *> InputSequenceLens();
  std::experimental::optional<Tensor *> InputInitialH();
  std::experimental::optional<Tensor *> InputInitialC();
  std::experimental::optional<Tensor *> InputP();

  std::experimental::optional<Tensor *> OutputY();
  std::experimental::optional<Tensor *> OutputYH();
  std::experimental::optional<Tensor *> OutputYC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOATS m_AttrActivationAlpha;
  AttrType::FLOATS m_AttrActivationBeta;
  AttrType::STRINGS m_AttrActivations;
  AttrType::FLOAT m_AttrClip;
  AttrType::STRING m_AttrDirection;
  AttrType::INT m_AttrHiddenSize;
  AttrType::INT m_AttrInputForget;
  AttrType::INT m_AttrOutputSequence;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class LSTMT: public LSTM {
public:
  LSTMT(AttrType::FLOATS pAttrActivationAlpha, AttrType::FLOATS pAttrActivationBeta, AttrType::STRINGS pAttrActivations, AttrType::FLOAT pAttrClip, AttrType::STRING pAttrDirection, AttrType::INT pAttrHiddenSize, AttrType::INT pAttrInputForget, AttrType::INT pAttrOutputSequence):
    LSTM(pAttrActivationAlpha, pAttrActivationBeta, pAttrActivations, pAttrClip, pAttrDirection, pAttrHiddenSize, pAttrInputForget, pAttrOutputSequence) {};
  ~LSTMT() {};

  AttrType::FLOATS getActivationAlpha();
  AttrType::FLOATS getActivationBeta();
  AttrType::STRINGS getActivations();
  AttrType::FLOAT getClip();
  AttrType::STRING getDirection();
  AttrType::INT getHiddenSize();
  AttrType::INT getInputForget();
  AttrType::INT getOutputSequence();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputW();
  TensorT<T> *InputR();
  std::experimental::optional<TensorT<T> *> InputB();
  std::experimental::optional<TensorT<T1> *> InputSequenceLens();
  std::experimental::optional<TensorT<T> *> InputInitialH();
  std::experimental::optional<TensorT<T> *> InputInitialC();
  std::experimental::optional<TensorT<T> *> InputP();

  std::experimental::optional<TensorT<T> *> OutputY();
  std::experimental::optional<TensorT<T> *> OutputYH();
  std::experimental::optional<TensorT<T> *> OutputYC();

  void accept(OperatorVisitor& pV) override;
};

class LeakyRelu: public Operator {
protected:
  LeakyRelu(AttrType::FLOAT pAttrAlpha):
    Operator("LeakyRelu"), m_AttrAlpha(pAttrAlpha) {};
public:
  ~LeakyRelu() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LeakyReluT: public LeakyRelu {
public:
  LeakyReluT(AttrType::FLOAT pAttrAlpha):
    LeakyRelu(pAttrAlpha) {};
  ~LeakyReluT() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Less: public Operator {
protected:
  Less(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Less"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Less() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(bool)
template<typename T, typename T1>
class LessT: public Less {
public:
  LessT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Less(pAttrAxis, pAttrBroadcast) {};
  ~LessT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T1> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class Log: public Operator {
protected:
  Log():
    Operator("Log") {};
public:
  ~Log() {};

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LogT: public Log {
public:
  LogT():
    Log() {};
  ~LogT() {};

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class LogSoftmax: public Operator {
protected:
  LogSoftmax(AttrType::INT pAttrAxis):
    Operator("LogSoftmax"), m_AttrAxis(pAttrAxis) {};
public:
  ~LogSoftmax() {};

  AttrType::INT getAxis();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LogSoftmaxT: public LogSoftmax {
public:
  LogSoftmaxT(AttrType::INT pAttrAxis):
    LogSoftmax(pAttrAxis) {};
  ~LogSoftmaxT() {};

  AttrType::INT getAxis();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class LpNormalization: public Operator {
protected:
  LpNormalization(AttrType::INT pAttrAxis, AttrType::INT pAttrP):
    Operator("LpNormalization"), m_AttrAxis(pAttrAxis), m_AttrP(pAttrP) {};
public:
  ~LpNormalization() {};

  AttrType::INT getAxis();
  AttrType::INT getP();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrP;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LpNormalizationT: public LpNormalization {
public:
  LpNormalizationT(AttrType::INT pAttrAxis, AttrType::INT pAttrP):
    LpNormalization(pAttrAxis, pAttrP) {};
  ~LpNormalizationT() {};

  AttrType::INT getAxis();
  AttrType::INT getP();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class LpPool: public Operator {
protected:
  LpPool(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrKernelShape, AttrType::INT pAttrP, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    Operator("LpPool"), m_AttrAutoPad(pAttrAutoPad), m_AttrKernelShape(pAttrKernelShape), m_AttrP(pAttrP), m_AttrPads(pAttrPads), m_AttrStrides(pAttrStrides) {};
public:
  ~LpPool() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INT getP();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::STRING m_AttrAutoPad;
  AttrType::INTS m_AttrKernelShape;
  AttrType::INT m_AttrP;
  AttrType::INTS m_AttrPads;
  AttrType::INTS m_AttrStrides;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class LpPoolT: public LpPool {
public:
  LpPoolT(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrKernelShape, AttrType::INT pAttrP, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    LpPool(pAttrAutoPad, pAttrKernelShape, pAttrP, pAttrPads, pAttrStrides) {};
  ~LpPoolT() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INT getP();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class MatMul: public Operator {
protected:
  MatMul():
    Operator("MatMul") {};
public:
  ~MatMul() {};

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MatMulT: public MatMul {
public:
  MatMulT():
    MatMul() {};
  ~MatMulT() {};

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Max: public Operator {
protected:
  Max():
    Operator("Max") {};
public:
  ~Max() {};

  // TODO: setter
  std::list<Tensor *> InputData0();

  Tensor *OutputMax();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MaxT: public Max {
public:
  MaxT():
    Max() {};
  ~MaxT() {};

  // TODO: setter
  std::list<TensorT<T> *> InputData0();

  TensorT<T> *OutputMax();

  void accept(OperatorVisitor& pV) override;
};

class MaxPool: public Operator {
protected:
  MaxPool(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrKernelShape, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    Operator("MaxPool"), m_AttrAutoPad(pAttrAutoPad), m_AttrKernelShape(pAttrKernelShape), m_AttrPads(pAttrPads), m_AttrStrides(pAttrStrides) {};
public:
  ~MaxPool() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::STRING m_AttrAutoPad;
  AttrType::INTS m_AttrKernelShape;
  AttrType::INTS m_AttrPads;
  AttrType::INTS m_AttrStrides;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MaxPoolT: public MaxPool {
public:
  MaxPoolT(AttrType::STRING pAttrAutoPad, AttrType::INTS pAttrKernelShape, AttrType::INTS pAttrPads, AttrType::INTS pAttrStrides):
    MaxPool(pAttrAutoPad, pAttrKernelShape, pAttrPads, pAttrStrides) {};
  ~MaxPoolT() {};

  AttrType::STRING getAutoPad();
  AttrType::INTS getKernelShape();
  AttrType::INTS getPads();
  AttrType::INTS getStrides();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class MaxRoiPool: public Operator {
protected:
  MaxRoiPool(AttrType::INTS pAttrPooledShape, AttrType::FLOAT pAttrSpatialScale):
    Operator("MaxRoiPool"), m_AttrPooledShape(pAttrPooledShape), m_AttrSpatialScale(pAttrSpatialScale) {};
public:
  ~MaxRoiPool() {};

  AttrType::INTS getPooledShape();
  AttrType::FLOAT getSpatialScale();

  // TODO: setter
  Tensor *InputX();
  Tensor *InputRois();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrPooledShape;
  AttrType::FLOAT m_AttrSpatialScale;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MaxRoiPoolT: public MaxRoiPool {
public:
  MaxRoiPoolT(AttrType::INTS pAttrPooledShape, AttrType::FLOAT pAttrSpatialScale):
    MaxRoiPool(pAttrPooledShape, pAttrSpatialScale) {};
  ~MaxRoiPoolT() {};

  AttrType::INTS getPooledShape();
  AttrType::FLOAT getSpatialScale();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputRois();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Mean: public Operator {
protected:
  Mean():
    Operator("Mean") {};
public:
  ~Mean() {};

  // TODO: setter
  std::list<Tensor *> InputData0();

  Tensor *OutputMean();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MeanT: public Mean {
public:
  MeanT():
    Mean() {};
  ~MeanT() {};

  // TODO: setter
  std::list<TensorT<T> *> InputData0();

  TensorT<T> *OutputMean();

  void accept(OperatorVisitor& pV) override;
};

class Min: public Operator {
protected:
  Min():
    Operator("Min") {};
public:
  ~Min() {};

  // TODO: setter
  std::list<Tensor *> InputData0();

  Tensor *OutputMin();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MinT: public Min {
public:
  MinT():
    Min() {};
  ~MinT() {};

  // TODO: setter
  std::list<TensorT<T> *> InputData0();

  TensorT<T> *OutputMin();

  void accept(OperatorVisitor& pV) override;
};

class Mul: public Operator {
protected:
  Mul(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Mul"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Mul() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MulT: public Mul {
public:
  MulT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Mul(pAttrAxis, pAttrBroadcast) {};
  ~MulT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class Neg: public Operator {
protected:
  Neg():
    Operator("Neg") {};
public:
  ~Neg() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class NegT: public Neg {
public:
  NegT():
    Neg() {};
  ~NegT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Not: public Operator {
protected:
  Not():
    Operator("Not") {};
public:
  ~Not() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(bool)
template<typename T>
class NotT: public Not {
public:
  NotT():
    Not() {};
  ~NotT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Or: public Operator {
protected:
  Or(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Or"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Or() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class OrT: public Or {
public:
  OrT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Or(pAttrAxis, pAttrBroadcast) {};
  ~OrT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T1> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class PRelu: public Operator {
protected:
  PRelu():
    Operator("PRelu") {};
public:
  ~PRelu() {};

  // TODO: setter
  Tensor *InputX();
  Tensor *InputSlope();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class PReluT: public PRelu {
public:
  PReluT():
    PRelu() {};
  ~PReluT() {};

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputSlope();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Pad: public Operator {
protected:
  Pad(AttrType::STRING pAttrMode, AttrType::INTS pAttrPads, AttrType::FLOAT pAttrValue):
    Operator("Pad"), m_AttrMode(pAttrMode), m_AttrPads(pAttrPads), m_AttrValue(pAttrValue) {};
public:
  ~Pad() {};

  AttrType::STRING getMode();
  AttrType::INTS getPads();
  AttrType::FLOAT getValue();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::STRING m_AttrMode;
  AttrType::INTS m_AttrPads;
  AttrType::FLOAT m_AttrValue;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class PadT: public Pad {
public:
  PadT(AttrType::STRING pAttrMode, AttrType::INTS pAttrPads, AttrType::FLOAT pAttrValue):
    Pad(pAttrMode, pAttrPads, pAttrValue) {};
  ~PadT() {};

  AttrType::STRING getMode();
  AttrType::INTS getPads();
  AttrType::FLOAT getValue();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Pow: public Operator {
protected:
  Pow(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Pow"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Pow() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputX();
  Tensor *InputY();

  Tensor *OutputZ();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class PowT: public Pow {
public:
  PowT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Pow(pAttrAxis, pAttrBroadcast) {};
  ~PowT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputY();

  TensorT<T> *OutputZ();

  void accept(OperatorVisitor& pV) override;
};

class RNN: public Operator {
protected:
  RNN(AttrType::FLOATS pAttrActivationAlpha, AttrType::FLOATS pAttrActivationBeta, AttrType::STRINGS pAttrActivations, AttrType::FLOAT pAttrClip, AttrType::STRING pAttrDirection, AttrType::INT pAttrHiddenSize, AttrType::INT pAttrOutputSequence):
    Operator("RNN"), m_AttrActivationAlpha(pAttrActivationAlpha), m_AttrActivationBeta(pAttrActivationBeta), m_AttrActivations(pAttrActivations), m_AttrClip(pAttrClip), m_AttrDirection(pAttrDirection), m_AttrHiddenSize(pAttrHiddenSize), m_AttrOutputSequence(pAttrOutputSequence) {};
public:
  ~RNN() {};

  AttrType::FLOATS getActivationAlpha();
  AttrType::FLOATS getActivationBeta();
  AttrType::STRINGS getActivations();
  AttrType::FLOAT getClip();
  AttrType::STRING getDirection();
  AttrType::INT getHiddenSize();
  AttrType::INT getOutputSequence();

  // TODO: setter
  Tensor *InputX();
  Tensor *InputW();
  Tensor *InputR();
  std::experimental::optional<Tensor *> InputB();
  std::experimental::optional<Tensor *> InputSequenceLens();
  std::experimental::optional<Tensor *> InputInitialH();

  std::experimental::optional<Tensor *> OutputY();
  std::experimental::optional<Tensor *> OutputYH();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOATS m_AttrActivationAlpha;
  AttrType::FLOATS m_AttrActivationBeta;
  AttrType::STRINGS m_AttrActivations;
  AttrType::FLOAT m_AttrClip;
  AttrType::STRING m_AttrDirection;
  AttrType::INT m_AttrHiddenSize;
  AttrType::INT m_AttrOutputSequence;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// T1: tensor(int32)
template<typename T, typename T1>
class RNNT: public RNN {
public:
  RNNT(AttrType::FLOATS pAttrActivationAlpha, AttrType::FLOATS pAttrActivationBeta, AttrType::STRINGS pAttrActivations, AttrType::FLOAT pAttrClip, AttrType::STRING pAttrDirection, AttrType::INT pAttrHiddenSize, AttrType::INT pAttrOutputSequence):
    RNN(pAttrActivationAlpha, pAttrActivationBeta, pAttrActivations, pAttrClip, pAttrDirection, pAttrHiddenSize, pAttrOutputSequence) {};
  ~RNNT() {};

  AttrType::FLOATS getActivationAlpha();
  AttrType::FLOATS getActivationBeta();
  AttrType::STRINGS getActivations();
  AttrType::FLOAT getClip();
  AttrType::STRING getDirection();
  AttrType::INT getHiddenSize();
  AttrType::INT getOutputSequence();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputW();
  TensorT<T> *InputR();
  std::experimental::optional<TensorT<T> *> InputB();
  std::experimental::optional<TensorT<T1> *> InputSequenceLens();
  std::experimental::optional<TensorT<T> *> InputInitialH();

  std::experimental::optional<TensorT<T> *> OutputY();
  std::experimental::optional<TensorT<T> *> OutputYH();

  void accept(OperatorVisitor& pV) override;
};

class RandomNormal: public Operator {
protected:
  RandomNormal(AttrType::INT pAttrDtype, AttrType::FLOAT pAttrMean, AttrType::FLOAT pAttrScale, AttrType::FLOAT pAttrSeed, AttrType::INTS pAttrShape):
    Operator("RandomNormal"), m_AttrDtype(pAttrDtype), m_AttrMean(pAttrMean), m_AttrScale(pAttrScale), m_AttrSeed(pAttrSeed), m_AttrShape(pAttrShape) {};
public:
  ~RandomNormal() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getMean();
  AttrType::FLOAT getScale();
  AttrType::FLOAT getSeed();
  AttrType::INTS getShape();

  // TODO: setter

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrDtype;
  AttrType::FLOAT m_AttrMean;
  AttrType::FLOAT m_AttrScale;
  AttrType::FLOAT m_AttrSeed;
  AttrType::INTS m_AttrShape;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomNormalT: public RandomNormal {
public:
  RandomNormalT(AttrType::INT pAttrDtype, AttrType::FLOAT pAttrMean, AttrType::FLOAT pAttrScale, AttrType::FLOAT pAttrSeed, AttrType::INTS pAttrShape):
    RandomNormal(pAttrDtype, pAttrMean, pAttrScale, pAttrSeed, pAttrShape) {};
  ~RandomNormalT() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getMean();
  AttrType::FLOAT getScale();
  AttrType::FLOAT getSeed();
  AttrType::INTS getShape();

  // TODO: setter

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class RandomNormalLike: public Operator {
protected:
  RandomNormalLike(AttrType::INT pAttrDtype, AttrType::FLOAT pAttrMean, AttrType::FLOAT pAttrScale, AttrType::FLOAT pAttrSeed):
    Operator("RandomNormalLike"), m_AttrDtype(pAttrDtype), m_AttrMean(pAttrMean), m_AttrScale(pAttrScale), m_AttrSeed(pAttrSeed) {};
public:
  ~RandomNormalLike() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getMean();
  AttrType::FLOAT getScale();
  AttrType::FLOAT getSeed();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrDtype;
  AttrType::FLOAT m_AttrMean;
  AttrType::FLOAT m_AttrScale;
  AttrType::FLOAT m_AttrSeed;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomNormalLikeT: public RandomNormalLike {
public:
  RandomNormalLikeT(AttrType::INT pAttrDtype, AttrType::FLOAT pAttrMean, AttrType::FLOAT pAttrScale, AttrType::FLOAT pAttrSeed):
    RandomNormalLike(pAttrDtype, pAttrMean, pAttrScale, pAttrSeed) {};
  ~RandomNormalLikeT() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getMean();
  AttrType::FLOAT getScale();
  AttrType::FLOAT getSeed();

  // TODO: setter
  TensorT<int32_t> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class RandomUniform: public Operator {
protected:
  RandomUniform(AttrType::INT pAttrDtype, AttrType::FLOAT pAttrHigh, AttrType::FLOAT pAttrLow, AttrType::FLOAT pAttrSeed, AttrType::INTS pAttrShape):
    Operator("RandomUniform"), m_AttrDtype(pAttrDtype), m_AttrHigh(pAttrHigh), m_AttrLow(pAttrLow), m_AttrSeed(pAttrSeed), m_AttrShape(pAttrShape) {};
public:
  ~RandomUniform() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getHigh();
  AttrType::FLOAT getLow();
  AttrType::FLOAT getSeed();
  AttrType::INTS getShape();

  // TODO: setter

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrDtype;
  AttrType::FLOAT m_AttrHigh;
  AttrType::FLOAT m_AttrLow;
  AttrType::FLOAT m_AttrSeed;
  AttrType::INTS m_AttrShape;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomUniformT: public RandomUniform {
public:
  RandomUniformT(AttrType::INT pAttrDtype, AttrType::FLOAT pAttrHigh, AttrType::FLOAT pAttrLow, AttrType::FLOAT pAttrSeed, AttrType::INTS pAttrShape):
    RandomUniform(pAttrDtype, pAttrHigh, pAttrLow, pAttrSeed, pAttrShape) {};
  ~RandomUniformT() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getHigh();
  AttrType::FLOAT getLow();
  AttrType::FLOAT getSeed();
  AttrType::INTS getShape();

  // TODO: setter

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class RandomUniformLike: public Operator {
protected:
  RandomUniformLike(AttrType::INT pAttrDtype, AttrType::FLOAT pAttrHigh, AttrType::FLOAT pAttrLow, AttrType::FLOAT pAttrSeed):
    Operator("RandomUniformLike"), m_AttrDtype(pAttrDtype), m_AttrHigh(pAttrHigh), m_AttrLow(pAttrLow), m_AttrSeed(pAttrSeed) {};
public:
  ~RandomUniformLike() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getHigh();
  AttrType::FLOAT getLow();
  AttrType::FLOAT getSeed();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrDtype;
  AttrType::FLOAT m_AttrHigh;
  AttrType::FLOAT m_AttrLow;
  AttrType::FLOAT m_AttrSeed;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class RandomUniformLikeT: public RandomUniformLike {
public:
  RandomUniformLikeT(AttrType::INT pAttrDtype, AttrType::FLOAT pAttrHigh, AttrType::FLOAT pAttrLow, AttrType::FLOAT pAttrSeed):
    RandomUniformLike(pAttrDtype, pAttrHigh, pAttrLow, pAttrSeed) {};
  ~RandomUniformLikeT() {};

  AttrType::INT getDtype();
  AttrType::FLOAT getHigh();
  AttrType::FLOAT getLow();
  AttrType::FLOAT getSeed();

  // TODO: setter
  TensorT<int32_t> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Reciprocal: public Operator {
protected:
  Reciprocal():
    Operator("Reciprocal") {};
public:
  ~Reciprocal() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReciprocalT: public Reciprocal {
public:
  ReciprocalT():
    Reciprocal() {};
  ~ReciprocalT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class ReduceL1: public Operator {
protected:
  ReduceL1(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceL1"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceL1() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceL1T: public ReduceL1 {
public:
  ReduceL1T(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceL1(pAttrAxes, pAttrKeepdims) {};
  ~ReduceL1T() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceL2: public Operator {
protected:
  ReduceL2(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceL2"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceL2() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceL2T: public ReduceL2 {
public:
  ReduceL2T(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceL2(pAttrAxes, pAttrKeepdims) {};
  ~ReduceL2T() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceLogSum: public Operator {
protected:
  ReduceLogSum(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceLogSum"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceLogSum() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceLogSumT: public ReduceLogSum {
public:
  ReduceLogSumT(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceLogSum(pAttrAxes, pAttrKeepdims) {};
  ~ReduceLogSumT() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceLogSumExp: public Operator {
protected:
  ReduceLogSumExp(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceLogSumExp"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceLogSumExp() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceLogSumExpT: public ReduceLogSumExp {
public:
  ReduceLogSumExpT(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceLogSumExp(pAttrAxes, pAttrKeepdims) {};
  ~ReduceLogSumExpT() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceMax: public Operator {
protected:
  ReduceMax(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceMax"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceMax() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMaxT: public ReduceMax {
public:
  ReduceMaxT(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceMax(pAttrAxes, pAttrKeepdims) {};
  ~ReduceMaxT() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceMean: public Operator {
protected:
  ReduceMean(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceMean"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceMean() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMeanT: public ReduceMean {
public:
  ReduceMeanT(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceMean(pAttrAxes, pAttrKeepdims) {};
  ~ReduceMeanT() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceMin: public Operator {
protected:
  ReduceMin(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceMin"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceMin() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceMinT: public ReduceMin {
public:
  ReduceMinT(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceMin(pAttrAxes, pAttrKeepdims) {};
  ~ReduceMinT() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceProd: public Operator {
protected:
  ReduceProd(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceProd"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceProd() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceProdT: public ReduceProd {
public:
  ReduceProdT(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceProd(pAttrAxes, pAttrKeepdims) {};
  ~ReduceProdT() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceSum: public Operator {
protected:
  ReduceSum(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceSum"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceSum() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceSumT: public ReduceSum {
public:
  ReduceSumT(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceSum(pAttrAxes, pAttrKeepdims) {};
  ~ReduceSumT() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class ReduceSumSquare: public Operator {
protected:
  ReduceSumSquare(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    Operator("ReduceSumSquare"), m_AttrAxes(pAttrAxes), m_AttrKeepdims(pAttrKeepdims) {};
public:
  ~ReduceSumSquare() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReduced();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INT m_AttrKeepdims;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReduceSumSquareT: public ReduceSumSquare {
public:
  ReduceSumSquareT(AttrType::INTS pAttrAxes, AttrType::INT pAttrKeepdims):
    ReduceSumSquare(pAttrAxes, pAttrKeepdims) {};
  ~ReduceSumSquareT() {};

  AttrType::INTS getAxes();
  AttrType::INT getKeepdims();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReduced();

  void accept(OperatorVisitor& pV) override;
};

class Relu: public Operator {
protected:
  Relu():
    Operator("Relu") {};
public:
  ~Relu() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReluT: public Relu {
public:
  ReluT():
    Relu() {};
  ~ReluT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Reshape: public Operator {
protected:
  Reshape(AttrType::INTS pAttrShape):
    Operator("Reshape"), m_AttrShape(pAttrShape) {};
public:
  ~Reshape() {};

  AttrType::INTS getShape();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputReshaped();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrShape;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ReshapeT: public Reshape {
public:
  ReshapeT(AttrType::INTS pAttrShape):
    Reshape(pAttrShape) {};
  ~ReshapeT() {};

  AttrType::INTS getShape();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputReshaped();

  void accept(OperatorVisitor& pV) override;
};

class Selu: public Operator {
protected:
  Selu(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrGamma):
    Operator("Selu"), m_AttrAlpha(pAttrAlpha), m_AttrGamma(pAttrGamma) {};
public:
  ~Selu() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getGamma();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
  AttrType::FLOAT m_AttrGamma;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SeluT: public Selu {
public:
  SeluT(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrGamma):
    Selu(pAttrAlpha, pAttrGamma) {};
  ~SeluT() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getGamma();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Shape: public Operator {
protected:
  Shape():
    Operator("Shape") {};
public:
  ~Shape() {};

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputShape();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double), tensor(int8), tensor(int16), tensor(int32), tensor(int64), tensor(uint8), tensor(uint16), tensor(bool)
// T1: tensor(int64)
template<typename T, typename T1>
class ShapeT: public Shape {
public:
  ShapeT():
    Shape() {};
  ~ShapeT() {};

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T1> *OutputShape();

  void accept(OperatorVisitor& pV) override;
};

class Sigmoid: public Operator {
protected:
  Sigmoid():
    Operator("Sigmoid") {};
public:
  ~Sigmoid() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SigmoidT: public Sigmoid {
public:
  SigmoidT():
    Sigmoid() {};
  ~SigmoidT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Size: public Operator {
protected:
  Size():
    Operator("Size") {};
public:
  ~Size() {};

  // TODO: setter
  Tensor *InputData();

  void *OutputSize();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double), tensor(int8), tensor(int16), tensor(int32), tensor(int64), tensor(uint8), tensor(uint16), tensor(bool)
// T1: int64
template<typename T, typename T1>
class SizeT: public Size {
public:
  SizeT():
    Size() {};
  ~SizeT() {};

  // TODO: setter
  TensorT<T> *InputData();

  T1 *OutputSize();

  void accept(OperatorVisitor& pV) override;
};

class Slice: public Operator {
protected:
  Slice(AttrType::INTS pAttrAxes, AttrType::INTS pAttrEnds, AttrType::INTS pAttrStarts):
    Operator("Slice"), m_AttrAxes(pAttrAxes), m_AttrEnds(pAttrEnds), m_AttrStarts(pAttrStarts) {};
public:
  ~Slice() {};

  AttrType::INTS getAxes();
  AttrType::INTS getEnds();
  AttrType::INTS getStarts();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
  AttrType::INTS m_AttrEnds;
  AttrType::INTS m_AttrStarts;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SliceT: public Slice {
public:
  SliceT(AttrType::INTS pAttrAxes, AttrType::INTS pAttrEnds, AttrType::INTS pAttrStarts):
    Slice(pAttrAxes, pAttrEnds, pAttrStarts) {};
  ~SliceT() {};

  AttrType::INTS getAxes();
  AttrType::INTS getEnds();
  AttrType::INTS getStarts();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Softmax: public Operator {
protected:
  Softmax(AttrType::INT pAttrAxis):
    Operator("Softmax"), m_AttrAxis(pAttrAxis) {};
public:
  ~Softmax() {};

  AttrType::INT getAxis();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SoftmaxT: public Softmax {
public:
  SoftmaxT(AttrType::INT pAttrAxis):
    Softmax(pAttrAxis) {};
  ~SoftmaxT() {};

  AttrType::INT getAxis();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Softplus: public Operator {
protected:
  Softplus():
    Operator("Softplus") {};
public:
  ~Softplus() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SoftplusT: public Softplus {
public:
  SoftplusT():
    Softplus() {};
  ~SoftplusT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Softsign: public Operator {
protected:
  Softsign():
    Operator("Softsign") {};
public:
  ~Softsign() {};

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SoftsignT: public Softsign {
public:
  SoftsignT():
    Softsign() {};
  ~SoftsignT() {};

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class SpaceToDepth: public Operator {
protected:
  SpaceToDepth(AttrType::INT pAttrBlocksize):
    Operator("SpaceToDepth"), m_AttrBlocksize(pAttrBlocksize) {};
public:
  ~SpaceToDepth() {};

  AttrType::INT getBlocksize();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrBlocksize;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SpaceToDepthT: public SpaceToDepth {
public:
  SpaceToDepthT(AttrType::INT pAttrBlocksize):
    SpaceToDepth(pAttrBlocksize) {};
  ~SpaceToDepthT() {};

  AttrType::INT getBlocksize();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Split: public Operator {
protected:
  Split(AttrType::INT pAttrAxis, AttrType::INTS pAttrSplit):
    Operator("Split"), m_AttrAxis(pAttrAxis), m_AttrSplit(pAttrSplit) {};
public:
  ~Split() {};

  AttrType::INT getAxis();
  AttrType::INTS getSplit();

  // TODO: setter
  Tensor *InputInput();

  std::list<Tensor *> OutputOutputs();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INTS m_AttrSplit;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SplitT: public Split {
public:
  SplitT(AttrType::INT pAttrAxis, AttrType::INTS pAttrSplit):
    Split(pAttrAxis, pAttrSplit) {};
  ~SplitT() {};

  AttrType::INT getAxis();
  AttrType::INTS getSplit();

  // TODO: setter
  TensorT<T> *InputInput();

  std::list<TensorT<T> *> OutputOutputs();

  void accept(OperatorVisitor& pV) override;
};

class Sqrt: public Operator {
protected:
  Sqrt():
    Operator("Sqrt") {};
public:
  ~Sqrt() {};

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SqrtT: public Sqrt {
public:
  SqrtT():
    Sqrt() {};
  ~SqrtT() {};

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Squeeze: public Operator {
protected:
  Squeeze(AttrType::INTS pAttrAxes):
    Operator("Squeeze"), m_AttrAxes(pAttrAxes) {};
public:
  ~Squeeze() {};

  AttrType::INTS getAxes();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputSqueezed();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SqueezeT: public Squeeze {
public:
  SqueezeT(AttrType::INTS pAttrAxes):
    Squeeze(pAttrAxes) {};
  ~SqueezeT() {};

  AttrType::INTS getAxes();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputSqueezed();

  void accept(OperatorVisitor& pV) override;
};

class Sub: public Operator {
protected:
  Sub(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Sub"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Sub() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SubT: public Sub {
public:
  SubT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Sub(pAttrAxis, pAttrBroadcast) {};
  ~SubT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class Sum: public Operator {
protected:
  Sum():
    Operator("Sum") {};
public:
  ~Sum() {};

  // TODO: setter
  std::list<Tensor *> InputData0();

  Tensor *OutputSum();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class SumT: public Sum {
public:
  SumT():
    Sum() {};
  ~SumT() {};

  // TODO: setter
  std::list<TensorT<T> *> InputData0();

  TensorT<T> *OutputSum();

  void accept(OperatorVisitor& pV) override;
};

class Tanh: public Operator {
protected:
  Tanh():
    Operator("Tanh") {};
public:
  ~Tanh() {};

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class TanhT: public Tanh {
public:
  TanhT():
    Tanh() {};
  ~TanhT() {};

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Tile: public Operator {
protected:
  Tile():
    Operator("Tile") {};
public:
  ~Tile() {};

  // TODO: setter
  Tensor *InputInput();
  Tensor *InputTiles();
  Tensor *InputAxis();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class TileT: public Tile {
public:
  TileT():
    Tile() {};
  ~TileT() {};

  // TODO: setter
  TensorT<T> *InputInput();
  TensorT<T> *InputTiles();
  TensorT<T> *InputAxis();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class TopK: public Operator {
protected:
  TopK(AttrType::INT pAttrAxis, AttrType::INT pAttrK):
    Operator("TopK"), m_AttrAxis(pAttrAxis), m_AttrK(pAttrK) {};
public:
  ~TopK() {};

  AttrType::INT getAxis();
  AttrType::INT getK();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputValues();
  Tensor *OutputIndices();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrK;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
// I: tensor(int64), tensor(int32)
template<typename T, typename I>
class TopKT: public TopK {
public:
  TopKT(AttrType::INT pAttrAxis, AttrType::INT pAttrK):
    TopK(pAttrAxis, pAttrK) {};
  ~TopKT() {};

  AttrType::INT getAxis();
  AttrType::INT getK();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputValues();
  TensorT<I> *OutputIndices();

  void accept(OperatorVisitor& pV) override;
};

class Transpose: public Operator {
protected:
  Transpose(AttrType::INTS pAttrPerm):
    Operator("Transpose"), m_AttrPerm(pAttrPerm) {};
public:
  ~Transpose() {};

  AttrType::INTS getPerm();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputTransposed();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrPerm;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class TransposeT: public Transpose {
public:
  TransposeT(AttrType::INTS pAttrPerm):
    Transpose(pAttrPerm) {};
  ~TransposeT() {};

  AttrType::INTS getPerm();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputTransposed();

  void accept(OperatorVisitor& pV) override;
};

class Unsqueeze: public Operator {
protected:
  Unsqueeze(AttrType::INTS pAttrAxes):
    Operator("Unsqueeze"), m_AttrAxes(pAttrAxes) {};
public:
  ~Unsqueeze() {};

  AttrType::INTS getAxes();

  // TODO: setter
  Tensor *InputData();

  Tensor *OutputExpanded();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrAxes;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class UnsqueezeT: public Unsqueeze {
public:
  UnsqueezeT(AttrType::INTS pAttrAxes):
    Unsqueeze(pAttrAxes) {};
  ~UnsqueezeT() {};

  AttrType::INTS getAxes();

  // TODO: setter
  TensorT<T> *InputData();

  TensorT<T> *OutputExpanded();

  void accept(OperatorVisitor& pV) override;
};

class Xor: public Operator {
protected:
  Xor(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Operator("Xor"), m_AttrAxis(pAttrAxis), m_AttrBroadcast(pAttrBroadcast) {};
public:
  ~Xor() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  Tensor *InputA();
  Tensor *InputB();

  Tensor *OutputC();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrBroadcast;
};
// TODO: Type Constraints
// T: tensor(bool)
// T1: tensor(bool)
template<typename T, typename T1>
class XorT: public Xor {
public:
  XorT(AttrType::INT pAttrAxis, AttrType::INT pAttrBroadcast):
    Xor(pAttrAxis, pAttrBroadcast) {};
  ~XorT() {};

  AttrType::INT getAxis();
  AttrType::INT getBroadcast();

  // TODO: setter
  TensorT<T> *InputA();
  TensorT<T> *InputB();

  TensorT<T1> *OutputC();

  void accept(OperatorVisitor& pV) override;
};

class ATen: public Operator {
protected:
  ATen():
    Operator("ATen") {};
public:
  ~ATen() {};

  // TODO: setter
  std::list<Tensor *> InputInput();

  std::list<Tensor *> OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64), tensor(float16), tensor(float), tensor(double)
template<typename T>
class ATenT: public ATen {
public:
  ATenT():
    ATen() {};
  ~ATenT() {};

  // TODO: setter
  std::list<TensorT<T> *> InputInput();

  std::list<TensorT<T> *> OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Affine: public Operator {
protected:
  Affine(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta):
    Operator("Affine"), m_AttrAlpha(pAttrAlpha), m_AttrBeta(pAttrBeta) {};
public:
  ~Affine() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
  AttrType::FLOAT m_AttrBeta;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class AffineT: public Affine {
public:
  AffineT(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta):
    Affine(pAttrAlpha, pAttrBeta) {};
  ~AffineT() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class ConstantFill: public Operator {
protected:
  ConstantFill(AttrType::INT pAttrDtype, AttrType::INTS pAttrExtraShape, AttrType::INT pAttrInputAsShape, AttrType::INTS pAttrShape, AttrType::FLOAT pAttrValue):
    Operator("ConstantFill"), m_AttrDtype(pAttrDtype), m_AttrExtraShape(pAttrExtraShape), m_AttrInputAsShape(pAttrInputAsShape), m_AttrShape(pAttrShape), m_AttrValue(pAttrValue) {};
public:
  ~ConstantFill() {};

  AttrType::INT getDtype();
  AttrType::INTS getExtraShape();
  AttrType::INT getInputAsShape();
  AttrType::INTS getShape();
  AttrType::FLOAT getValue();

  // TODO: setter
  std::experimental::optional<Tensor *> InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrDtype;
  AttrType::INTS m_AttrExtraShape;
  AttrType::INT m_AttrInputAsShape;
  AttrType::INTS m_AttrShape;
  AttrType::FLOAT m_AttrValue;
};
// TODO: Type Constraints
// T1: tensor(float), tensor(int32), tensor(int64), tensor(bool)
// T2: tensor(float), tensor(int32), tensor(int64), tensor(bool)
template<typename T1, typename T2>
class ConstantFillT: public ConstantFill {
public:
  ConstantFillT(AttrType::INT pAttrDtype, AttrType::INTS pAttrExtraShape, AttrType::INT pAttrInputAsShape, AttrType::INTS pAttrShape, AttrType::FLOAT pAttrValue):
    ConstantFill(pAttrDtype, pAttrExtraShape, pAttrInputAsShape, pAttrShape, pAttrValue) {};
  ~ConstantFillT() {};

  AttrType::INT getDtype();
  AttrType::INTS getExtraShape();
  AttrType::INT getInputAsShape();
  AttrType::INTS getShape();
  AttrType::FLOAT getValue();

  // TODO: setter
  std::experimental::optional<TensorT<T1> *> InputInput();

  TensorT<T2> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Crop: public Operator {
protected:
  Crop(AttrType::INTS pAttrBorder, AttrType::INTS pAttrScale):
    Operator("Crop"), m_AttrBorder(pAttrBorder), m_AttrScale(pAttrScale) {};
public:
  ~Crop() {};

  AttrType::INTS getBorder();
  AttrType::INTS getScale();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrBorder;
  AttrType::INTS m_AttrScale;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class CropT: public Crop {
public:
  CropT(AttrType::INTS pAttrBorder, AttrType::INTS pAttrScale):
    Crop(pAttrBorder, pAttrScale) {};
  ~CropT() {};

  AttrType::INTS getBorder();
  AttrType::INTS getScale();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class FC: public Operator {
protected:
  FC(AttrType::INT pAttrAxis, AttrType::INT pAttrAxisW):
    Operator("FC"), m_AttrAxis(pAttrAxis), m_AttrAxisW(pAttrAxisW) {};
public:
  ~FC() {};

  AttrType::INT getAxis();
  AttrType::INT getAxisW();

  // TODO: setter
  Tensor *InputX();
  Tensor *InputW();
  Tensor *InputB();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
  AttrType::INT m_AttrAxisW;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class FCT: public FC {
public:
  FCT(AttrType::INT pAttrAxis, AttrType::INT pAttrAxisW):
    FC(pAttrAxis, pAttrAxisW) {};
  ~FCT() {};

  AttrType::INT getAxis();
  AttrType::INT getAxisW();

  // TODO: setter
  TensorT<T> *InputX();
  TensorT<T> *InputW();
  TensorT<T> *InputB();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class GRUUnit: public Operator {
protected:
  GRUUnit(AttrType::INT pAttrDropStates):
    Operator("GRUUnit"), m_AttrDropStates(pAttrDropStates) {};
public:
  ~GRUUnit() {};

  AttrType::INT getDropStates();

  // TODO: setter
  Tensor *InputHiddenPrev();
  Tensor *InputGates();
  Tensor *InputSeqLengths();
  Tensor *InputT();

  Tensor *OutputHidden();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrDropStates;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GRUUnitT: public GRUUnit {
public:
  GRUUnitT(AttrType::INT pAttrDropStates):
    GRUUnit(pAttrDropStates) {};
  ~GRUUnitT() {};

  AttrType::INT getDropStates();

  // TODO: setter
  TensorT<T> *InputHiddenPrev();
  TensorT<T> *InputGates();
  TensorT<T> *InputSeqLengths();
  TensorT<T> *InputT();

  TensorT<T> *OutputHidden();

  void accept(OperatorVisitor& pV) override;
};

class GivenTensorFill: public Operator {
protected:
  GivenTensorFill(AttrType::INTS pAttrExtraShape, AttrType::INT pAttrInputAsShape, AttrType::INTS pAttrShape, AttrType::FLOATS pAttrValues):
    Operator("GivenTensorFill"), m_AttrExtraShape(pAttrExtraShape), m_AttrInputAsShape(pAttrInputAsShape), m_AttrShape(pAttrShape), m_AttrValues(pAttrValues) {};
public:
  ~GivenTensorFill() {};

  AttrType::INTS getExtraShape();
  AttrType::INT getInputAsShape();
  AttrType::INTS getShape();
  AttrType::FLOATS getValues();

  // TODO: setter
  std::experimental::optional<Tensor *> InputShape();

  Tensor *OutputX();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INTS m_AttrExtraShape;
  AttrType::INT m_AttrInputAsShape;
  AttrType::INTS m_AttrShape;
  AttrType::FLOATS m_AttrValues;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class GivenTensorFillT: public GivenTensorFill {
public:
  GivenTensorFillT(AttrType::INTS pAttrExtraShape, AttrType::INT pAttrInputAsShape, AttrType::INTS pAttrShape, AttrType::FLOATS pAttrValues):
    GivenTensorFill(pAttrExtraShape, pAttrInputAsShape, pAttrShape, pAttrValues) {};
  ~GivenTensorFillT() {};

  AttrType::INTS getExtraShape();
  AttrType::INT getInputAsShape();
  AttrType::INTS getShape();
  AttrType::FLOATS getValues();

  // TODO: setter
  std::experimental::optional<TensorT<T> *> InputShape();

  TensorT<T> *OutputX();

  void accept(OperatorVisitor& pV) override;
};

class Identity: public Operator {
protected:
  Identity():
    Operator("Identity") {};
public:
  ~Identity() {};

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class IdentityT: public Identity {
public:
  IdentityT():
    Identity() {};
  ~IdentityT() {};

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class If: public Operator {
protected:
  If(AttrType::GRAPH pAttrElseBranch, AttrType::GRAPH pAttrThenBranch):
    Operator("If"), m_AttrElseBranch(pAttrElseBranch), m_AttrThenBranch(pAttrThenBranch) {};
public:
  ~If() {};

  AttrType::GRAPH getElseBranch();
  AttrType::GRAPH getThenBranch();

  // TODO: setter
  Tensor *InputCond();

  std::list<Tensor *> OutputOutputs();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::GRAPH m_AttrElseBranch;
  AttrType::GRAPH m_AttrThenBranch;
};
// TODO: Type Constraints
// V: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// B: tensor(bool)
template<typename V, typename B>
class IfT: public If {
public:
  IfT(AttrType::GRAPH pAttrElseBranch, AttrType::GRAPH pAttrThenBranch):
    If(pAttrElseBranch, pAttrThenBranch) {};
  ~IfT() {};

  AttrType::GRAPH getElseBranch();
  AttrType::GRAPH getThenBranch();

  // TODO: setter
  TensorT<B> *InputCond();

  std::list<TensorT<V> *> OutputOutputs();

  void accept(OperatorVisitor& pV) override;
};

class ImageScaler: public Operator {
protected:
  ImageScaler(AttrType::FLOATS pAttrBias, AttrType::FLOAT pAttrScale):
    Operator("ImageScaler"), m_AttrBias(pAttrBias), m_AttrScale(pAttrScale) {};
public:
  ~ImageScaler() {};

  AttrType::FLOATS getBias();
  AttrType::FLOAT getScale();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOATS m_AttrBias;
  AttrType::FLOAT m_AttrScale;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ImageScalerT: public ImageScaler {
public:
  ImageScalerT(AttrType::FLOATS pAttrBias, AttrType::FLOAT pAttrScale):
    ImageScaler(pAttrBias, pAttrScale) {};
  ~ImageScalerT() {};

  AttrType::FLOATS getBias();
  AttrType::FLOAT getScale();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class Loop: public Operator {
protected:
  Loop(AttrType::GRAPH pAttrBody):
    Operator("Loop"), m_AttrBody(pAttrBody) {};
public:
  ~Loop() {};

  AttrType::GRAPH getBody();

  // TODO: setter
  void *InputM();
  void *InputCond();
  std::list<Tensor *> InputVInitial();

  std::list<Tensor *> OutputVFinalAndScanOutputs();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::GRAPH m_AttrBody;
};
// TODO: Type Constraints
// V: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// I: int64
// B: bool
template<typename V, typename I, typename B>
class LoopT: public Loop {
public:
  LoopT(AttrType::GRAPH pAttrBody):
    Loop(pAttrBody) {};
  ~LoopT() {};

  AttrType::GRAPH getBody();

  // TODO: setter
  I *InputM();
  B *InputCond();
  std::list<TensorT<V> *> InputVInitial();

  std::list<TensorT<V> *> OutputVFinalAndScanOutputs();

  void accept(OperatorVisitor& pV) override;
};

class LoopIndexTensor: public Operator {
protected:
  LoopIndexTensor(AttrType::INT pAttrAxis):
    Operator("LoopIndexTensor"), m_AttrAxis(pAttrAxis) {};
public:
  ~LoopIndexTensor() {};

  AttrType::INT getAxis();

  // TODO: setter
  Tensor *InputT();
  void *InputLoopIdx();

  Tensor *OutputO();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAxis;
};
// TODO: Type Constraints
// T: tensor(float), tensor(int32), tensor(string), tensor(bool), tensor(uint8), tensor(int8), tensor(uint16), tensor(int16), tensor(int64), tensor(float16), tensor(double)
// I: int32
template<typename T, typename I>
class LoopIndexTensorT: public LoopIndexTensor {
public:
  LoopIndexTensorT(AttrType::INT pAttrAxis):
    LoopIndexTensor(pAttrAxis) {};
  ~LoopIndexTensorT() {};

  AttrType::INT getAxis();

  // TODO: setter
  TensorT<T> *InputT();
  I *InputLoopIdx();

  TensorT<T> *OutputO();

  void accept(OperatorVisitor& pV) override;
};

class MeanVarianceNormalization: public Operator {
protected:
  MeanVarianceNormalization(AttrType::INT pAttrAcrossChannels, AttrType::INT pAttrNormalizeVariance):
    Operator("MeanVarianceNormalization"), m_AttrAcrossChannels(pAttrAcrossChannels), m_AttrNormalizeVariance(pAttrNormalizeVariance) {};
public:
  ~MeanVarianceNormalization() {};

  AttrType::INT getAcrossChannels();
  AttrType::INT getNormalizeVariance();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::INT m_AttrAcrossChannels;
  AttrType::INT m_AttrNormalizeVariance;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class MeanVarianceNormalizationT: public MeanVarianceNormalization {
public:
  MeanVarianceNormalizationT(AttrType::INT pAttrAcrossChannels, AttrType::INT pAttrNormalizeVariance):
    MeanVarianceNormalization(pAttrAcrossChannels, pAttrNormalizeVariance) {};
  ~MeanVarianceNormalizationT() {};

  AttrType::INT getAcrossChannels();
  AttrType::INT getNormalizeVariance();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class ParametricSoftplus: public Operator {
protected:
  ParametricSoftplus(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta):
    Operator("ParametricSoftplus"), m_AttrAlpha(pAttrAlpha), m_AttrBeta(pAttrBeta) {};
public:
  ~ParametricSoftplus() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
  AttrType::FLOAT m_AttrBeta;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ParametricSoftplusT: public ParametricSoftplus {
public:
  ParametricSoftplusT(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta):
    ParametricSoftplus(pAttrAlpha, pAttrBeta) {};
  ~ParametricSoftplusT() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Scale: public Operator {
protected:
  Scale(AttrType::FLOAT pAttrScale):
    Operator("Scale"), m_AttrScale(pAttrScale) {};
public:
  ~Scale() {};

  AttrType::FLOAT getScale();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrScale;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ScaleT: public Scale {
public:
  ScaleT(AttrType::FLOAT pAttrScale):
    Scale(pAttrScale) {};
  ~ScaleT() {};

  AttrType::FLOAT getScale();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class ScaledTanh: public Operator {
protected:
  ScaledTanh(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta):
    Operator("ScaledTanh"), m_AttrAlpha(pAttrAlpha), m_AttrBeta(pAttrBeta) {};
public:
  ~ScaledTanh() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  Tensor *InputInput();

  Tensor *OutputOutput();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
  AttrType::FLOAT m_AttrBeta;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ScaledTanhT: public ScaledTanh {
public:
  ScaledTanhT(AttrType::FLOAT pAttrAlpha, AttrType::FLOAT pAttrBeta):
    ScaledTanh(pAttrAlpha, pAttrBeta) {};
  ~ScaledTanhT() {};

  AttrType::FLOAT getAlpha();
  AttrType::FLOAT getBeta();

  // TODO: setter
  TensorT<T> *InputInput();

  TensorT<T> *OutputOutput();

  void accept(OperatorVisitor& pV) override;
};

class ThresholdedRelu: public Operator {
protected:
  ThresholdedRelu(AttrType::FLOAT pAttrAlpha):
    Operator("ThresholdedRelu"), m_AttrAlpha(pAttrAlpha) {};
public:
  ~ThresholdedRelu() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrAlpha;
};
// TODO: Type Constraints
// T: tensor(float16), tensor(float), tensor(double)
template<typename T>
class ThresholdedReluT: public ThresholdedRelu {
public:
  ThresholdedReluT(AttrType::FLOAT pAttrAlpha):
    ThresholdedRelu(pAttrAlpha) {};
  ~ThresholdedReluT() {};

  AttrType::FLOAT getAlpha();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};

class Upsample: public Operator {
protected:
  Upsample(AttrType::FLOAT pAttrHeightScale, AttrType::STRING pAttrMode, AttrType::FLOAT pAttrWidthScale):
    Operator("Upsample"), m_AttrHeightScale(pAttrHeightScale), m_AttrMode(pAttrMode), m_AttrWidthScale(pAttrWidthScale) {};
public:
  ~Upsample() {};

  AttrType::FLOAT getHeightScale();
  AttrType::STRING getMode();
  AttrType::FLOAT getWidthScale();

  // TODO: setter
  Tensor *InputX();

  Tensor *OutputY();

  void accept(OperatorVisitor& pV) override;
private:
  AttrType::FLOAT m_AttrHeightScale;
  AttrType::STRING m_AttrMode;
  AttrType::FLOAT m_AttrWidthScale;
};
// TODO: Type Constraints
// T: tensor(bool), tensor(int32), tensor(int64), tensor(float16), tensor(float), tensor(double)
template<typename T>
class UpsampleT: public Upsample {
public:
  UpsampleT(AttrType::FLOAT pAttrHeightScale, AttrType::STRING pAttrMode, AttrType::FLOAT pAttrWidthScale):
    Upsample(pAttrHeightScale, pAttrMode, pAttrWidthScale) {};
  ~UpsampleT() {};

  AttrType::FLOAT getHeightScale();
  AttrType::STRING getMode();
  AttrType::FLOAT getWidthScale();

  // TODO: setter
  TensorT<T> *InputX();

  TensorT<T> *OutputY();

  void accept(OperatorVisitor& pV) override;
};


} // namespace tensor
} // namespace onnc

