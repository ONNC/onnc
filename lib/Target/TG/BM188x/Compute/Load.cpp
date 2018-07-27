//===- Load.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Load.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "tl_load"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// Load
//===----------------------------------------------------------------------===//
BM188X::Load::Load(const IntAttr &pSrcGOffset,
                   const IntAttr &pDstLAddr,
                   const BoolAttr &pDoTranspose,
                   const BoolAttr &pIsAligned,
                   const BoolAttr &pIsNeuron,
                   const IntsAttr &pLocalDim,
                   const IntsAttr &pGlobalDim,
                   const StringAttr &pSplitName)
    : ComputeOperator("TLLoad"),
      m_SrcGOffset(pSrcGOffset),
      m_DstLAddr(pDstLAddr),
      m_DoTranspose(pDoTranspose),
      m_IsAligned(pIsAligned),
      m_IsNeuron(pIsNeuron),
      m_LocalDim(pLocalDim),
      m_GlobalDim(pGlobalDim),
      m_SplitName(pSplitName)
{
}

void BM188X::Load::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Load::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Load::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

