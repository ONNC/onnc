//===- SlicedConv.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "SlicedConv.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "tl_slicedconv"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::SlicedConv::ID = 0;

//===----------------------------------------------------------------------===//
// SlicedConv
//===----------------------------------------------------------------------===//
SlicedConv::SlicedConv(const IntAttr& pIFmapAddr,
               const IntAttr& pOFmapAddr,
							 const IntAttr& pWeightAddr,
							 const IntAttr& pDoResultAdd,
							 const IntsAttr& pInDim,
							 const IntsAttr& pOutDim,
							 const StringAttr& pSpliteName)
    :  ComputeOperator("SlicedConv", ID), m_IFmapAddr(pIFmapAddr),
       m_OFmapAddr(pOFmapAddr), m_WeightAddr(pWeightAddr),
       m_InDim(pInDim), m_OutDim(pOutDim),
       m_DoRelu(false), m_DoBias(false), m_BiasAddr(0),
       m_SplitName(pSpliteName), m_Groups(0), m_RShiftWidth(0)
{
  m_DoResultAdd = pDoResultAdd.value() ? true : false;
}

void BM188X::SlicedConv::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::SlicedConv::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::SlicedConv::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::SlicedConv::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}