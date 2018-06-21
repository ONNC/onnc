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
#include <onnx/common/ir.h>
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
    kUndefined = onnx::TensorProto_DataType_UNDEFINED,

    // Basic types.
    kFloat     = onnx::TensorProto_DataType_FLOAT,   // float
    kUint8     = onnx::TensorProto_DataType_UINT8,   // uint8_t
    kInt8      = onnx::TensorProto_DataType_INT8,    // int8_t
    kUint16    = onnx::TensorProto_DataType_UINT16,  // uint16_t
    kInt16     = onnx::TensorProto_DataType_INT16,   // int16_t
    kInt32     = onnx::TensorProto_DataType_INT32,   // int32_t
    kInt64     = onnx::TensorProto_DataType_INT64,   // int64_t
    kString    = onnx::TensorProto_DataType_STRING,  // string
    kBoolean   = onnx::TensorProto_DataType_BOOL,    // bool

    // Advanced types
    kFloat16   = onnx::TensorProto_DataType_FLOAT16,
    kDouble    = onnx::TensorProto_DataType_DOUBLE,
    kUint32    = onnx::TensorProto_DataType_UINT32,
    kUint64    = onnx::TensorProto_DataType_UINT64,

    // complex with float32 real and imaginary components
    kComplex64  = onnx::TensorProto_DataType_COMPLEX64,
    // complex with float64 real and imaginary components
    kComplex128 = onnx::TensorProto_DataType_COMPLEX128
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

  Value(Type pKind, ::onnx::Tensor& pAdaptee)
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

  void adapt(::onnx::Tensor& pAdaptee);

  ::onnx::Tensor* adaptee() { return m_pAdaptee; }

  const ::onnx::Tensor* adaptee() const { return m_pAdaptee; }

  bool hasAdaptee() const { return (nullptr != m_pAdaptee); }

protected:
  onnc::Define* m_pDefine;
  unsigned int m_DefineNo;
  UseList m_UseList;
  std::string m_Name;
  Type m_Kind;

  // Object adaptee. ONNX represents all types in a single tensor.
  ::onnx::Tensor* m_pAdaptee;
};

} // namespace of onnc

#endif
