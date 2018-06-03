#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

#define DEBUG_TYPE "tg_mem_alloc"
#include <onnc/ADT/Color.h>
#include <onnc/Support/Debug.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

namespace {

class TGMemAllocInfo : public ModulePass
{

private:
  TGBackend *m_pTarget;
  void ddrAllocInfo();

public:
  static char ID;

public:
  TGMemAllocInfo(TGBackend *pTarget) : ModulePass(ID), m_pTarget(pTarget) {}

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override
  {
    ddrAllocInfo();
    return Pass::kModuleNoChanged;
  }
};

void TGMemAllocInfo::ddrAllocInfo()
{
  unsigned int weight_offset = 0;
  unsigned int neuron_offset = 0;
  std::string tab = "\t";

  DEBUG(dbgs() << __func__ << " dump global memory layout:"
               << "\n";);
  std::map<std::string, MemOperand *> memoryMap;
  for (auto &inst : m_pTarget->getInsts()) {
    DEBUG(dbgs() << tab << "=== Inst: " << inst->getLayerName() << "\n";);
    for (auto &mem : inst->getMemOprnds()) {
      int tensor_size = 0;
      if (memoryMap.find(mem.name) != memoryMap.end()) {
        mem.addr = memoryMap.at(mem.name)->addr;
        mem.size = memoryMap.at(mem.name)->size;
        continue;
      }
      if (mem.tag == GLOBAL_NEURON_TAG) {
        mem.addr = neuron_offset;
        tensor_size = m_pTarget->sizeOfTensorType(mem.type) * mem.count;
        neuron_offset += tensor_size;
      } else if (mem.tag == GLOBAL_WEIGHT_TAG) {
        mem.addr = weight_offset;
        tensor_size = m_pTarget->sizeOfTensorType(mem.type) * mem.count;
        weight_offset += tensor_size;
      } else {
        errs() << Color::RED << "Error" << Color::RESET
               << ": Unsupport mem type " << mem.tag << "\n";
        assert(0);
      }
      mem.size = tensor_size;
      memoryMap.insert({ mem.name, &mem });
      DEBUG(dbgs() << tab << mem << "\n");
    }
  }
}

} // anonymous namespace

char TGMemAllocInfo::ID = 0;

ModulePass *onnc::createTGMemAllocInfoPass(TGBackend *pTarget)
{
  return new TGMemAllocInfo(pTarget);
}
