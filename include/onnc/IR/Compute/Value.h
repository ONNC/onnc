//===- Value.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_VALUE_H
#define ONNC_IR_COMPUTE_VALUE_H
#include <onnc/IR/Compute/Use.h>
#include <onnc/IR/Compute/Define.h>
#include <onnc/Config/ONNX.h>
#include <vector>
#include <string>

namespace onnc {

/** \class Value
 *  \brief Value represents all values computed by a program.
 *
 *  Value is an object adapter of ::onnx::Tensor.
 */
class Value
{
public:
  typedef std::vector<onnc::Use> UseList;
  enum Type {
    kUndefined = xValueType::kUndefined,

    // Basic types.
    kFloat     = xValueType::kFloat,   // float
    kUint8     = xValueType::kUint8,   // uint8_t
    kInt8      = xValueType::kInt8,    // int8_t
    kUint16    = xValueType::kUint16,  // uint16_t
    kInt16     = xValueType::kInt16,   // int16_t
    kInt32     = xValueType::kInt32,   // int32_t
    kInt64     = xValueType::kInt64,   // int64_t
    kString    = xValueType::kString,  // string
    kBoolean   = xValueType::kBoolean, // bool

    // Advanced types
    kFloat16   = xValueType::kFloat16,
    kDouble    = xValueType::kDouble,
    kUint32    = xValueType::kUint32,
    kUint64    = xValueType::kUint64,

    // complex with float32 real and imaginary components
    kComplex64  = xValueType::kComplex64,
    // complex with float64 real and imaginary components
    kComplex128 = xValueType::kComplex128
  };

public:
  Value()
    : m_Name(), m_Kind(kUndefined), m_pAdaptee(nullptr) {
  }

  Value(Type pKind)
    : m_Name(), m_Kind(pKind), m_pAdaptee(nullptr) {
  }

  Value(const std::string& pName, Type pKind)
    : m_Name(pName), m_Kind(pKind), m_pAdaptee(nullptr) {
  }

  Value(Type pKind, xTensor& pAdaptee)
    : m_Name(pAdaptee.name()), m_Kind(pKind), m_pAdaptee(&pAdaptee) {
  }

  virtual ~Value() { }

  const std::string& getName() const { return m_Name; }

  Type kind() const { return m_Kind; }

  onnc::Define* getDefine() { return m_pDefine; }

  unsigned int getDefineNo() { return m_DefineNo; }

  const UseList& getUses() const { return m_UseList; }

  UseList& getUses() { return m_UseList; }

  /// replace all uses of this value to @ref pValue 
  void replaceAllUsesWith(Value& pValue);

  void adapt(xTensor& pAdaptee);

  xTensor* adaptee() { return m_pAdaptee; }

  const xTensor* adaptee() const { return m_pAdaptee; }

  bool hasAdaptee() const { return (nullptr != m_pAdaptee); }

protected:
  onnc::Define* m_pDefine;
  unsigned int m_DefineNo;
  UseList m_UseList;
  std::string m_Name;
  Type m_Kind;

  // Object adaptee. ONNX represents all types in a single tensor.
  xTensor* m_pAdaptee;
};

} // namespace of onnc

#endif
