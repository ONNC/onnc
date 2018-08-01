//===- FillWeightVisitor.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "FillWeightVisitor.h"
#include <onnc/IR/Compute/Initializer.h>
#include "Compute/Conv.h"
#include "Compute/SlicedConv.h"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// FillWeightVisitor
//===----------------------------------------------------------------------===//
FillWeightVisitor::FillWeightVisitor(GenWeightPass::WeightType& pWeight)
    : m_Weight(pWeight), m_DoneOpndSet()
{
}

void FillWeightVisitor::visit(const onnc::Initializer &pOp)
{
  // TODO
}

// visit TGConv
void FillWeightVisitor::visit(const BM188X::Conv& pConv)
{
  // get weight value V from pConv.input(1)
  // create a weight matrix W whose size is V's dimension 
  // copy V's value to a data D
  GenWeightPass::WeightType weight;
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}

// visit TLConv
void FillWeightVisitor::visit(const BM188X::SlicedConv& pSlicedConv)
{
  // TODO
}

//===----------------------------------------------------------------------===//
// FillWeightVisitor support members
//===----------------------------------------------------------------------===//
void BM188X::FillWeightVisitor::Convert(Weight& pWeight, const std::string& pRaw,
                      int pKS, int pIC, int pOC)
{
  // conv weight is arranged by (1, oc, kh*kw, ic)
  // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
  for (int oc_i = 0; oc_i < pOC; ++oc_i) {
    for (int k_i = 0; k_i < pKS; ++k_i) {
      for (int ic_i = 0; ic_i < pIC; ++ic_i) {
        int to   = oc_i*pKS*pIC + k_i  * pIC + ic_i;
        int from = oc_i*pKS*pIC + ic_i * pKS + k_i;
        pWeight[to] = (int8_t)pRaw[from];
      }
    }
  }
}

void BM188X::FillWeightVisitor::Append8bit(Weight& pW, const std::string &pRaw)
{
  std::copy(pRaw.begin(), pRaw.end(), std::back_inserter(pW));
}

void BM188X::FillWeightVisitor::Append16bit(Weight& pW, const std::string &pRaw)
{
  size_t count = pRaw.size();
  size_t offset = pW.size();
  pW.resize(offset + count * 2);
  for (size_t i = 0; i*2 < count; ++i) {
    pW[offset + i] = (int8_t)pRaw[i*2];
    pW[offset + i + count] = (int8_t)pRaw[i*2 + 1];
  }
}

bool BM188X::FillWeightVisitor::isWritten(const onnx::Tensor* pOpnd) const
{
  return (m_DoneOpndSet.end() != m_DoneOpndSet.find(pOpnd));
}

void BM188X::FillWeightVisitor::setWritten(const onnx::Tensor* pOpnd)
{
  m_DoneOpndSet.insert(pOpnd);
}
