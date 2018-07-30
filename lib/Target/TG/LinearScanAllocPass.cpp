//===- LinearScanAllocPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LinearScanAllocPass.h"
#include "TGBackend.h"
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/Value.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LinearScanAlloc
//===----------------------------------------------------------------------===//
char LinearScanAlloc::ID = 0;

LinearScanAlloc::LinearScanAlloc(TGBackend *pTarget)
    : ModulePass(ID), m_pTarget(pTarget)
{
}

Pass::ReturnType LinearScanAlloc::runOnModule(::onnc::Module &pModule)
{
  linearScanAlloMem(getAnalysis<BuildMemOpnd>()->getMemOperandList());

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

void LinearScanAlloc::linearScanAlloMem(
    const BuildMemOpnd::MemOperandValList &pMemOps)
{
  unsigned int weight_offset = 0;
  unsigned int neuron_offset = 0;

  // FIXME memory allocation only need to traverse MemOperands in order
  // but currently CodeEmitter's prepareWeight function can't save the weight
  // on the address of MemOperand. So we need to sync the traverse order
  // between MemAlloc and prepareWeight now.
  std::unordered_map<const onnc::Value *, ComputeMemOperand *> allocatedValue;
  for (auto memOpVal : pMemOps) {
    ComputeMemOperand *memOp = memOpVal.first;
    onnc::Value *memVal = memOpVal.second;
    if (allocatedValue.count(memVal)) {
      memOp->setStart(allocatedValue[memVal]->start());
      memOp->setLength(allocatedValue[memVal]->length());
      continue;
    }

    ::onnx::TensorProto_DataType ty =
      (::onnx::TensorProto_DataType)memVal->kind();

    int tensor_size = m_pTarget->sizeOfTensorType(ty) * getNumElems(memVal);

    if (memOp->residence() != ComputeOperand::kWeightResidence) {
      memOp->setStart(neuron_offset);
      neuron_offset += tensor_size;
    } else {
      memOp->setStart(weight_offset);
      weight_offset += tensor_size;
    }
    memOp->setLength(tensor_size);
    allocatedValue.insert({memVal, memOp});
  }
}

void LinearScanAlloc::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(BuildMemOpnd::ID);
}

ModulePass *onnc::CreateLinearScanAllocPass(TGBackend *pTarget)
{
  return new LinearScanAlloc(pTarget);
}