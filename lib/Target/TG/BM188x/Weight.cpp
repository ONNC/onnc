//===- Weight.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Weight.h"
#include <onnc/Target/TG/io.hpp>
#include <assert.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// Weight
//===----------------------------------------------------------------------===//
void
BM188X::Weight::prepare8bitWeight(const MemOperand &pMemOp, WeightType& pThis)
{
  const onnx::Tensor &tensor = onnc::getTensor(pMemOp.m_Value->uniqueName(),
                                               *pMemOp.m_Value->owningGraph());

  assert(tensor.is_raw_data());
  const std::string &raw = tensor.raw();
  std::copy(raw.begin(), raw.end(), std::back_inserter(pThis));
}

void
BM188X::Weight::prepare16bitWeight(const MemOperand &pMemOp, WeightType& pThis)
{
  assert(pMemOp.m_Type == onnx::TensorProto_DataType_INT16);
  const onnx::Tensor &tensor = onnc::getTensor(pMemOp.m_Value->uniqueName(),
                                               *pMemOp.m_Value->owningGraph());

  assert(tensor.is_raw_data());
  const std::string &raw = tensor.raw();
  size_t count = onnc::getTotalCount(tensor.sizes());
  std::vector<int16_t> int16_vector(count);
  memcpy(int16_vector.data(), raw.data(), count * sizeof(int16_t));
  size_t offset = pThis.size();
  pThis.resize(offset + count * 2);
  for (size_t i = 0; i < count; ++i) {
    pThis[offset + i] = (int8_t)(int16_vector[i] & 0xff);
    pThis[offset + i + count] = (int8_t)((int16_vector[i] >> 8) & 0xff);
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
        if (mem_op->m_Type == ::onnx::TensorProto_DataType_INT8) {
          prepare8bitWeight(*mem_op, m_Weight);
        } else {
          prepare16bitWeight(*mem_op, m_Weight);
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
  const onnx::Value *value = pTGConv.getMemOperand(1)->m_Value;
  const onnx::Tensor &tensor =
        onnc::getTensor(value->uniqueName(), *value->owningGraph());
  assert(pTGConv.getMemOperand(1)->m_Type == onnx::TensorProto_DataType_INT8);
  assert(tensor.is_raw_data());
  const std::string &raw = tensor.raw();
  size_t count = onnc::getTotalCount(tensor.sizes());

  Weight::WeightType weight;
  weight.resize(count);

  // conv weight is arranged by (1, oc, kh*kw, ic)
  // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
  int ic = pTGConv.getInC() / pTGConv.getGroups();
  for (int oc_i = 0; oc_i < pTGConv.getOutC(); ++oc_i) {
    for (int k_i = 0; k_i < pTGConv.getKH() * pTGConv.getKW(); ++k_i) {
      for (int ic_i = 0; ic_i < ic; ++ic_i) {
        int to = oc_i * (pTGConv.getKH() * pTGConv.getKW() * ic) + k_i * ic +
                 ic_i;
        int from = oc_i * (pTGConv.getKH() * pTGConv.getKW() * ic) +
                   ic_i * (pTGConv.getKH() * pTGConv.getKW()) + k_i;
        weight[to] = (int8_t)raw[from];
      }
    }
  }

  // 16bit bias
  if (pTGConv.getDoBias() == 1) {
    auto *mem_op = pTGConv.getMemOperand(pTGConv.getBiasIdx());
    Weight::prepare16bitWeight(*mem_op, weight);
  }

  // 8bit scale bias
  if (pTGConv.getDoScale() == 1) {
    auto *mem_op = pTGConv.getMemOperand(pTGConv.getScaleIdx());
    Weight::prepare8bitWeight(*mem_op, weight);
  }

  // 16bit scale bias
  if (pTGConv.getDoScaleBias() == 1) {
    auto *mem_op = pTGConv.getMemOperand(pTGConv.getScaleBiasIdx());
    Weight::prepare16bitWeight(*mem_op, weight);
  }

  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}

void Weight::prepareWeight(const TLConv& pTLConv)
{
  Weight::WeightType weight;
  if (!isWritten(pTLConv.getMemOperand(1))) {
    setWritten(pTLConv.getMemOperand(1));
    const onnx::Value *value = pTLConv.getMemOperand(1)->m_Value;
    const onnx::Tensor &tensor =
        onnc::getTensor(value->uniqueName(), *value->owningGraph());
    assert(pTLConv.getMemOperand(1)->m_Type == onnx::TensorProto_DataType_INT8);
    assert(tensor.is_raw_data());
    const std::string &raw = tensor.raw();
    size_t count = onnc::getTotalCount(tensor.sizes());
    weight.resize(count);
    int ic = pTLConv.getInC() / pTLConv.getGroups();

    // conv weight is arranged by (1, oc, kh*kw, ic)
    // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
    for (int oc_i = 0; oc_i < pTLConv.getOutC(); ++oc_i) {
      for (int k_i = 0; k_i < pTLConv.getKH() * pTLConv.getKW(); ++k_i) {
        for (int ic_i = 0; ic_i < ic; ++ic_i) {
          int to = oc_i * (pTLConv.getKH() * pTLConv.getKW() * ic) +
              k_i * ic + ic_i;
          int from = oc_i * (pTLConv.getKH() * pTLConv.getKW() * ic) +
              ic_i * (pTLConv.getKH() * pTLConv.getKW()) + k_i;
          weight[to] = (int8_t)raw[from];
        }
      }
    }
  }

  if (pTLConv.getDoBias() == 1) {
    auto *mem_op = pTLConv.getMemOperand(pTLConv.getBiasIdx());
    if (!isWritten(pTLConv.getMemOperand(pTLConv.getBiasIdx()))) {
      setWritten(pTLConv.getMemOperand(pTLConv.getBiasIdx()));
      Weight::prepare16bitWeight(*mem_op, weight);
    }
  }

  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}
