//===- StringSwitch.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_STRING_SWITCH_H
#define ONNC_ADT_STRING_SWITCH_H
#include <onnc/ADT/StringRef.h>
#include <cstring>
#include <cassert>

namespace onnc {

/** \class StringSwitch
 *  \brief A switch-like statement whose cases are string literals.
 *
 *  The StringSwitch class comes from LLVM ADT. It is a simple form of a
 *  switch() statement that determines if the given string matches one of the
 *  given string literals. The template type \p T is the type of the value that
 *  will be returned from the string-switch expression.
 *
 *  Example:
 *  @code
 *  Color color = StringSwitch<Color>(argv[i])
 *    .Case("red", Red)
 *    .Case("blue", Blue)
 *    .Default(UnknownColor);
 *  @endcode
 */
template<typename T, typename R = T>
class StringSwitch
{
public:
  explicit StringSwitch(StringRef pS)
  : m_Str(pS), m_pResult(NULL) { }

  template<unsigned int N>
  StringSwitch& Case(const char (&pS)[N], const T& pValue) {
    if ((NULL == m_pResult) &&
        (N - 1 == m_Str.size())  &&
        (0 == std::memcmp(pS, m_Str.data(), N-1))) {
      m_pResult = &pValue;
    }
    return *this;
  }

  template<unsigned int N0, unsigned int N1>
  StringSwitch& Cases(const char (&pS0)[N0],
                     const char (&pS1)[N1],
                     const T& pValue) {
    return Case(pS0, pValue)
           .Case(pS1, pValue);
  }

  template<unsigned int N0, unsigned int N1, unsigned int N2>
  StringSwitch& Cases(const char (&pS0)[N0],
                     const char (&pS1)[N1],
                     const char (&pS2)[N2],
                     const T& pValue) {
    return Case(pS0, pValue)
           .Case(pS1, pValue)
           .Case(pS2, pValue);
  }

  template<unsigned int N0, unsigned int N1, unsigned int N2, unsigned int N3>
  StringSwitch& Cases(const char (&pS0)[N0],
                     const char (&pS1)[N1],
                     const char (&pS2)[N2],
                     const char (&pS3)[N3],
                     const T& pValue) {
    return Case(pS0, pValue)
           .Case(pS1, pValue)
           .Case(pS2, pValue)
           .Case(pS3, pValue);
  }

  template<unsigned int N0, unsigned int N1, unsigned int N2, unsigned int N3, unsigned int N4>
  StringSwitch& Cases(const char (&pS0)[N0],
                     const char (&pS1)[N1],
                     const char (&pS2)[N2],
                     const char (&pS3)[N3],
                     const char (&pS4)[N4],
                     const T& pValue) {
    return Case(pS0, pValue)
           .Case(pS1, pValue)
           .Case(pS2, pValue)
           .Case(pS3, pValue)
           .Case(pS4, pValue);
  }
  template<unsigned int N>
  StringSwitch& StartsWith(const char (&pS)[N], const T& pValue) {
    if ((NULL == m_pResult) &&
        (N - 1 <= m_Str.size())  &&
        (0 == std::memcmp(pS, m_Str.data(), N-1))) {
      m_pResult = &pValue;
    }
    return *this;
  }

  template<unsigned int N0, unsigned int N1>
  StringSwitch& StartsWith(const char (&pS0)[N0],
                           const char (&pS1)[N1],
                           const T& pValue) {
    return StartsWith(pS0, pValue)
           .StartsWith(pS1, pValue);
  }

  template<unsigned int N0, unsigned int N1, unsigned int N2>
  StringSwitch& StartsWith(const char (&pS0)[N0],
                           const char (&pS1)[N1],
                           const char (&pS2)[N2],
                           const T& pValue) {
    return StartsWith(pS0, pValue)
           .StartsWith(pS1, pValue)
           .StartsWith(pS2, pValue);
  }

  template<unsigned int N0, unsigned int N1, unsigned int N2, unsigned int N3>
  StringSwitch& StartsWith(const char (&pS0)[N0],
                           const char (&pS1)[N1],
                           const char (&pS2)[N2],
                           const char (&pS3)[N3],
                           const T& pValue) {
    return StartsWith(pS0, pValue)
           .StartsWith(pS1, pValue)
           .StartsWith(pS2, pValue)
           .StartsWith(pS3, pValue);
  }

  template<unsigned int N>
  StringSwitch& EndsWith(const char (&pS)[N], const T& pValue) {
    if ((NULL == m_pResult) &&
        (N - 1 <= m_Str.size())  &&
        (0 == std::memcmp(pS, m_Str.data() + m_Str.size() + 1 - N, N-1))) {
      m_pResult = &pValue;
    }
    return *this;
  }

  template<unsigned int N0, unsigned int N1>
  StringSwitch& EndsWith(const char (&pS0)[N0],
                         const char (&pS1)[N1],
                         const T& pValue) {
    return EndsWith(pS0, pValue)
           .EndsWith(pS1, pValue);
  }

  template<unsigned int N0, unsigned int N1, unsigned int N2>
  StringSwitch& EndsWith(const char (&pS0)[N0],
                         const char (&pS1)[N1],
                         const char (&pS2)[N2],
                         const T& pValue) {
    return EndsWith(pS0, pValue)
           .EndsWith(pS1, pValue)
           .EndsWith(pS2, pValue);
  }

  template<unsigned int N0, unsigned int N1, unsigned int N2, unsigned int N3>
  StringSwitch& EndsWith(const char (&pS0)[N0],
                         const char (&pS1)[N1],
                         const char (&pS2)[N2],
                         const char (&pS3)[N3],
                         const T& pValue) {
    return EndsWith(pS0, pValue)
           .EndsWith(pS1, pValue)
           .EndsWith(pS2, pValue)
           .EndsWith(pS3, pValue);
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
  StringRef m_Str;

  /// The pointer to the result of this switch statement, once known,
  /// null before that.
  const T* m_pResult;
};

} // namespace of onnc

#endif

