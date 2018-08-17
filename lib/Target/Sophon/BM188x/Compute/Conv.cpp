//===- Conv.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Conv.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "tg_conv"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::Conv::ID = 0;

//===----------------------------------------------------------------------===//
// Conv
//===----------------------------------------------------------------------===//
BM188X::Conv::Conv()
    : onnc::Conv(), m_RShiftWidth(0), m_ScaleRShiftWidth(0),
      m_ConvOutputThreshold(0.f), m_DoRelu(false)
{
  setID(ID);
}

BM188X::Conv::~Conv()
{
}

void BM188X::Conv::print(std::ostream& pOS) const
{
  pOS << m_Outputs[0]->getName() << " = " << name();
  for (unsigned int i = 0; i < m_Inputs.size(); ++i) {
    pOS << " " << m_Inputs[i]->getName();
  }
}

void BM188X::Conv::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Conv::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::Conv::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
