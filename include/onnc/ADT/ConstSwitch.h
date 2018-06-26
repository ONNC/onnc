//===- ConstSwitch.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===-----------------------------------------------------------------------===//
#ifndef ONNC_ADT_CONSTANT_SWITCH_H
#define ONNC_ADT_CONSTANT_SWITCH_H
#include <cstring>
#include <cassert>

namespace onnc {

/** \class ConstSwitch
 *  \brief A switch-like statement whose cases are types.
 *
 *  Example:
 *  @code
 *  int a;
 *  std::string a = ConstSwitch<std::string>(a)
 *    .Case<Red>("red")
 *    .Case<Blue>("blue")
 *    .Default("unknown);
 *  @endcode
 */
template<typename R>
class ConstSwitch
{
public:
  explicit ConstSwitch(int pConstant)
  : m_Constant(pConstant), m_pResult(NULL) { }

  template<int S>
  ConstSwitch& Case(const R& pValue) {
    if ((NULL == m_pResult) && S == m_Constant) {
      m_pResult = &pValue;
    }
    return *this;
  }

  template<int S0, int S1>
  ConstSwitch& Cases(const R& pValue) {
    return Case<S0>(pValue)
           .template Case<S1>(pValue);
  }

  template<int S0, int S1, int S2>
  ConstSwitch& Cases(const R& pValue) {
    return Case<S0>(pValue)
           .template Case<S1>(pValue)
           .template Case<S2>(pValue);
  }

  template<int S0, int S1, int S2, int S3>
  ConstSwitch& Cases(const R& pValue) {
    return Case<S0>(pValue)
           .template Case<S1>(pValue)
           .template Case<S2>(pValue)
           .template Case<S3>(pValue);
  }

  R Default(const R& pValue) const {
    if (NULL != m_pResult)
      return *m_pResult;
    return pValue;
  }

  operator R() const {
    assert(NULL != m_pResult && "Fell off the end of a string-switch");
    return *m_pResult;
  }

private:
  /// The constant we are matching.
  int m_Constant;

  /// The pointer to the result of this switch statement, once known,
  /// null before that.
  const R* m_pResult;
};

} // namespace of onnc

#endif
