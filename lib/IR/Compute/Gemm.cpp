//===- Gemm.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Gemm.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Gemm
//===----------------------------------------------------------------------===//
Gemm::Gemm()
  : ComputeOperator("Gemm"),
    m_Alpha(),
    m_Beta(),
    m_Broadcast(),
    m_TransA(),
    m_TransB() {
}

Gemm::Gemm(const FloatAttr& pAlpha,
           const FloatAttr& pBeta,
           const IntAttr& pBroadcast,
           const IntAttr& pTransA,
           const IntAttr& pTransB)
  : ComputeOperator("Gemm"),
    m_Alpha(pAlpha),
    m_Beta(pBeta),
    m_Broadcast(pBroadcast),
    m_TransA(pTransA),
    m_TransB(pTransB) {
}

void Gemm::print(std::ostream& pOS) const
{
}
