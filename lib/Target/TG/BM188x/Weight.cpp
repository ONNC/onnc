//===- Weight.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Weight.h"
#include "TGConv.h"
#include "TLConv.h"
#include <onnc/Target/TG/io.hpp>

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
  size_t weight_size = 0;
  size_t weight_count = 0;
  for (auto *mem_op : pMemOperands) {
    if (mem_op->m_MemType == MemType::NEURON)
      continue;
    weight_size += mem_op->m_Size;
    ++weight_count;
  }

  m_Weight.reserve(weight_size);

  // TODO save the weight on the address of MemOperand
  for (auto &inst : pInstructions) {

    // handle special case
    if (inst->getTypeName() == "Conv") {
      std::vector<DataType> weight;
      auto *tgconv = dynamic_cast< ::onnc::BM188X::TGConv *>(inst.get());
      tgconv->prepareWeight(weight);
      m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
      continue;
    }

    if (inst->getTypeName() == "TLConv") {
      std::vector<DataType> weight;
      auto *tlconv = dynamic_cast< ::onnc::BM188X::TLConv *>(inst.get());
      tlconv->prepareWeight(weight);
      m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
      continue;
    }

    if (inst->getTypeName() == "TLLoad") {
      continue;
    }

    if (inst->getTypeName() == "TLStore") {
      continue;
    }

    // for those instruction not TLLoad and TLStore
    for (auto *mem_op : inst->getMemOperands()) {
      if (mem_op->m_MemType == MemType::NEURON)
        continue;

      if (mem_op->m_Type == ::onnx::TensorProto_DataType_INT8) {
        prepare8bitWeight(*mem_op, m_Weight);
      } else {
        prepare16bitWeight(*mem_op, m_Weight);
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
