//===- Store.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Store.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Casting.h>
#define DEBUG_TYPE "tl_store"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::Store::ID = 0;

//===----------------------------------------------------------------------===//
// Store
//===----------------------------------------------------------------------===//
BM188X::Store::Store(const IntAttr& pDstGOffset,
             const IntAttr& pSrcLAddr,
             const BoolAttr& pDoTranspose,
             const BoolAttr& pIsAligned,
             const BoolAttr& pIsNeuron,
             const IntsAttr& pLocalDim,
             const IntsAttr& pGlobalDim,
             const StringAttr& pSplitName)
    : ComputeOperator("Store", ID), m_DstGOffset(pDstGOffset),
      m_SrcLAddr(pSrcLAddr), m_DoTranspose(pDoTranspose),
      m_IsAligned(pIsAligned), m_IsNeuron(pIsNeuron), m_LocalDim(pLocalDim),
      m_GlobalDim(pGlobalDim), m_SplitName(pSplitName)
{
}

void BM188X::Store::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Store::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Store::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::Store::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
