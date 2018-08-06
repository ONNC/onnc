//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#ifndef ONNC_TARGET_SOPHON_PATTERNMATCH_H
#define ONNC_TARGET_SOPHON_PATTERNMATCH_H
#include <onnc/Config/ONNX.h>
#include <string>
#include <type_traits>

namespace onnc {
namespace PatternMatch {

template <typename Val, typename Pattern>
bool match(const Val *pV, const Pattern &pP)
{
  if (pV == nullptr)
    return false;
  return const_cast<Pattern &>(pP).match(pV);
}

template <typename Val, typename Pattern, typename... Patterns>
bool match(const Val *pV, const Pattern &pP, const Patterns &... pPs)
{
  return match(pV, pP) and match(pV, pPs...);
}

struct matchSymbol {
  xSymbol m_Symbol;
  matchSymbol(const std::string &pSym) : m_Symbol(pSym) {}
  bool match(const xNode *pN) { return pN->kind() == m_Symbol; }
};
inline matchSymbol mSymbol(const std::string &pSym) { return pSym; }

inline xNode *next(xNode *pN)
{
  if (pN->outputs().size() != 1)
    return nullptr;
  if (pN->output()->uses().size() != 1)
    return nullptr;
  return pN->output()->uses()[0].user;
}

inline xNode *input(xNode *pN, size_t pIndex)
{
  if (pN->inputs()[pIndex]->uses().size() != 1)
    return nullptr;

  return pN->inputs()[pIndex]->node();
}

/// Matching combinators
template <typename LTy, typename RTy> struct match_combine_or {
  LTy m_L;
  RTy m_R;

  match_combine_or(const LTy &pLeft, const RTy &pRight)
      : m_L(pLeft), m_R(pRight)
  {
  }

  template <typename ITy> bool match(const ITy *pV)
  {
    return m_L.match(pV) or m_R.match(pV);
  }
};

template <typename LTy, typename RTy> struct match_combine_and {
  LTy m_L;
  RTy m_R;

  match_combine_and(const LTy &pLeft, const RTy &pRight)
      : m_L(pLeft), m_R(pRight)
  {
  }

  template <typename ITy> bool match(const ITy *pV)
  {
    return m_L.match(pV) and m_R.match(pV);
  }
};

/// Combine two pattern matchers matching L || R
template <typename LTy, typename RTy>
inline match_combine_or<LTy, RTy> m_CombineOr(const LTy &pL, const RTy &pR)
{
  return match_combine_or<LTy, RTy>(pL, pR);
}

/// Combine two pattern matchers matching L && R
template <typename LTy, typename RTy>
inline match_combine_and<LTy, RTy> m_CombineAnd(const LTy &pL, const RTy &pR)
{
  return match_combine_and<LTy, RTy>(pL, pR);
}

// Attribute
template <typename T> struct matchAttr {
  static_assert(sizeof(T) == -1, "error: unsupported type");
};
#define CREATE_ACCESSOR_BASE(Kind, method, Kind_T)                             \
  template <> struct matchAttr<Kind_T> {                                       \
    std::string m_Name;                                                        \
    x##Kind##Attr::ValueType m_Value;                                          \
    matchAttr(const std::string &pName, Kind_T pV)                             \
        : m_Name(pName), m_Value(pV)                                           \
    {                                                                          \
    }                                                                          \
    bool match(const xNode *pN) const                                          \
    {                                                                          \
      auto s = xSymbol(m_Name);                                                \
      if (not pN->hasAttribute(s))                                             \
        return false;                                                          \
      if (pN->method(s) != m_Value)                                            \
        return false;                                                          \
      return true;                                                             \
    }                                                                          \
  };
#define CREATE_ACCESSOR(Kind, method)                                          \
  CREATE_ACCESSOR_BASE(Kind, method, x##Kind##Attr::ValueType)
CREATE_ACCESSOR(Float, f)
CREATE_ACCESSOR(Floats, fs)
CREATE_ACCESSOR(String, s)
CREATE_ACCESSOR(Strings, ss)
CREATE_ACCESSOR(Int, i)
CREATE_ACCESSOR(Ints, is)
CREATE_ACCESSOR(Graph, g)
CREATE_ACCESSOR(Graphs, gs)

CREATE_ACCESSOR_BASE(Float, f, float)
CREATE_ACCESSOR_BASE(String, s, const char *)
CREATE_ACCESSOR_BASE(Int, i, int32_t)

#undef CREATE_ACCESSOR_BASE
#undef CREATE_ACCESSOR
template <typename T> matchAttr<T> mAttr(const std::string &pName, T pV)
{
  return matchAttr<T>(pName, pV);
}
struct matchNoAttr {
  std::string m_Name;
  matchNoAttr(const std::string &pName) : m_Name(pName) {}
  bool match(const xNode *pN) const
  {
    return not pN->hasAttribute(xSymbol(m_Name));
  }
};
inline matchNoAttr mNoAttr(const std::string &pName)
{
  return matchNoAttr(pName);
}

inline match_combine_or<matchNoAttr, matchAttr<xIntAttr::ValueType> >
mFalseAttr(const std::string &pName)
{
  return m_CombineOr(mNoAttr(pName), mAttr<xIntAttr::ValueType>(pName, 0));
}
inline matchAttr<xIntAttr::ValueType> mTrueAttr(const std::string &pName)
{
  return mAttr<xIntAttr::ValueType>(pName, 1);
}

} // namespace PatternMatch
} // namespace onnc

#endif /* PATTERNMATCH_H */
