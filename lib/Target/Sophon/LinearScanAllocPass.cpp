//===- LinearScanAllocPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "sophon_LSAP"
#include "LinearScanAllocPass.h"
#include "TGBackend.h"
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/Value.h>
#include <onnc/Support/Debug.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LinearScanAlloc
//===----------------------------------------------------------------------===//
LinearScanAlloc::LinearScanAlloc(TGBackend *pTarget)
    : m_pTarget(pTarget)
{
}

Pass::ReturnType LinearScanAlloc::runOnModule(::onnc::Module &pModule)
{
  assert (pModule.getNumOfComputeGraphs() == 1);
  linearScanAllocMem(*pModule.getRootComputeGraph(),
                     getAnalysis<BuildMemOpnd>()->getMemOperandList());

  return Pass::kModuleNoChanged;
}

int64_t getNumElems(const onnc::Value *pV)
{
  int64_t n = 1;
  onnc::Tensor *tensor = (onnc::Tensor *)pV;
  for (unsigned i = 0; i < tensor->getNumOfDimensions(); ++i)
    n *= tensor->dimension(i);
  return n;
}

void LinearScanAlloc::memoryAlloc(ComputeMemOperand *pMemOp,
                                  const onnc::Value *pMemVal)
{
  TGBackend::ValMemOpndMap &allocatedValue = m_pTarget->getValMemOpndMap();

  if (allocatedValue.find(pMemVal) != allocatedValue.end())
    return;

  // handle duplicate ComputeMemOperand for nop Operator
  // update valMemOpndMap
  if (m_AllocatedMemOpnd.find(pMemOp) != m_AllocatedMemOpnd.end()) {
    allocatedValue.insert({ pMemVal, pMemOp });
    DEBUG(dbgs() << "allocatedValue insert: " << pMemVal->getName()
                 << ", start: " << pMemOp->start()
                 << ", end: " << pMemOp->length() << "\n");
    return;
  }

  xTensorProtoDataType ty = (xTensorProtoDataType)pMemVal->kind();
  int tensor_size = m_pTarget->sizeOfTensorType(ty) * getNumElems(pMemVal);

  if (pMemOp->residence() != ComputeOperand::kWeightResidence) {
    pMemOp->setStart(m_NeuronOffset);
    m_NeuronOffset += tensor_size;
  } else {
    pMemOp->setStart(m_WeightOffset);
    m_WeightOffset += tensor_size;
  }

  pMemOp->setLength(tensor_size);
  allocatedValue.insert({ pMemVal, pMemOp });
  DEBUG(dbgs() << "allocatedValue insert: " << pMemVal->getName() << ", start: "
               << pMemOp->start() << ", end: " << pMemOp->length() << "\n");
  m_AllocatedMemOpnd.insert(pMemOp);
}

void LinearScanAlloc::linearScanAllocMem(
    const ComputeGraph &pCG, const BuildMemOpnd::ValMemOpndMap &pValMemOpndMap)
{
  m_WeightOffset = 0;
  m_NeuronOffset = 0;
  m_AllocatedMemOpnd.clear();
  auto valMemOpndMap = pValMemOpndMap;

  // we want to increase locality for loading weight data
  // so we allocate memory in operator execution order.
  // NOTE: if changed traverse order, we need to changed traverse order in
  // GenWeightPass too.
  auto iEnd = pCG.end();
  for (auto instIt = pCG.begin(); instIt != iEnd; ++instIt) {
    const ComputeOperator *inst = instIt;

    if (isa<OutputOperator>(inst) || isa<InputOperator>(inst) ||
        isa<Initializer>(inst))
      continue;

    // inputs of inst
    unsigned int ins = inst->getNumOfInputs();
    for (unsigned int i = 0; i < ins; ++i) {
      const onnc::Value *memVal = inst->getInput(i);
      ComputeMemOperand *memOp = valMemOpndMap[memVal];
      memoryAlloc(memOp, memVal);
    }

    // outputs of inst
    unsigned int outs = inst->getNumOfOutputs();
    for (unsigned int i = 0; i < outs; ++i) {
      const onnc::Value *memVal = inst->getOutput(i);
      ComputeMemOperand *memOp = valMemOpndMap[memVal];
      memoryAlloc(memOp, memVal);
    }
  }
}

void LinearScanAlloc::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequired<BuildMemOpnd>();
}

ModulePass *onnc::CreateLinearScanAllocPass(TGBackend *pTarget)
{
  return new LinearScanAlloc(pTarget);
}
