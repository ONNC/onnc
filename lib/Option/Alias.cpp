//===- Alias.cpp ----------------------------------------------------------==//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Option/Alias.h>

using namespace onnc;
using namespace onnc::cl;

//===----------------------------------------------------------------------===//
// class alias
//===----------------------------------------------------------------------===//
void cl::alias::setAliasFor(cl::OptDefs& pOption)
{
  m_pTruth = &pOption;

  // move attributes from true option to the alias.
  this->setValueOccurrence(pOption.getValueOccurrence());
  this->setDelimiter(pOption.getDelimiter());
  if (!this->hasDescription())
    this->setDescription(pOption.getDescription());
  if (!this->hasHelp())
    this->setHelp(pOption.getHelp());
}

void alias::done()
{
  if (!hasArgStr())
    fatal(*this, "cl::alias must have argument name specified!");

  if (NULL == m_pTruth)
    fatal(*this, "must specify the true option by cl::trueopt(...)!");

  RegisterOptDefs(*this);
}
