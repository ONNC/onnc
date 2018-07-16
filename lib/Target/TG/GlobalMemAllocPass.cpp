#define DEBUG_TYPE "tg_mem_alloc"
#include "TG.h"
#include "TGBackend.h"
#include <onnc/ADT/Color.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Support/Debug.h>
#include <onnc/Support/IOStream.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class GlobalMemAlloc : public ModulePass
{

private:
  TGBackend *m_pTarget; // NOLINT
  void AllocGlobalMem();

public:
  static char ID;

public:
  GlobalMemAlloc(TGBackend *pTarget) : ModulePass(ID), m_pTarget(pTarget) {}

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override
  {
    AllocGlobalMem();
    return Pass::kModuleNoChanged;
  }
};

} // anonymous namespace

void GlobalMemAlloc::AllocGlobalMem()
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

char GlobalMemAlloc::ID = 0;

ModulePass *onnc::createGlobalMemAllocPass(TGBackend *pTarget)
{
  return new GlobalMemAlloc(pTarget);
}
