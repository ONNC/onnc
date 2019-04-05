//===- CLangMemInfoPass.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CLangMemInfoPass.h"

#include <iostream>
#include <unordered_map>

#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>


using namespace onnc;

CLangMemInfoPass::CLangMemInfoPass(CLangMeta& pMeta) noexcept
  : m_pMeta(pMeta)
{}

Pass::ReturnType CLangMemInfoPass::runOnModule(Module& pModule)
{
  std::unordered_map<
    const Tensor*, CLangMemoryBlock
  > internalMemoryBlocks;
  for (ComputeOperand *co : pModule.getComputeOperands()) {
    if (ComputeMemOperand *mem = dyn_cast<ComputeMemOperand>(co)) {
      if (mem->isInput()) {
        continue;
      } else if (mem->isWeight()) {
        const auto* const tensor = static_cast<const Tensor*>(co->getValue());
        m_pMeta.weightMemoryBlocks.emplace(
          tensor, CLangMemoryBlock{mem->start(), mem->length()}
        );
      } else {
        const auto* const tensor = static_cast<const Tensor*>(co->getValue());
        internalMemoryBlocks.emplace(
          tensor, CLangMemoryBlock{mem->start(), mem->length()}
        );
      }
    }
  }

  CLangMemoryBlock::size_type packedInternalMemorySize = 0;
  for (const auto& entry : internalMemoryBlocks) {
    const auto* const tensor      = entry.first;
    const auto&       memoryBlock = entry.second;
    m_pMeta.packedInternalMemoryBlocks.emplace_back(
      tensor, CLangMemoryBlock{packedInternalMemorySize, memoryBlock.length}
    );
    packedInternalMemorySize += memoryBlock.length;
  }

  for (const auto& entry : m_pMeta.packedInternalMemoryBlocks) {
    const auto* const tensor      = entry.first;
    const auto&       memoryBlock = entry.second;
    outs() << "(internal) address = " << tensor
           << ", offset = " << memoryBlock.offset
           << ", length = " << memoryBlock.length
           << std::endl;
  }
  outs() << "[Clang] internal memory: " << packedInternalMemorySize << std::endl;

  return Pass::kModuleNoChanged;
}
