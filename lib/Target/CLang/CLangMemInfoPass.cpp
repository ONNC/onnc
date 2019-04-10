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
  CLangMemoryBlock::size_type packedInputMemorySize    = 0;
  CLangMemoryBlock::size_type packedWeightMemorySize   = 0;
  CLangMemoryBlock::size_type packedInternalMemorySize = 0;
  for (ComputeOperand* co : pModule.getComputeOperands()) {
    if (ComputeMemOperand* mem = dyn_cast<ComputeMemOperand>(co)) {
      const auto* const tensor = static_cast<const Tensor*>(co->getValue());
      if (mem->isInput()) {
        m_pMeta.packedInputMemoryBlocks.emplace_back(
          std::make_pair(tensor, CLangMemoryBlock{packedInputMemorySize, mem->length()}));
        packedInputMemorySize += mem->length();
      } else if (mem->isWeight()) {
        m_pMeta.packedWeightMemoryBlocks.emplace_back(
          std::make_pair(tensor, CLangMemoryBlock{packedWeightMemorySize, mem->length()}));
        packedWeightMemorySize += mem->length();
      } else {
        m_pMeta.packedInternalMemoryBlocks.emplace_back(
          std::make_pair(tensor, CLangMemoryBlock{packedInternalMemorySize, mem->length()}));
        packedInternalMemorySize += mem->length();
      }
    }
  }

  static const auto showBlockInfo = [](std::ostream& stream, const auto& pair) {
    const auto* const tensor      = pair.first;
    const auto&       memoryBlock = pair.second;
    stream << "[Clang] (block) tensor = " << tensor << ", offset = " << std::setw(9) << memoryBlock.offset
           << ", length = " << std::setw(9) << memoryBlock.length << std::endl;
  };

  static const auto showBlocksInfo = [](std::ostream& stream, const auto& pairs) {
    for (const auto& pair : pairs) {
      showBlockInfo(stream, pair);
    }
  };

  assert(!m_pMeta.packedInputMemoryBlocks.empty());
  showBlocksInfo(outs(), m_pMeta.packedInputMemoryBlocks);
  outs() << "[Clang] input memory: " << packedInputMemorySize << std::endl;

  assert(!m_pMeta.packedWeightMemoryBlocks.empty());
  showBlocksInfo(outs(), m_pMeta.packedWeightMemoryBlocks);
  outs() << "[Clang] weight memory: " << packedWeightMemorySize << std::endl;

  assert(!m_pMeta.packedInternalMemoryBlocks.empty());
  showBlocksInfo(outs(), m_pMeta.packedInternalMemoryBlocks);
  outs() << "[Clang] internal memory: " << packedInternalMemorySize << std::endl;

  return Pass::kModuleNoChanged;
}
