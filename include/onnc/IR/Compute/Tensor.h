//===- Tensor.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_TENSOR_H
#define ONNC_IR_COMPUTE_TENSOR_H
#include <onnc/IR/Compute/Value.h>
#include <vector>

namespace onnc {

class Tensor : public onnc::Value
{
public:
  typedef std::vector<int64_t> Dimensions;

public:
  Tensor()
    : onnc::Value() {
  }

  Tensor(onnc::Value::Type pKind)
    : onnc::Value(pKind) {
  }

  Tensor(const std::string& pName, onnc::Value::Type pKind)
    : onnc::Value(pName, pKind) {
  }

  Tensor(onnc::Value::Type pKind, ::onnx::Tensor& pAdaptee)
    : onnc::Value(pKind, pAdaptee) {
  }

  unsigned int getNumOfDimensions() const { return m_Dimensions.size(); }

  unsigned int dimension(unsigned int pIdx) { return m_Dimensions[pIdx]; }

  void print(std::ostream& pOS) const {
    pOS << getName() << "[]";
  }

protected:
  Dimensions m_Dimensions;
};

/** \class TensorT
 *  \brief TensorT is a placeholder of tensor in a network
 */
template<typename ValueType, Value::Type Kind>
class TensorT : public onnc::Tensor
{
public:
  typedef std::vector<ValueType> ValueList;

public:
  TensorT()
    : onnc::Tensor(Kind) {
  }

  TensorT(const std::string& pName)
    : onnc::Tensor(pName, Kind) {
  }

  TensorT(::onnx::Tensor& pAdaptee)
    : onnc::Tensor(Kind, pAdaptee) {
  }

  virtual ~TensorT() { }

  ValueList& getValues() { assert(false && "Not partial specified!"); }

  const ValueList& getValues() const { assert(false && "Not partial specified!"); }
};

#define DEFINE_TENSOR_ACCESSORS(ValueType, NativeType, Element) \
template<> TensorT<NativeType, ValueType>::ValueList& \
TensorT<NativeType, ValueType>::getValues() { \
  return m_pAdaptee->Element(); \
} \
template<> const TensorT<NativeType, ValueType>::ValueList& \
TensorT<NativeType, ValueType>::getValues() const { \
  return m_pAdaptee->Element(); \
}

DEFINE_TENSOR_ACCESSORS(onnc::Value::kFloatTensor,   float,      floats)
// XXX: casting int32 to float? That ONNX does
DEFINE_TENSOR_ACCESSORS(onnc::Value::kFloat16Tensor, int,        int32s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kBooleanTensor, int,        int32s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kInt8Tensor,    int,        int32s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kInt16Tensor,   int,        int32s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kInt32Tensor,   int,        int32s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kUint8Tensor,   int,        int32s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kUint16Tensor,  int,        int32s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kInt64Tensor,   int64_t,    int64s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kUint32Tensor,  int64_t,    int64s)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kDoubleTensor,  double,     doubles)
DEFINE_TENSOR_ACCESSORS(onnc::Value::kStringTensor, std::string, strings)

typedef TensorT<float,       onnc::Value::kFloatTensor>   FloatTensor;
typedef TensorT<float,       onnc::Value::kFloat16Tensor> Float16Tensor;
typedef TensorT<bool,        onnc::Value::kBooleanTensor> BooleanTensor;
typedef TensorT<int8_t,      onnc::Value::kInt8Tensor>    Int8Tensor;
typedef TensorT<int16_t,     onnc::Value::kInt16Tensor>   Int16Tensor;
typedef TensorT<int32_t,     onnc::Value::kInt32Tensor>   Int32Tensor;
typedef TensorT<uint8_t,     onnc::Value::kUint8Tensor>   Uint8Tensor;
typedef TensorT<uint16_t,    onnc::Value::kUint16Tensor>  Uint16Tensor;
typedef TensorT<int64_t,     onnc::Value::kInt64Tensor>   Int64Tensor;
typedef TensorT<uint32_t,    onnc::Value::kUint32Tensor>  Uint32Tensor;
typedef TensorT<double,      onnc::Value::kDoubleTensor>  DoubleTensor;
typedef TensorT<std::string, onnc::Value::kStringTensor>  StringTensor;

} // namespace of onnc

#endif
