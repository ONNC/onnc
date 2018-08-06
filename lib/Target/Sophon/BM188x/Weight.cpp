//===- Weight.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Weight.h"
#include <onnc/Target/Sophon/io.hpp>
#include <assert.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// Weight
//===----------------------------------------------------------------------===//
static void Convert(Weight::WeightType& pWeight, const std::string& pRaw,
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

void
BM188X::Weight::append8bit(WeightType& pThis, const std::string& pRaw)
{
  std::copy(pRaw.begin(), pRaw.end(), std::back_inserter(pThis));
}

void
BM188X::Weight::append16bit(WeightType& pThis, const std::string &pRaw)
{
  size_t count = pRaw.size();
  size_t offset = pThis.size();
  pThis.resize(offset + count * 2);
  for (size_t i = 0; i*2 < count; ++i) {
    pThis[offset + i] = (int8_t)pRaw[i*2];
    pThis[offset + i + count] = (int8_t)pRaw[i*2 + 1];
  }
}

void BM188X::Weight::prepareWeight(TGBackend::Instructions& pInstructions,
                                   TGBackend::MemOperands& pMemOperands)
{
  // calculate weight size
  size_t weight_size = 0;
  for (auto *mem_op : pMemOperands) {
    if (MemType::WEIGHT == mem_op->m_MemType) {
      weight_size += mem_op->m_Size;
    }
  }

  m_Weight.reserve(weight_size);

  // TODO save the weight on the address of MemOperand
  for (auto &inst : pInstructions) {
    if (inst->getTypeName() == "TLLoad") {
      continue;
    }

    if (inst->getTypeName() == "TLStore") {
      continue;
    }

    // handle special case
    if (inst->getTypeName() == "Conv") {
      auto *tgconv = dynamic_cast< ::onnc::BM188X::TGConv *>(inst.get());
      prepareWeight(*tgconv);
      continue;
    }

    if (inst->getTypeName() == "TLConv") {
      std::vector<DataType> weight;
      auto *tlconv = dynamic_cast< ::onnc::BM188X::TLConv *>(inst.get());
      prepareWeight(*tlconv);
      continue;
    }

    // for those instruction not TLLoad and TLStore
    for (auto *mem_op : inst->getMemOperands()) {
      if (MemType::WEIGHT == mem_op->m_MemType) {
        const xTensor &tensor =
            onnc::getTensor(mem_op->m_Value->uniqueName(),
                            *mem_op->m_Value->owningGraph());
        if (mem_op->m_Type == onnc::Value::kInt8) {
          append8bit(m_Weight, tensor.raw());
        } else {
          append16bit(m_Weight, tensor.raw());
        }
      }
    } // for each mem operand
  } // for each instruction
}

void BM188X::Weight::genWeightBin(const std::string &pOutputFilename,
                                  TGBackend::Instructions& pInstructions,
                                  TGBackend::MemOperands& pMemOperands)
{
  prepareWeight(pInstructions, pMemOperands);
  if (!m_Weight.empty())
    bmnet::WriteInt8DataToBinaryFile(&m_Weight, pOutputFilename.c_str());
}

bool BM188X::Weight::isWritten(const MemOperand* pOpnd) const
{
  return (m_DoneOpndSet.end() != m_DoneOpndSet.find(pOpnd));
}

void BM188X::Weight::setWritten(const MemOperand* pOpnd)
{
  m_DoneOpndSet.insert(pOpnd);
}

void Weight::prepareWeight(const TGConv& pTGConv)
{
  const xValue *value = pTGConv.getMemOperand(1)->m_Value;
  const xTensor &tensor =
        onnc::getTensor(value->uniqueName(), *value->owningGraph());
  assert(pTGConv.getMemOperand(1)->m_Type == onnc::Value::kInt8);
  assert(tensor.is_raw_data());
  const std::string &raw = tensor.raw();
  size_t count = onnc::getTotalCount(tensor.sizes());

  Weight::WeightType weight;
  weight.resize(count);

  // conv weight is arranged by (1, oc, kh*kw, ic)
  // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
  int ks =  pTGConv.getKH() * pTGConv.getKW();
  int ic = pTGConv.getInC() / pTGConv.getGroups();
  int oc = pTGConv.getOutC();
  Convert(weight, raw, ks, ic, oc);

  // 16bit bias
  if (pTGConv.getDoBias() == 1) {
    auto *mem_op = pTGConv.getMemOperand(pTGConv.getBiasIdx());
    const xTensor &tensor =
            onnc::getTensor(mem_op->m_Value->uniqueName(),
                            *mem_op->m_Value->owningGraph());
    Weight::append16bit(weight, tensor.raw());
  }

  // 8bit scale bias
  if (pTGConv.getDoScale() == 1) {
    auto *mem_op = pTGConv.getMemOperand(pTGConv.getScaleIdx());
    const xTensor &tensor =
            onnc::getTensor(mem_op->m_Value->uniqueName(),
                            *mem_op->m_Value->owningGraph());
    Weight::append8bit(weight, tensor.raw());
  }

  // 16bit scale bias
  if (pTGConv.getDoScaleBias() == 1) {
    auto *mem_op = pTGConv.getMemOperand(pTGConv.getScaleBiasIdx());
    const xTensor &tensor =
            onnc::getTensor(mem_op->m_Value->uniqueName(),
                            *mem_op->m_Value->owningGraph());
    Weight::append16bit(weight, tensor.raw());
  }

  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}

void Weight::prepareWeight(const TLConv& pTLConv)
{
  Weight::WeightType weight;
  if (!isWritten(pTLConv.getMemOperand(1))) {
    setWritten(pTLConv.getMemOperand(1));
    const xValue *value = pTLConv.getMemOperand(1)->m_Value;
    const xTensor &tensor =
        onnc::getTensor(value->uniqueName(), *value->owningGraph());
    assert(pTLConv.getMemOperand(1)->m_Type == onnc::Value::kInt8);
    assert(tensor.is_raw_data());
    const std::string &raw = tensor.raw();
    size_t count = onnc::getTotalCount(tensor.sizes());
    weight.resize(count);

    // conv weight is arranged by (1, oc, kh*kw, ic)
    // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
    int ks = pTLConv.getKH() * pTLConv.getKW();
    int ic = pTLConv.getInC() / pTLConv.getGroups();
    int oc = pTLConv.getOutC();
    Convert(weight, raw, ks, ic, oc);
  }

  if (pTLConv.getDoBias() == 1) {
    auto *mem_op = pTLConv.getMemOperand(pTLConv.getBiasIdx());
    if (!isWritten(pTLConv.getMemOperand(pTLConv.getBiasIdx()))) {
      setWritten(pTLConv.getMemOperand(pTLConv.getBiasIdx()));
      const xTensor &tensor =
            onnc::getTensor(mem_op->m_Value->uniqueName(),
                            *mem_op->m_Value->owningGraph());
      Weight::append16bit(weight, tensor.raw());
    }
  }

  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}
