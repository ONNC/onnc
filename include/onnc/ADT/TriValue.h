//===- TriValue.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_TRIVALUE_H
#define ONNC_ADT_TRIVALUE_H
#include <onnc/ADT/TypeTraits.h>
#include <onnc/Support/MsgHandling.h>

namespace onnc {

struct GenericTriValue
{
  virtual ~GenericTriValue() { }
  virtual bool compare(const GenericTriValue &V) const = 0;
};

template<class DataType> struct TriValue;

/** \class TriValueBase
 *  \brief 
 * The default value safely does nothing. Option value printing is only
 * best-effort.
 */
template<class DataType, bool IsClass>
struct TriValueBase : public GenericTriValue
{
public:
  bool hasValue() const { return false; }

  const DataType &getValue() const { unreachable(unreach_no_value); }

  // Some options may take their value from a different data type.
  template<class DT>
  void setValue(const DT& /*V*/) {}

  bool compare(const DataType &/*V*/) const { return false; }

  virtual bool compare(const GenericTriValue& /*V*/) const { return false; }
};

template<class DataType>
struct TriValueBase<DataType, false> : public GenericTriValue
{
public:
  TriValueBase() : m_bValid(false) { }

  bool hasValue() const { return m_bValid; }

  const DataType &getValue() const {
    assert(m_bValid && "invalid option value");
    return m_Value;
  }

  void setValue(const DataType& pValue) {
    m_bValid = true;
    m_Value = pValue;
  }

  bool compare(const DataType &pValue) const {
    return m_bValid && (m_Value != pValue);
  }

  virtual bool compare(const GenericTriValue &pValue) const {
    const TriValueBase<DataType, false> &vc =
      static_cast< const TriValueBase<DataType, false>& >(pValue);
    if (!vc.hasValue()) return false;
    return compare(vc.getValue());
  }

private:
  DataType m_Value;
  bool m_bValid;
};

/** \class TriValue
 *  \brief TriValue provides un-initial state for a variable.
 *
 *  The term TriValue is borrowed from hardware description language - tristate.
 */
template<class DataType>
class TriValue : public TriValueBase<DataType, is_class<DataType>::value>
{
public:
  TriValue() {}

  TriValue(const DataType& V) {
    this->setValue(V);
  }

  // Some options may take their value from a different data type.
  template<class DT>
  TriValue<DataType> &operator=(const DT& pValue) {
    this->setValue(pValue);
    return *this;
  }
};

} // namespace of onnc

#endif
