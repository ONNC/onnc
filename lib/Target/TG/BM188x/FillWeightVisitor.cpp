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
    : m_Weight(pWeight)
{
}

void FillWeightVisitor::visit(const onnc::Initializer &pOp)
{
  if (onnc::Value::kInt8 == pOp.getTensor<onnc::Value>()->kind()) {
  }
  else {
  }
}

void FillWeightVisitor::visit(const BM188X::Conv& pConv)
{
/**
  const onnx::Value *value = pTGConv.getMemOperand(1)->m_Value;
  const onnx::Tensor &tensor =
        onnc::getTensor(value->uniqueName(), *value->owningGraph());
  assert(pTGConv.getMemOperand(1)->m_Type == onnx::TensorProto_DataType_INT8);
  assert(tensor.is_raw_data());
  const std::string &raw = tensor.raw();
  size_t count = onnc::getTotalCount(tensor.sizes());
**/
  GenWeightPass::WeightType weight;
/**
  weight.resize(count);
  std::vector<int8_t> data;
  std::copy(raw.begin(), raw.end(), std::back_inserter(data));

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
        weight[to] = data[from];
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
**/
  // update weight
  m_Weight.insert(m_Weight.end(), weight.begin(), weight.end());
}

void FillWeightVisitor::visit(const BM188X::SlicedConv& pSlicedConv)
{
}
