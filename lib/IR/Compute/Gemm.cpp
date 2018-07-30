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
    m_TransA(0),
    m_TransB(0) {
}

Gemm::Gemm(const FloatAttr& pAlpha,
           const FloatAttr& pBeta,
           const IntAttr& pTransA,
           const IntAttr& pTransB)
  : ComputeOperator("Gemm", ID),
    m_Alpha(pAlpha),
    m_Beta(pBeta),
    m_TransA(pTransA),
    m_TransB(pTransB) {
}

Gemm::Gemm(const Gemm& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()),
    m_Beta(pCopy.getBeta()),
    m_TransA(pCopy.getTransA()),
    m_TransB(pCopy.getTransB()) {
}

void Gemm::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ", " << getBeta() << ", " << getTransA() << ", " << getTransB() << ">";
}

bool Gemm::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
