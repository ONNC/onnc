//===- CLangMemInfoPass.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CLangMemInfoPass.h"

#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

#include <unordered_map>

using namespace onnc;

CLangMemInfoPass::CLangMemInfoPass(CLangMeta* pMeta) noexcept
: m_pMeta(pMeta)
{}

Pass::ReturnType CLangMemInfoPass::runOnModule(Module& pModule)
{
  std::unordered_map<Value *, int64_t> mem_start;
  std::unordered_map<Value *, int64_t> mem_length;

  uint64_t weight_memory_size = 0;
  uint64_t internal_memory_size = 0;
  for (ComputeOperand *co : pModule.getComputeOperands()) {
    if (ComputeMemOperand *mem = dyn_cast<ComputeMemOperand>(co)) {
      Value *v = co->getValue();
      if (mem->isInput()) {
        // m_pMeta->m_ATable[v] = m_pInputMem;
      } else if (mem->isWeight()) {
        FloatTensor *t = static_cast<FloatTensor *>(v);
        m_pMeta->m_ATable[v] = t->getValues().data();
        weight_memory_size +=
            t->getValues().size() * sizeof(FloatTensor::ValueList::value_type);
      } else {
        mem_start[co->getValue()] = mem->start();
        mem_length[co->getValue()] = mem->length();
        internal_memory_size =
            std::max(internal_memory_size,
                     static_cast<uint64_t>(mem->start()) + mem->length());
      }
    }
  }
  outs() << "[Clang] weight memory: " << weight_memory_size << std::endl;
  outs() << "[Clang] internal memory: " << internal_memory_size << std::endl;

  return Pass::kModuleNoChanged;
}
