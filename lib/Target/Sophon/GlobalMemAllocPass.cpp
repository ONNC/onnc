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
//===----------------------------------------------------------------------===//
#include "GlobalMemAllocPass.h"
#include "BM188x/BM188xVisitor.h"
#include "TG.h"
#include "TGBackend.h"
#include <onnc/ADT/Color.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/Support/Debug.h>
#include <onnc/Support/IOStream.h>
#include <onnx/common/ir.h>

#define DEBUG_TYPE "tg_mem_alloc"

using namespace onnc;

//===----------------------------------------------------------------------===//
// GlobalMemAlloc
//===----------------------------------------------------------------------===//
char GlobalMemAlloc::ID = 0;

GlobalMemAlloc::GlobalMemAlloc(TGBackend *pTarget)
    : ModulePass(ID), m_pTarget(pTarget)
{
}

Pass::ReturnType GlobalMemAlloc::runOnModule(::onnc::Module &pModule)
{
  allocGlobalMem(); // remove this later.

  return Pass::kModuleNoChanged;
}

void GlobalMemAlloc::allocGlobalMem()
{
  unsigned int weight_offset = 0;
  unsigned int neuron_offset = 0;
  std::string tab = "\t";

  DEBUG(dbgs() << __func__ << " dump global memory layout:"
               << "\n";);

  // FIXME memory allocation only need to traverse MemOperands in order
  // but currently CodeEmitter's prepareWeight function can't save the weight
  // on the address of MemOperand. So we need to sync the traverse order
  // between MemAlloc and prepareWeight now.
  std::unordered_map<const ::onnx::Value *, MemOperand *> allocatedValue;
  for (auto &inst : m_pTarget->getInsts()) {
    for (auto &mem : inst->getMemOperands()) {
      int tensor_size = 0;
      if (allocatedValue.count(mem->m_Value)) {
        mem->m_Addr = allocatedValue[mem->m_Value]->m_Addr;
        mem->m_Size = allocatedValue[mem->m_Value]->m_Size;
        continue;
      }
      if (mem->m_MemType == MemType::NEURON) {
        mem->m_Addr = neuron_offset;
        tensor_size = m_pTarget->sizeOfTensorType(mem->m_Type) * mem->m_Count;
        neuron_offset += tensor_size;
      } else if (mem->m_MemType == MemType::WEIGHT) {
        mem->m_Addr = weight_offset;
        tensor_size = m_pTarget->sizeOfTensorType(mem->m_Type) * mem->m_Count;
        weight_offset += tensor_size;
      }
      mem->m_Size = tensor_size;
      allocatedValue.insert({ mem->m_Value, mem });
      DEBUG(dbgs() << tab << *mem << "\n");
    }
  }
}

ModulePass *onnc::CreateGlobalMemAllocPass(TGBackend *pTarget)
{
  return new GlobalMemAlloc(pTarget);
}
