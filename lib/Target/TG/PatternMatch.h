#ifndef PATTERNMATCH_H
#define PATTERNMATCH_H

#include <onnx/common/ir.h>
#include <string>

namespace onnc {
namespace PatternMatch {

template <typename Val, typename Pattern> bool match(Val *pV, const Pattern &pP)
{
  if (pV == nullptr)
    return false;
  return pP.match(pV);
}
struct matchSymbol {
  onnx::Symbol m_Symbol;
  matchSymbol(const std::string &pSym) : m_Symbol(pSym) {}
  bool match(onnx::Node *pN) const { return pN->kind() == m_Symbol; }
};
matchSymbol mSymbol(const std::string &pSym) { return matchSymbol(pSym); }

onnx::Node *next(onnx::Node *pN)
{
  if (pN->outputs().size() != 1)
    return nullptr;
  if (pN->output()->uses().size() != 1)
    return nullptr;
  return pN->output()->uses()[0].user;
}

} // namespace PatternMatch
} // namespace onnc

#endif /* PATTERNMATCH_H */
