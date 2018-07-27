//===- Pool.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Pool.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "tl_pool"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// Pool
//===----------------------------------------------------------------------===//
Pool::Pool(const IntAttr& pIFmapAddr,
           const IntAttr& pOFmapAddr,
           const IntsAttr& pInDim,
           const IntsAttr& pOutDim,
           bool pIsAvgPooling,
           const StringAttr& pSpliteName)
    :  ComputeOperator("TLPool"), m_IFmapAddr(pIFmapAddr),
       m_OFmapAddr(pOFmapAddr), m_InDim(pInDim), m_OutDim(pOutDim),
       m_IsAvgPooling(pIsAvgPooling), m_SplitName(pSpliteName)
{
}

void BM188X::Pool::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Pool::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Pool::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}
