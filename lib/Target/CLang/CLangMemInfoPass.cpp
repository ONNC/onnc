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

#include <cassert>
#include <iomanip>
#include <iostream>
#include <unordered_map>

using namespace onnc;

CLangMemInfoPass::CLangMemInfoPass(CLangMeta& pMeta) noexcept
  : m_pMeta(pMeta)
{}

Pass::ReturnType CLangMemInfoPass::runOnModule(Module& pModule)
{
  CLangMemoryBlock::size_type packedWeightMemorySize   = 0;
  CLangMemoryBlock::size_type packedInternalMemorySize = 0;
  for (ComputeOperand* co : pModule.getComputeOperands()) {
    if (ComputeMemOperand* mem = dyn_cast<ComputeMemOperand>(co)) {
      if (mem->isInput()) {
        const auto* const tensor = static_cast<const Tensor*>(co->getValue());
        std::cout << "[Clang] (input) tensor = " << tensor << std::endl;
      } else if (mem->isWeight()) {
        const auto* const tensor = static_cast<const Tensor*>(co->getValue());
        m_pMeta.packedWeightMemoryBlocks.emplace_back(
          std::make_pair(tensor, CLangMemoryBlock{packedWeightMemorySize, mem->length()}));
        packedWeightMemorySize += mem->length();
      } else {
        const auto* const tensor = static_cast<const Tensor*>(co->getValue());
        m_pMeta.packedInternalMemoryBlocks.emplace_back(
          std::make_pair(tensor, CLangMemoryBlock{packedInternalMemorySize, mem->length()}));
        packedInternalMemorySize += mem->length();
      }
    }
  }

  assert(!m_pMeta.packedWeightMemoryBlocks.empty());
  for (const auto& entry : m_pMeta.packedWeightMemoryBlocks) {
    const auto* const tensor      = entry.first;
    const auto&       memoryBlock = entry.second;
    outs() << "[Clang] (block) tensor = " << tensor << ", offset = " << std::setw(9) << memoryBlock.offset
           << ", length = " << std::setw(9) << memoryBlock.length << std::endl;
  }
  outs() << "[Clang] weight memory: " << packedWeightMemorySize << std::endl;

  assert(!m_pMeta.packedInternalMemoryBlocks.empty());
  for (const auto& entry : m_pMeta.packedInternalMemoryBlocks) {
    const auto* const tensor      = entry.first;
    const auto&       memoryBlock = entry.second;
    outs() << "[Clang] (block) tensor = " << tensor << ", offset = " << std::setw(9) << memoryBlock.offset
           << ", length = " << std::setw(9) << memoryBlock.length << std::endl;
  }
  outs() << "[Clang] internal memory: " << packedInternalMemorySize << std::endl;

  return Pass::kModuleNoChanged;
}
