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
  Scalar();

  Scalar(onnc::Value::Type pKind);

  Scalar(const std::string& pName, onnc::Value::Type pKind);

  Scalar(onnc::Value::Type pKind, ::onnx::Tensor& pAdaptee);
};

template<typename ValueType, Value::Type Kind, bool IsClass>
class ScalarT { };

template<typename ValueType, Value::Type Kind>
class ScalarT<ValueType, Kind, false> : public onnc::Scalar
{
public:
  ScalarT()
    : onnc::Scalar(Kind), m_Value(0) {
  }

  ScalarT(const std::string& pName)
    : onnc::Scalar(pName, Kind), m_Value(0) {
  }

  ScalarT(::onnx::Tensor& pAdaptee)
    : onnc::Scalar(Kind, pAdaptee), m_Value(0) {
  }

  virtual ~ScalarT() { }

  ValueType getValue() const { return m_Value; }

  void setValue(const ValueType& pValue) { m_Value = pValue; }

  void print(std::ostream& pOS) const { pOS << m_Value; }

private:
  ValueType m_Value;
};

template<typename ValueType, Value::Type Kind>
class ScalarT<ValueType, Kind, true> : public onnc::Scalar, public ValueType
{
public:
  ScalarT()
    : onnc::Scalar(Kind), ValueType() {
  }

  ScalarT(const std::string& pName)
    : onnc::Scalar(pName, Kind), ValueType() {
  }

  ScalarT(::onnx::Tensor& pAdaptee)
    : onnc::Scalar(Kind, pAdaptee), ValueType() {
  }

  virtual ~ScalarT() { }

  const ValueType& getValue() const { return *this; }

  void setValue(const ValueType& pValue) { ValueType::operator=(pValue); }

  void print(std::ostream& pOS) const { pOS << getValue(); }
};

typedef ScalarT<float,       onnc::Value::kFloat,   false> FloatScalar;
typedef ScalarT<float,       onnc::Value::kFloat16, false> Float16Scalar;
typedef ScalarT<bool,        onnc::Value::kBoolean, false> BooleanScalar;
typedef ScalarT<int8_t,      onnc::Value::kInt8,    false> Int8Scalar;
typedef ScalarT<int16_t,     onnc::Value::kInt16,   false> Int16Scalar;
typedef ScalarT<int32_t,     onnc::Value::kInt32,   false> Int32Scalar;
typedef ScalarT<uint8_t,     onnc::Value::kUint8,   false> Uint8Scalar;
typedef ScalarT<uint16_t,    onnc::Value::kUint16,  false> Uint16Scalar;
typedef ScalarT<int64_t,     onnc::Value::kInt64,   false> Int64Scalar;
typedef ScalarT<uint32_t,    onnc::Value::kUint32,  false> Uint32Scalar;
typedef ScalarT<double,      onnc::Value::kDouble,  false> DoubleScalar;
typedef ScalarT<std::string, onnc::Value::kString,  true>  StringScalar;

} // namespace of onnc

#endif
