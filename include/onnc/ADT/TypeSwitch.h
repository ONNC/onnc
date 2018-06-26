//===- TypeSwitch.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_TYPE_SWITCH_H
#define ONNC_ADT_TYPE_SWITCH_H
#include <onnc/ADT/TypeTag.h>
#include <cstring>
#include <cassert>

namespace onnc {

/** \class TypeSwitch
 *  \brief A switch-like statement whose cases are types.
 *
 *  Example:
 *  @code
 *  Color color = TypeSwitch<Color>(type_tag<bool>())
 *    .Case<bool>(Red)
 *    .Case<double>(Bule)
 *    .Default(UnknownColor);
 *  @endcode
 */
template<typename T, typename R = T>
class TypeSwitch
{
public:
  explicit TypeSwitch(TypeTag pTypeTag)
  : m_TypeTag(pTypeTag), m_pResult(NULL) { }

  template<typename S>
  TypeSwitch& Case(const T& pValue) {
    if ((NULL == m_pResult) && type_tag<S>() == m_TypeTag) {
      m_pResult = &pValue;
    }
    return *this;
  }

  template<typename S0, typename S1>
  TypeSwitch& Cases(const T& pValue) {
    return Case<S0>(pValue)
           .template Case<S1>(pValue);
  }

  template<typename S0, typename S1, typename S2>
  TypeSwitch& Cases(const T& pValue) {
    return Case<S0>(pValue)
           .template Case<S1>(pValue)
           .template Case<S2>(pValue);
  }

  template<typename S0, typename S1, typename S2, typename S3>
  TypeSwitch& Cases(const T& pValue) {
    return Case<S0>(pValue)
           .template Case<S1>(pValue)
           .template Case<S2>(pValue)
           .template Case<S3>(pValue);
  }

  R Default(const T& pValue) const {
    if (NULL != m_pResult)
      return *m_pResult;
    return pValue;
  }

  operator R() const {
    assert(NULL != m_pResult && "Fell off the end of a string-switch");
    return *m_pResult;
  }

private:
  /// The string we are matching.
  TypeTag m_TypeTag;

  /// The pointer to the result of this switch statement, once known,
  /// null before that.
  const R* m_pResult;
};

} // namespace of onnc

#endif
