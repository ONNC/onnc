//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "BM188xTargetTransformInfo.h"
#include "TGBackend.h"
#include <algorithm>
#include <iostream>
#include <onnc/Target/TargetMemInfo.h>
#include <unordered_map>

using namespace onnc;

namespace {
const int NPU_NUM = 32;
const int EU_NUM = 16;
const int BUS_BITWIDTH = 128;

using CostFunction = int (*)(TGBackend *, const xNode *pNode);
using CostModelMap = std::unordered_map<xNodeKind, CostFunction>;

inline int idiv_round(int pNumerator, int pDenominator)
{
  return (pNumerator + pDenominator - 1) / pDenominator;
}

size_t getNumNeuron(const xValue &pValue)
{
  size_t total = 1;
  for (const xDimension &dim : pValue.sizes())
    total *= dim.dim;
  return total;
}

#if 0
// Currently not used by anyone
size_t getNumElems(const onnx::Node &pNode)
{
  size_t total = 1;
  for (const onnx::Value *v : pNode.inputs())
    for (const onnx::Dimension &dim : v->sizes())
      total *= dim.dim;
  return total;
}
#endif

int ZeroCost(TGBackend *pTGBackend, const xNode *pNode) { return 0; }

int countHWStepsFromOutputValue(const xValue *pValue)
{
  auto ResultDim = pValue->sizes();
  int DimNum = ResultDim.size();
  int MinDimNum = std::min(4, DimNum);
  assert(DimNum > 0 && "ResultDim is under zero");
  std::vector<int> NCHW(4, 1);
  for (int i = 0; i < MinDimNum; ++i)
    NCHW[i + 4 - MinDimNum] = ResultDim[i].dim;

  int HWSteps = NCHW[0] * idiv_round(NCHW[1], NPU_NUM) *
                idiv_round(NCHW[2] * NCHW[3], EU_NUM);

  for (int i = 4; i < DimNum; ++i)
    HWSteps *= ResultDim[i].dim;
  return HWSteps;
}

template <int m_Factor8Bit, int m_Factor16Bit>
int TensorOpCost(TGBackend *pTGBackend, const xNode *pNode)
{
  auto *Value = pNode->outputs().at(0);
  int HWSteps = countHWStepsFromOutputValue(Value);
  int TotalCycles = HWSteps * m_Factor8Bit;
  return TotalCycles;
}

int ConvOpCost(TGBackend *pTGBackend, const xNode *pNode)
{
  /* From wiki
     RES_N * idiv_round((RES_C_START + RES_C), LANE_NUM)
     * idiv_round(RES_W * RES_H, EU_NUM)
     * (OPD1_W * OPD1_H * OPD0_C *
           (((STR_W + 1) > MIN(OPD0_C, LANE_NUM))
           ? (STR_W + 1) / MIN(OPD0_C, LANE_NUM) : 1)
           + (HAVE_BIAS ? 2 : 0) + 3
       );
   */
  auto *OutputValue = pNode->outputs().at(0);
  auto OutputDim = OutputValue->sizes();
  // TODO(arcbbb): Assume 2d conv here
  assert(OutputDim.size() == 4);

  int OutputN = OutputDim[0].dim;
  int OutputC = OutputDim[1].dim;
  int OutputH = OutputDim[2].dim;
  int OutputW = OutputDim[3].dim;

  int HWSteps = OutputN * idiv_round(OutputC, NPU_NUM) *
                idiv_round(OutputH * OutputW, EU_NUM);

  auto *WeightValue = pNode->inputs().at(1);
  int WeightCount = 1;
  for (auto dim : WeightValue->sizes()) {
    WeightCount *= dim.dim;
  }
  bool HasBias = pNode->inputs().size() == 3;
  int BiasCycle;
  if (HasBias) {
    BiasCycle = 2;
  } else {
    BiasCycle = 0;
  }

  int BaseCost = 3;
  int StepCost = (WeightCount + BiasCycle + BaseCost);
  int TotalCycles = HWSteps * StepCost;
  return TotalCycles;
}

int GemmOpCost(TGBackend *pTGBackend, const xNode *pNode)
{
  /* From wiki
     RES_N * idiv_round(RES_C_START+RES_C, LANE_NUM)
     * idiv_round(RES_W,EU_NUM)
     * ((OPD0_W*(OPD0_C-1)+OPD1_W)+6+(HAVE_BIAS?2:0)+(ADD_RES?2:0))
   */

  // A' = transpose(A) if transA else A
  // B' = transpose(B) if transB else B
  // Y = alpha * A' * B' + beta * C
  auto *OutputValue = pNode->outputs().at(0);
  auto OutputDim = OutputValue->sizes();

  auto ADim = pNode->inputs()[0]->sizes();
  auto BDim = pNode->inputs()[1]->sizes();

  // A: M x K
  int64_t DimM = ADim[0].dim;
  int64_t DimK = ADim[1].dim;
  if (pNode->hasAttribute(xSymbol("transA"))) {
    auto transA = pNode->i(xSymbol("transA"));
    DimM = transA ? ADim[1].dim : ADim[0].dim;
    DimK = transA ? ADim[0].dim : ADim[1].dim;
  }
  // B: DimK x N
  int64_t DimN = BDim[1].dim;
  if (pNode->hasAttribute(xSymbol("transB"))) {
    auto transB = pNode->i(xSymbol("transB"));
    DimN = transB ? BDim[0].dim : BDim[1].dim;
  }
  int HWSteps = idiv_round(DimM, NPU_NUM) * idiv_round(DimN, EU_NUM);
  int StepCost = (DimK * (DimM - 1) + DimN) + 6;
  int TotalCycles = HWSteps * StepCost;
  return TotalCycles;
}

int MaxPoolOpCost(TGBackend *pTGBackend, const xNode *pNode)
{
  /* From 1682
  int ActiveEUNum = idiv_round(EU_NUM/stride_w);
  int TotalCycle = 6 + input_n * idiv_round(input_c, NPU_NUM) * kh * kw
    * idiv_round(output_h * output_w, ActiveEUNum);
    */
  auto *OutputValue = pNode->outputs().at(0);
  auto OutputDim = OutputValue->sizes();
  assert(OutputDim.size() == 4);

  auto &KernelShape = pNode->is(xSymbol("kernel_shape"));
  int KHeight = KernelShape.at(0);
  int KWidth = KernelShape.at(1);
  auto &StrideShape = pNode->is(xSymbol("strides"));
  int StrideW = StrideShape.at(1);

  int OutputN = OutputDim[0].dim;
  int OutputC = OutputDim[1].dim;
  int OutputH = OutputDim[2].dim;
  int OutputW = OutputDim[3].dim;

  int ActiveEUNum = idiv_round(EU_NUM, StrideW);
  int HWSteps = idiv_round(OutputH * OutputW, ActiveEUNum);
  int StepCost = OutputN * idiv_round(OutputC, NPU_NUM) * KHeight * KWidth + 6;
  int TotalCycles = HWSteps * StepCost;
  return TotalCycles;
}

int LoadOpCost(TGBackend *pTGBackend, const xNode *pNode)
{
  const xValue *OutputValue = pNode->outputs().at(0);
  int NumNeuron = getNumNeuron(*OutputValue);
  TargetMemInfo *MemInfo = pTGBackend->getMemInfo();
  int Bytes = NumNeuron * MemInfo->getElemSize(OutputValue->elemType());
  int TotalCycles = (Bytes / (BUS_BITWIDTH >> 3));
  return TotalCycles;
}

int StoreOpCost(TGBackend *pTGBackend, const xNode *pNode)
{
  auto *InputValue = pNode->inputs().at(0);
  int NumNeuron = getNumNeuron(*InputValue);
  int TotalCycles = NumNeuron;
  return TotalCycles;
}

CostModelMap g_NodeCostModels = {
  { xSymbol("Conv"), ConvOpCost },
  { xSymbol("MaxPool"), MaxPoolOpCost },
  { xSymbol("Gemm"), GemmOpCost },

  { xSymbol("Add"), TensorOpCost<6, 7> },
  { xSymbol("Mul"), TensorOpCost<5, 3> },
  { xSymbol("Sub"), TensorOpCost<6, 7> },
  { xSymbol("Max"), TensorOpCost<2, 2> },
  { xSymbol("Relu"), TensorOpCost<2, 2> },
  { xSymbol("Min"), TensorOpCost<2, 2> },
  { xSymbol("And"), TensorOpCost<2, 5> },
  { xSymbol("Or"), TensorOpCost<2, 5> },
  { xSymbol("Xor"), TensorOpCost<2, 5> },

  { xSymbol("Softmax"), ZeroCost },
  { xSymbol("Dropout"), ZeroCost },
  { xSymbol("Undefined"), ZeroCost },

  // ONNC Extension
  { xSymbol("Load"), LoadOpCost },
  { xSymbol("Store"), StoreOpCost },
};

} // namespace

uint64_t BM188xTargetTransformInfo::getOperatorCost(const xNode *pNode,
                                                    unsigned pKind) const
{
  auto it = g_NodeCostModels.find(pNode->kind());
  if (it != g_NodeCostModels.end()) {
    return (*it->second)(m_pTGBackend, pNode);
  }
  std::cerr << "Unsupported node: " << pNode->kind().toString() << "\n";
  assert(false && "TG1880TTI::getOperatorCost: Unsupported node.");
  return -1;
}

int BM188xTargetTransformInfo::getWarpSize() const { return NPU_NUM; }

int BM188xTargetTransformInfo::getProcessingUnitCount() const { return EU_NUM; }

int BM188xTargetTransformInfo::getBusBitWidth() const { return BUS_BITWIDTH; }
