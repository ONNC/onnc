//===- Rope.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_ROPE_H
#define ONNC_ADT_ROPE_H
#include <onnc/Support/DataTypes.h>
#include <string>
#include <iosfwd>

namespace onnc {

class StringRef;
class Path;

/** \class Rope
 *  \brief Ropes are scalable string implementation: representing the
 *  concatenation of temporary values of strings.
 *
 *  Like StringRef, Rope does not own the string data. It is expected
 *  to be used in situations where the character data resides in some other
 *  buffer, whose life time is longer than Rope.
 *
 *  @note Don't modify the string data by Rope.
 */
class Rope
{
public:
  Rope();

  Rope(const char* pStr);

  Rope(const std::string& pStr);

  Rope(const Path& pPath);

  Rope(const StringRef& pStr);

  explicit Rope(char pVal);
  explicit Rope(signed char pValue);   ///< Conform to all ABIs
  explicit Rope(unsigned char pValue); ///< Conform to all ABIs
  explicit Rope(unsigned int pValue);
  explicit Rope(int pValue);
  explicit Rope(const unsigned long& pValue);
  explicit Rope(const long& pValue);
  explicit Rope(const unsigned long long& pValue);
  explicit Rope(const long long& pValue);

  // Efficient constructors for concatenation
  explicit Rope(const char* pLHS, const StringRef& pRHS);

  // Efficient constructors for concatenation
  explicit Rope(const StringRef& pLHS, const char* pRHS);

  Rope concat(const Rope& pSuffix) const;

  std::string str() const;

  void toStr(std::ostream& pOS) const;

private:
  enum Kind {
    /// An empty string; the result of concatenating anything with it is also
    /// empty.
    NullKind,

    /// The empty string.
    EmptyKind,

    /// A pointer to a Rope instance. `const Rope*`
    RopeKind,

    /// A pointer to a C string instance. `const char*`
    CStringKind,

    /// A pointer to an std::string instance. `const std::string*`
    StdStringKind,

    /// A pointer to a StringRef instance. `const StringRef*`
    StringRefKind,

    /// A pointer to a Path instance. `const Path*`
    PathKind,

    /// A char value reinterpreted as a pointer, to render as a character.
    /// `char`
    CharKind,

    /// An int value reinterpreted as a pointer, to render as a signed
    /// decimal integer. `int`
    IntKind,

    /// An unsigned int value reinterpreted as a pointer, to render as an
    /// unsigned decimal integer. `unsigned int`
    UIntKind,

    /// A pointer to unsigned long value, to render as an unsigned decimal
    /// integer. `const unsigned long*`
    ULongKind,

    /// A pointer to long value, to render as a signed decimal integer.
    /// `const long*`
    LongKind,

    /// A pointer to an unsigned long long value, to render as an unsigned
    /// decimal integer. `const unsigned long long*`
    ULongLongKind,

    /// A pointer to a long long value, to render as a signed decimal integer.
    /// `const long long*`
    LongLongKind
  };

  struct Child
  {
    uint8_t kind;
    intptr_t data;

    Child() : kind(0), data(0) { }
    Child(uint8_t pKind) : kind(pKind), data(0) { }

    Kind getKind() const { return (Kind) kind; }

    Rope* rope() const;

    void toStr(std::ostream& pOS) const;
  };

private:
  explicit Rope(Kind pKind);

  explicit Rope(const Rope& pLHS, const Rope& pRHS);

  explicit Rope(const Child& pLHS, const Child& pRHS);

  bool isNull() const;
  bool isEmpty() const;
  bool isNullary() const;
  bool isUnary() const;
  bool isBinary() const;
  bool isValid() const;

private:
  /// Left hand side child. The prefix in the concatenation.
  Child m_LHS;

  /// Right hand side child. The suffix in the concatenation.
  Child m_RHS;
};

//===----------------------------------------------------------------------===//
// Operators
//===----------------------------------------------------------------------===//
inline Rope operator+(const Rope& pLHS, const Rope& pRHS)
{
  return pLHS.concat(pRHS);
}

inline Rope operator+(const char* pLHS, const StringRef& pRHS)
{
  return Rope(pLHS, pRHS);
}

inline Rope operator+(const StringRef& pLHS, const char* pRHS)
{
  return Rope(pLHS, pRHS);
}
//===----------------------------------------------------------------------===//
// Inline member function body
//===----------------------------------------------------------------------===//
/// isNull - Check for the null twine.
inline bool Rope::isNull() const
{
  return NullKind == m_LHS.getKind();
}

/// isEmpty - Check for the empty twine.
inline bool Rope::isEmpty() const
{
  return EmptyKind == m_LHS.getKind();
}

/// isNullary - Check if this is a nullary twine (null or empty).
inline bool Rope::isNullary() const
{
  return isNull() || isEmpty();
}

/// isUnary - Check if this is a unary twine.
inline bool Rope::isUnary() const
{
  return (EmptyKind == m_RHS.getKind()) && !isNullary();
}

/// isBinary - Check if this is a binary twine.
inline bool Rope::isBinary() const
{
  return (NullKind != m_LHS.getKind()) && (EmptyKind != m_RHS.getKind());
}

/// isValid - Check if this is a valid twine (satisfying the invariants on
/// order and number of arguments).
inline bool Rope::isValid() const
{
  // Nullary twines always have Empty on the RHS.
  if (isNullary() && m_RHS.getKind() != EmptyKind)
    return false;

  // Null should never appear on the RHS.
  if (NullKind == m_RHS.getKind())
    return false;

  // The RHS cannot be non-empty if the LHS is empty.
  if (EmptyKind != m_RHS.getKind() && EmptyKind == m_LHS.getKind())
    return false;

  // A twine child should always be binary.
  if (RopeKind == m_LHS.getKind() && !m_LHS.rope()->isBinary())
    return false;
  if (RopeKind == m_RHS.getKind() && !m_RHS.rope()->isBinary())
    return false;

  return true;
}

inline Rope* Rope::Child::rope() const
{
  return (Rope*)data;
}

} // namespace of onnc

namespace std {

inline ostream& operator<<(ostream& pOS, const onnc::Rope& pRope)
{
  pRope.toStr(pOS);
  return pOS;
}

} // namespace std
#endif
