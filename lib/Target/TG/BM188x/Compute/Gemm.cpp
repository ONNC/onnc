//===- Gemm.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Gemm.h"
#include "PatternMatch.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Debug.h>

#define DEBUG_TYPE "tg_gemm"

using namespace onnc;
using namespace onnc::BM188X;

namespace pm = onnc::PatternMatch;
char BM188X::Gemm::ID = 0;

//===----------------------------------------------------------------------===//
// Gemm
//===----------------------------------------------------------------------===//
BM188X::Gemm::Gemm()
    : onnc::Gemm(), m_InRowNum(0),
      m_InColNum(0), m_OutColNum(0), m_HaveBias(0), m_WeightTp(false),
      m_EnableRelu(false), m_RShiftWidth(0)
{
  setID(ID);
}

BM188X::Gemm::~Gemm()
{
}

void BM188X::Gemm::init(const ::onnx::Node &pNode)
{
  auto &aDim = pNode.inputs()[0]->sizes();
  auto &bDim = pNode.outputs()[0]->sizes();
  m_InRowNum = aDim[0].dim;
  m_InColNum = aDim[1].dim;
  if (aDim.size() == 4)
    m_InColNum *= aDim[2].dim * aDim[3].dim;
  m_OutColNum = bDim[1].dim;
  m_HaveBias = true;

  if (pNode.hasAttribute(::onnx::Symbol("transB"))) {
    auto transB = pNode.i(::onnx::Symbol("transB"));
    DEBUG(dbgs() << "transB:" << transB << std::endl;);
    m_WeightTp = true;
  }
  
  m_EnableRelu = pm::match(&pNode, pm::mTrueAttr("do_relu"));
}

void BM188X::Gemm::print(std::ostream &pOS) const
{
  // TODO
}

void BM188X::Gemm::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Gemm::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::Gemm::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
