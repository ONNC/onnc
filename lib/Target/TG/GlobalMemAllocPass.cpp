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
  TGBackend *m_pTarget;
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

void GlobalMemAlloc::AllocGlobalMem()
{
  unsigned int weight_offset = 0;
  unsigned int neuron_offset = 0;
  std::string tab = "\t";

  DEBUG(dbgs() << __func__ << " dump global memory layout:"
               << "\n";);
  std::unordered_map<const ::onnx::Value *, MemOperand *> allocatedValue;
  for (auto &mem : m_pTarget->getMemOperands()) {
    int tensor_size = 0;
    if (allocatedValue.count(mem->value)) {
      mem->addr = allocatedValue[mem->value]->addr;
      mem->size = allocatedValue[mem->value]->size;
      continue;
    }
    if (mem->memType == MemType::NEURON) {
      mem->addr = neuron_offset;
      tensor_size = m_pTarget->sizeOfTensorType(mem->type) * mem->count;
      neuron_offset += tensor_size;
    } else if (mem->memType == MemType::WEIGHT) {
      mem->addr = weight_offset;
      tensor_size = m_pTarget->sizeOfTensorType(mem->type) * mem->count;
      weight_offset += tensor_size;
    }
    mem->size = tensor_size;
    allocatedValue.insert({ mem->value, mem });
    DEBUG(dbgs() << tab << mem << "\n");
  }
}

} // anonymous namespace

char GlobalMemAlloc::ID = 0;

ModulePass *onnc::createGlobalMemAllocPass(TGBackend *pTarget)
{
  return new GlobalMemAlloc(pTarget);
}
