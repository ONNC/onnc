//===- Transpose.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Transpose.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "tg_transpose"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::Transpose::ID = 0;

//===----------------------------------------------------------------------===//
// Transpose
//===----------------------------------------------------------------------===//
BM188X::Transpose::Transpose()
    : onnc::Transpose(), m_W(0),
      m_Order(), m_OutputShape(), m_NeedPermute(false)
{
  setID(ID);
}

BM188X::Transpose::~Transpose()
{
}

void BM188X::Transpose::init(const xNode &pNode)
{
  // error check in TransposeLower.cpp
  auto inDim = pNode.inputs()[0]->sizes();
  auto &orders = pNode.is(xSymbol("perm"));

  // Now, we only support permute layer for ssd, shufflenet and OCR model
  assert(((orders[0] == 0) && (orders[1] == 2) && (orders[2] == 3) &&
          (orders[3] == 1)) ||
         ((orders[0] == 2) && (orders[1] == 3) && (orders[2] == 0) &&
          (orders[3] == 1)) ||
         ((orders[0] == 1) && (orders[1] == 0) && (orders[2] == 2) &&
          (orders[3] == 3)) ||
         ((orders[0] == 0) && (orders[1] == 2) && (orders[2] == 1) &&
          (orders[3] == 3) && (orders[4] == 4)));

  m_W = inDim[3].dim;
  m_Order.resize(orders.size());
  m_OutputShape.resize(inDim.size());

  for (unsigned i = 0; i < orders.size(); ++i) {
    m_Order[i] = orders[i];
    m_OutputShape[i] = inDim[orders[i]].dim;
  }

  if (orders.size() == 5) {
    m_W = inDim[3].dim * inDim[4].dim;
    m_OutputShape[3] = m_OutputShape[3] * m_OutputShape[4];
  }

  // Check if we need to reorder the data or keep it.
  for (unsigned i = 0; i < orders.size(); ++i) {
    if (orders[i] != i) {
      // As long as there is one order which is different from the natural order
      // of the data, we need to permute. Otherwise, we share the data and diff.
      m_NeedPermute = true;
      break;
    }
  }
}

void BM188X::Transpose::print(std::ostream &pOS) const
{
  // TODO
}

void BM188X::Transpose::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Transpose::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::Transpose::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
