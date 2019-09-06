//===- NvDlaShuffle.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaShuffle.h"

#include "../CodeEmitVisitor.h"
#include "../NvDlaDefine.h"

using namespace onnc;
using namespace onnc::nvdla;

char NvDlaShuffle::ID = 0;

//===----------------------------------------------------------------------===//
// NvDlaShuffle
//===----------------------------------------------------------------------===//
void NvDlaShuffle::printAttributes(std::ostream& pOS) const
{
  pOS << "<axis: " << m_Axis.value() << ", ";
  pOS << "group: " << m_Group.value() << ">";
}

void NvDlaShuffle::accept(ComputeVisitor& pV)
{
  CodeEmitVisitor* visitor = dyn_cast<CodeEmitVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void NvDlaShuffle::accept(ComputeVisitor& pV) const
{
  CodeEmitVisitor* visitor = dyn_cast<CodeEmitVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool NvDlaShuffle::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
