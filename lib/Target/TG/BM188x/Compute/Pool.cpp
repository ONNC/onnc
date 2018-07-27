//===- Pool.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Pool.h"

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
