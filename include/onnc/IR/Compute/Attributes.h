//===- Attributes.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_ATTRIBUTES_H
#define ONNC_IR_COMPUTE_ATTRIBUTES_H
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/ComputeGraph.h>
#include <onnc/ADT/ConstSwitch.h>

#include <initializer_list>
#include <string>

namespace onnc {

class Attribute
{
public:
  enum Type {
    kBoolean,
    kFloat,
    kInteger,
    kString,
    kTensor,
    kGraph
  };

public:
  Attribute(Type pKind)
    : m_Kind(pKind) {
  }

  Attribute(const Attribute& pCopy)
    : m_Kind(pCopy.kind()) {
  }

  virtual ~Attribute() { }

  Attribute& operator=(const Attribute& pCopy) {
    m_Kind = pCopy.kind();
    return *this;
  }

  Type kind() const { return m_Kind; }

  virtual void print(std::ostream& pOS) const {
    pOS << ConstSwitch<std::string>(kind())
           .Case<kBoolean>("bool")
           .Case<kFloat>("float")
           .Case<kInteger>("int")
           .Case<kString>("string")
           .Case<kTensor>("tensor")
           .Case<kGraph>("graph")
           .Default("undefined");
  }

private:
  Type m_Kind;
};

template<typename ValueTypeT, Attribute::Type Kind>
class ScalarAttribute : public Attribute
{
public:
  using ValueType = ValueTypeT;

public:
  ScalarAttribute()
    : Attribute(Kind), m_Value() {
  }

  ScalarAttribute(const ValueType& pValue)
    : Attribute(Kind), m_Value(pValue) {
  }

  ScalarAttribute(const ScalarAttribute& pCopy)
    : Attribute(pCopy), m_Value(pCopy.value()) {
  }

  virtual ~ScalarAttribute() { }

  ScalarAttribute& operator=(const ScalarAttribute& pCopy) {
    Attribute::operator=(pCopy);
    m_Value = pCopy.value();
    return *this;
  }

  operator ValueType() const { return m_Value; }

  /// return a copy
  ValueType value() { return m_Value; }

  const ValueType& value() const { return m_Value; }

  void setValue(const ValueType& pValue) { m_Value = pValue; }

  void print(std::ostream& pOS) const override;

private:
  ValueType m_Value;
};

template<typename ValueTypeT, Attribute::Type Kind>
class VectorAttribute : public Attribute
{
public:
  using VectorType = std::vector<ValueTypeT>;
  using ValueType = ValueTypeT;
  using Size = typename VectorType::size_type;

public:
  VectorAttribute()
    : Attribute(Kind), m_Vector() {
  }

  VectorAttribute(std::initializer_list<ValueType> list)
    : Attribute(Kind), m_Vector(list) {
  }

  VectorAttribute(const VectorType& pVector)
    : Attribute(Kind), m_Vector(pVector) {
  }

  VectorAttribute(VectorType&& pVector)
    : Attribute(Kind), m_Vector(pVector) {
  }

  VectorAttribute(const VectorAttribute& pCopy)
    : Attribute(pCopy), m_Vector(pCopy.vector()) {
  }

  /// fill constructor.
  /// Constructs a container with pN elements. Each element is a copy of pV
  VectorAttribute(typename VectorType::size_type pN, const ValueTypeT& pV)
    : Attribute(Kind), m_Vector(pN, pV) {
  }

  virtual ~VectorAttribute() { }

  VectorAttribute& operator=(const VectorAttribute& pCopy) {
    Attribute::operator=(pCopy);
    m_Vector = pCopy.vector();
    return *this;
  }

  VectorType& vector() { return m_Vector; }

  const VectorType& vector() const { return m_Vector; }

  ValueType& at(Size pIdx) { return m_Vector.at(pIdx); }

  const ValueType& at(Size pIdx) const { return m_Vector.at(pIdx); }

  void print(std::ostream& pOS) const override;

private:
  VectorType m_Vector;
};

typedef ScalarAttribute<bool, Attribute::kBoolean>       BoolAttr;
typedef ScalarAttribute<double, Attribute::kFloat>       FloatAttr;
typedef ScalarAttribute<int64_t, Attribute::kInteger>    IntAttr;
typedef ScalarAttribute<std::string, Attribute::kString> StringAttr;
typedef ScalarAttribute<Tensor, Attribute::kTensor>      TensorAttr;
typedef ScalarAttribute<ComputeGraph, Attribute::kGraph> GraphAttr;

typedef VectorAttribute<bool, Attribute::kBoolean>       BoolsAttr;
typedef VectorAttribute<double, Attribute::kFloat>       FloatsAttr;
typedef VectorAttribute<int64_t, Attribute::kInteger>    IntsAttr;
typedef VectorAttribute<std::string, Attribute::kString> StringsAttr;
typedef VectorAttribute<Tensor, Attribute::kTensor>      TensorsAttr;
typedef VectorAttribute<ComputeGraph, Attribute::kGraph> GraphsAttr;

template<typename ValueType, Attribute::Type Kind> std::ostream&
operator<<(std::ostream& pOS, const ScalarAttribute<ValueType, Kind>& pV) {
  pV.print(pOS);
  return pOS;
}

template<typename ValueType, Attribute::Type Kind> std::ostream&
operator<<(std::ostream& pOS, const VectorAttribute<ValueType, Kind>& pV) {
  pV.print(pOS);
  return pOS;
}

template<typename ValueType, Attribute::Type Kind>
void ScalarAttribute<ValueType, Kind>::print(std::ostream& pOS) const {
  pOS << "undefined";
}
template<> void BoolAttr::print(std::ostream& pOS) const;
template<> void FloatAttr::print(std::ostream& pOS) const;
template<> void IntAttr::print(std::ostream& pOS) const;
template<> void StringAttr::print(std::ostream& pOS) const;
template<> void TensorAttr::print(std::ostream& pOS) const;
template<> void GraphAttr::print(std::ostream& pOS) const;

template<typename ValueType, Attribute::Type Kind>
void VectorAttribute<ValueType, Kind>::print(std::ostream& pOS) const {
  pOS << "undefined[" << vector().size() << "]";
}
template<> void BoolsAttr::print(std::ostream& pOS) const;
template<> void FloatsAttr::print(std::ostream& pOS) const;
template<> void IntsAttr::print(std::ostream& pOS) const;
template<> void StringsAttr::print(std::ostream& pOS) const;
template<> void TensorsAttr::print(std::ostream& pOS) const;
template<> void GraphsAttr::print(std::ostream& pOS) const;

} // namespace of onnc

#endif
