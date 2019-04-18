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

  return Pass::kModuleNoChanged;
}
