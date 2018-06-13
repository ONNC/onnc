//===- Scalar.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_SCALAR_H
#define ONNC_IR_COMPUTE_SCALAR_H
#include <onnc/IR/Compute/Value.h>
#include <ostream>

namespace onnc {

/** \class Scalar
 *  \brief Scalar is a placehoder of scalars in a network.
 */
class Scalar : public onnc::Value
{
public:
  Scalar()
    : onnc::Value(onnc::Value::kUndefined) {
  }

  Scalar(onnc::Value::Type pKind)
    : onnc::Value(pKind) {
  }

  Scalar(const std::string& pName, onnc::Value::Type pKind)
    : onnc::Value(pName, pKind) {
  }

  Scalar(onnc::Value::Type pKind, ::onnx::Tensor& pAdaptee)
    : onnc::Value(pKind, pAdaptee) {
  }
};

template<typename ValueType, Value::Type Kind>
class ScalarT : public onnc::Scalar
{
public:
  ScalarT()
    : onnc::Scalar(Kind) {
  }

  ScalarT(const std::string& pName)
    : onnc::Scalar(pName, Kind) {
  }

  ScalarT(::onnx::Tensor& pAdaptee)
    : onnc::Scalar(Kind, pAdaptee) {
  }

  virtual ~ScalarT() { }

  ValueType getValue() const { assert(false && "Not partial specified!"); }

  ScalarT& setValue(const ValueType& pValue) {
    assert(false && "Not partial specified!");
    return *this;
  }
};

#define DEFINE_SCALAR_ACCESSORS(ValueType, NativeType, Element) \
template<> NativeType ScalarT<NativeType, ValueType>::getValue() const { \
  return m_pAdaptee->Element().operator[](0); \
} \
template<> ScalarT<NativeType, ValueType>& \
ScalarT<NativeType, ValueType>::setValue(const NativeType& pValue) { \
  if (m_pAdaptee->Element().empty()) \
    m_pAdaptee->Element().push_back(pValue); \
  else \
    m_pAdaptee->Element().operator[](0) = pValue; \
  return *this; \
}

/// Define accessors - see onnx/common/ir_pb_converter.cc
DEFINE_SCALAR_ACCESSORS(onnc::Value::kFloat,   float,       floats)
// XXX: casting int32 to float? That ONNX does
DEFINE_SCALAR_ACCESSORS(onnc::Value::kFloat16, float,       int32s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kBoolean, bool,        int32s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kInt8,    int8_t,      int32s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kInt16,   int16_t,     int32s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kInt32,   int32_t,     int32s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kUint8,   uint8_t,     int32s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kUint16,  uint16_t,    int32s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kInt64,   int64_t,     int64s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kUint32,  uint32_t,    int64s)
DEFINE_SCALAR_ACCESSORS(onnc::Value::kDouble,  double,      doubles)

typedef ScalarT<float,       onnc::Value::kFloat>   FloatScalar;
typedef ScalarT<float,       onnc::Value::kFloat16> Float16Scalar;
typedef ScalarT<bool,        onnc::Value::kBoolean> BooleanScalar;
typedef ScalarT<int8_t,      onnc::Value::kInt8>    Int8Scalar;
typedef ScalarT<int16_t,     onnc::Value::kInt16>   Int16Scalar;
typedef ScalarT<int32_t,     onnc::Value::kInt32>   Int32Scalar;
typedef ScalarT<uint8_t,     onnc::Value::kUint8>   Uint8Scalar;
typedef ScalarT<uint16_t,    onnc::Value::kUint16>  Uint16Scalar;
typedef ScalarT<int64_t,     onnc::Value::kInt64>   Int64Scalar;
typedef ScalarT<uint32_t,    onnc::Value::kUint32>  Uint32Scalar;
typedef ScalarT<double,      onnc::Value::kDouble>  DoubleScalar;

class StringScalar : public onnc::Scalar
{
public:
  StringScalar()
    : onnc::Scalar(onnc::Value::kString) {
  }

  StringScalar(const std::string& pName)
    : onnc::Scalar(pName, onnc::Value::kString) {
  }

  StringScalar(::onnx::Tensor& pAdaptee)
    : onnc::Scalar(onnc::Value::kString, pAdaptee) {
  }

  virtual ~StringScalar() { }

  const std::string& getValue() const {
    return m_pAdaptee->strings().operator[](0);
  }

  StringScalar& setValue(const std::string& pValue) {
    if (m_pAdaptee->strings().empty())
      m_pAdaptee->strings().push_back(pValue);
    else
      m_pAdaptee->strings().operator[](0) = pValue;
    return *this;
  }

  void print(std::ostream& pOS) const {
    pOS << getName() << ": " << getValue();
  }
};
} // namespace of onnc

#endif
