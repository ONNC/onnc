//===- Gemm.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Gemm.h>

using namespace onnc;

char Gemm::ID = 0;

//===----------------------------------------------------------------------===//
// Gemm
//===----------------------------------------------------------------------===//
Gemm::Gemm()
  : ComputeOperator("Gemm", ID),
    m_Alpha(1.0),
    m_Beta(1.0),
    m_Broadcast(),
    m_TransA(),
    m_TransB() {
}

Gemm::Gemm(const FloatAttr& pAlpha,
           const FloatAttr& pBeta,
           const IntAttr& pBroadcast,
           const IntAttr& pTransA,
           const IntAttr& pTransB)
  : ComputeOperator("Gemm", ID),
    m_Alpha(pAlpha),
    m_Beta(pBeta),
    m_Broadcast(pBroadcast),
    m_TransA(pTransA),
    m_TransB(pTransB) {
}

void Gemm::print(std::ostream& pOS) const
{
}

bool Gemm::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
