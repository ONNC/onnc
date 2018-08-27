//===- FillWeightVisitor.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "FillWeightVisitor.h"
#include "Compute/BMScale.h"
#include "Compute/Conv.h"
#include "Compute/Gemm.h"
#include "Compute/LRN.h"
#include "Compute/PRelu.h"
#include "Compute/SlicedConv.h"
#include <onnc/IR/Compute/Initializer.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

static const std::vector<int8_t> &getInt8Data(const onnc::Value *pValue)
{
  auto *tensor = dynamic_cast<const onnc::Int8Tensor *>(pValue);
  return tensor->getValues();
}

static const std::vector<int16_t> &getInt16Data(const onnc::Value *pValue)
{
  auto *tensor = dynamic_cast<const onnc::Int16Tensor *>(pValue);
  return tensor->getValues();
}

//===----------------------------------------------------------------------===//
// FillWeightVisitor
//===----------------------------------------------------------------------===//
FillWeightVisitor::FillWeightVisitor(GenWeightPass::WeightType &pWeight)
    : m_Weight(pWeight), m_DoneOpndSet()
{
}

// visit TGConv
void FillWeightVisitor::visit(const BM188X::Conv &pConv)
{
  Weight weight;

  auto *input = dynamic_cast<const onnc::Int8Tensor *>(pConv.getInput(0));
  auto *int8_weight = dynamic_cast<const onnc::Int8Tensor *>(pConv.getInput(1));
  assert(input->getNumOfDimensions() == 4);
  assert(int8_weight->getNumOfDimensions() == 4);

  std::vector<int8_t> weight_data = int8_weight->getValues();
  assert(weight_data.size() != 0);
  weight.resize(weight_data.size());

  // ks is kh * kw
  int ks = pConv.getKernelShape().at(0) * pConv.getKernelShape().at(1);
  int ic = input->dimension(1) / pConv.getGroup();
  int oc = int8_weight->dimension(0);
  Convert(weight, weight_data, ks, ic, oc);

  // 16bit bias
  if (pConv.isDoBias()) {
    const std::vector<int16_t> data = getInt16Data(pConv.getInput(2));
    Append16bit(weight, data);
  }

  // 8bit scale
  if (pConv.isDoScale()) {
    const std::vector<int8_t> data = getInt8Data(pConv.getScale());
    Append8bit(weight, data);
  }

  // 16bit scale bias
  if (pConv.isDoScaleBias()) {
    const std::vector<int16_t> data = getInt16Data(pConv.getScaleBias());
    Append16bit(weight, data);
  }

  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}

// visit TLConv
void FillWeightVisitor::visit(const BM188X::SlicedConv &pConv)
{
  Weight weight;
  auto *int8_weight = dynamic_cast<const onnc::Int8Tensor *>(pConv.getInput(1));
  assert(int8_weight->getNumOfDimensions() == 4);

  if (!isWritten(*int8_weight)) {
    setWritten(*int8_weight);

    std::vector<int8_t> weight_data = int8_weight->getValues();
    assert(weight_data.size() != 0);
    weight.resize(weight_data.size());

    auto *input = dynamic_cast<const onnc::Int8Tensor *>(pConv.getInput(0));
    assert(input->getNumOfDimensions() == 4);

    int ks = pConv.getKernelShape().at(0) * pConv.getKernelShape().at(1);
    int ic = input->dimension(1) / pConv.getGroups();
    int oc = int8_weight->dimension(0);
    Convert(weight, weight_data, ks, ic, oc);
  }

  if (pConv.getDoBias() && !isWritten(*pConv.getInput(2))) {
    auto *int16_weight =
        dynamic_cast<const onnc::Int16Tensor *>(pConv.getInput(2));
    setWritten(*int16_weight);
    const std::vector<int16_t> weight_data = int16_weight->getValues();
    Append16bit(weight, weight_data);
  }
  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}

void FillWeightVisitor::visit(const BM188X::Gemm &pGemm)
{
  const std::vector<int8_t> weight_data = getInt8Data(pGemm.getInput(1));
  Append8bit(m_Weight, weight_data);
  const std::vector<int16_t> bias_data = getInt16Data(pGemm.getInput(2));
  Append16bit(m_Weight, bias_data);
}

void FillWeightVisitor::visit(const BM188X::LRN &pLRN)
{
  const std::vector<int8_t> sqrlut_data = getInt8Data(pLRN.getInput(1));
  Append8bit(m_Weight, sqrlut_data);
  const std::vector<int8_t> powerlut_data = getInt8Data(pLRN.getInput(2));
  Append8bit(m_Weight, powerlut_data);
}

void FillWeightVisitor::visit(const BM188X::PRelu &pPRelu)
{
  const std::vector<int8_t> slope_data = getInt8Data(pPRelu.getInput(1));
  Append8bit(m_Weight, slope_data);
}

void FillWeightVisitor::visit(const BM188X::BMScale &pBMScale)
{
  const std::vector<int8_t> weight_data = getInt8Data(pBMScale.getInput(1));
  Append8bit(m_Weight, weight_data);
  const std::vector<int16_t> bias_data = getInt16Data(pBMScale.getInput(2));
  Append16bit(m_Weight, bias_data);
}

//===----------------------------------------------------------------------===//
// FillWeightVisitor support members
//===----------------------------------------------------------------------===//
void FillWeightVisitor::Convert(Weight &pWeight,
                                const std::vector<int8_t> &pData, int pKS,
                                int pIC, int pOC)
{
  // pKS is kh*kw
  // conv weight is arranged by (1, oc, kh*kw, ic)
  // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
  for (int oc_i = 0; oc_i < pOC; ++oc_i) {
    for (int k_i = 0; k_i < pKS; ++k_i) {
      for (int ic_i = 0; ic_i < pIC; ++ic_i) {
        int to = oc_i * (pKS * pIC) + k_i * pIC + ic_i;
        int from = oc_i * (pKS * pIC) + ic_i * pKS + k_i;
        pWeight[to] = pData[from];
      }
    }
  }
}

void FillWeightVisitor::Append8bit(Weight &pW, const std::vector<int8_t> &pData)
{
  std::copy(pData.begin(), pData.end(), std::back_inserter(pW));
}

void FillWeightVisitor::Append16bit(Weight &pW,
                                    const std::vector<int16_t> &pData)
{
  size_t count = pData.size();
  size_t offset = pW.size();
  pW.resize(offset + count * 2);
  for (size_t i = 0; i < count; ++i) {
    pW[offset + i] = (int8_t)(pData[i] & 0xff);
    pW[offset + i + count] = (int8_t)(pData[i] >> 8 & 0xff);
  }
}

bool FillWeightVisitor::isWritten(const onnc::Value &pValue) const
{
  return (m_DoneOpndSet.end() != m_DoneOpndSet.find(&pValue));
}

void BM188X::FillWeightVisitor::setWritten(const onnc::Value &pValue)
{
  m_DoneOpndSet.insert(&pValue);
}
