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
// Non-member functions
//===----------------------------------------------------------------------===//
static const std::string *GetRaw(const BM188X::Conv& pConv, unsigned int pIdx)
{
  switch (pIdx) {
    case 0: return &pConv.getInput(0)->adaptee()->raw();
    case 1: return &pConv.getInput(1)->adaptee()->raw();
    case 2: return &pConv.getOutput(0)->adaptee()->raw();
    case 3: return &pConv.getInput(2)->adaptee()->raw();
    case 4: return &pConv.getInput(3)->adaptee()->raw();
    case 5: return &pConv.getInput(4)->adaptee()->raw();
  }
  return nullptr;
}

static const onnx::Tensor *
GetTensor(const BM188X::SlicedConv& pConv, unsigned int pIdx)
{
  switch(pIdx) {
    case 0: return pConv.getInput(0)->adaptee();
    case 1: return pConv.getInput(1)->adaptee();
    case 2: return pConv.getOutput(0)->adaptee();
    case 3: return pConv.getInput(2)->adaptee();
  }
  return nullptr;
}

//===----------------------------------------------------------------------===//
// FillWeightVisitor
//===----------------------------------------------------------------------===//
FillWeightVisitor::FillWeightVisitor(GenWeightPass::WeightType& pWeight)
    : m_Weight(pWeight), m_DoneOpndSet()
{
}

void FillWeightVisitor::visit(const onnc::Initializer &pOp)
{
  const onnc::Value* value = pOp.getOutput<onnc::Value>();
  const std::string& raw = value->adaptee()->raw();

  if (onnc::Value::kInt8 == value->kind())
    Append8bit(m_Weight, raw);
  else
    Append16bit(m_Weight, raw);
}

// visit TGConv
void FillWeightVisitor::visit(const BM188X::Conv& pConv)
{
  Weight weight;
  weight.resize(onnc::getTotalCount(pConv.getInput(1)->adaptee()->sizes()));

  int ks = pConv.getKernelShape().at(0) * pConv.getKernelShape().at(1);
  int ic = pConv.getInput(0)->adaptee()->sizes().at(1) / pConv.getGroup();
  int oc = pConv.getInput(1)->adaptee()->sizes().at(0);
  const std::string& raw = pConv.getInput(1)->adaptee()->raw();
  Convert(weight, raw, ks, ic, oc);

  // 16bit bias
  if (1 == pConv.getDoBias())
    Append16bit(weight, *GetRaw(pConv, pConv.getBiasIdx()));

  // 8bit scale bias
  if (1 == pConv.getDoScale())
    Append8bit(weight, *GetRaw(pConv, pConv.getScaleIdx()));

  // 16bit scale bias
  if (1 == pConv.getDoScaleBias())
    Append16bit(weight, *GetRaw(pConv, pConv.getScaleBiasIdx()));

  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}

// visit TLConv
void FillWeightVisitor::visit(const BM188X::SlicedConv& pConv)
{
  Weight weight;
  const onnx::Tensor* tensor = pConv.getInput(1)->adaptee();
  if (!isWritten(*tensor)) {
    setWritten(*tensor);
    weight.resize(onnc::getTotalCount(tensor->sizes()));

    int ks = pConv.getKernelShape().at(0) * pConv.getKernelShape().at(1);
    int ic = pConv.getInDim().at(1) / pConv.getGroups();
    int oc = pConv.getOutDim().at(1);
    Convert(weight, tensor->raw(), ks, ic, oc);
  }

  if (1 == pConv.getDoBias()) {
    const onnx::Tensor* tensor = GetTensor(pConv, pConv.getBiasIdx());
    if (!isWritten(*tensor)) {
      setWritten(*tensor);
      Append16bit(weight, tensor->raw());
    }
  }

  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}

//===----------------------------------------------------------------------===//
// FillWeightVisitor support members
//===----------------------------------------------------------------------===//
void FillWeightVisitor::Convert(Weight& pWeight, const std::string& pRaw,
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

void FillWeightVisitor::Append8bit(Weight& pW, const std::string &pRaw)
{
  std::copy(pRaw.begin(), pRaw.end(), std::back_inserter(pW));
}

void FillWeightVisitor::Append16bit(Weight& pW, const std::string &pRaw)
{
  size_t count = pRaw.size();
  size_t offset = pW.size();
  pW.resize(offset + count * 2);
  for (size_t i = 0; i*2 < count; ++i) {
    pW[offset + i] = (int8_t)pRaw[i*2];
    pW[offset + i + count] = (int8_t)pRaw[i*2 + 1];
  }
}

bool FillWeightVisitor::isWritten(const onnx::Tensor &pOpnd) const
{
  return (m_DoneOpndSet.end() != m_DoneOpndSet.find(&pOpnd));
}

void BM188X::FillWeightVisitor::setWritten(const onnx::Tensor &pOpnd)
{
  m_DoneOpndSet.insert(&pOpnd);
}
