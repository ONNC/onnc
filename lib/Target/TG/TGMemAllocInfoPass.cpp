#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

#define DEBUG_TYPE "tg_mem_alloc"
#include <onnc/Support/Debug.h>

using namespace onnc;

namespace {

class TGMemAllocInfo : public ModulePass
{

private:
  TGBackend *m_pTarget;
  void ddrAllocInfo(::onnx::Graph &graph, MemTable &memTable);

public:
  static char ID;

public:
  TGMemAllocInfo(TGBackend *pTarget) : ModulePass(ID), m_pTarget(pTarget) {}

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override
  {
    ::onnx::Graph *graph = pModule.getGraphIR().get();
    ddrAllocInfo(*graph, m_pTarget->getMemLayout());
    return Pass::kModuleNoChanged;
  }
};

void TGMemAllocInfo::ddrAllocInfo(::onnx::Graph &graph, MemTable &memTable)
{
  // Definition fom BM168xBackendContext.hpp
  // TAG will be masked by runtime while processing cmdbuf.
  const int GLOBAL_NEURON_TAG = 0x1;
  const int GLOBAL_WEIGHT_TAG = 0x2;
  const int GLOBAL_ARM_TAG = 0x3;

  // allocate spaces for weight
  unsigned int weight_offset = 0;
  // BMKernel only supports DATA_FMT_F32 & DATA_FMT_I1
  std::string tab = "\t";

  DEBUG(dbgs() << __func__ << " dump global memory layout:" << std::endl;);

  for (auto i : graph.initializers()) {

    memTable[i.name()] = weight_offset + GLOBAL_WEIGHT_TAG;
    DEBUG(dbgs() << tab << i.name() << " = " << weight_offset;);

    if (i.sizes().size() > 0) {
      int tensor_size = m_pTarget->sizeOfTensorType(i.elem_type());
      DEBUG(dbgs() << " <";);
      for (auto dim : i.sizes()) {
        DEBUG(dbgs() << dim << ",";);
        tensor_size *= dim;
      }
      DEBUG(dbgs() << ">" << std::endl;);
      weight_offset += tensor_size;
    } else {
      DEBUG(dbgs() << std::endl;);
    }
  }

  unsigned int neuron_offset = 0;
  std::unordered_set<std::string> initNames(graph.initializer_names().begin(),
                                            graph.initializer_names().end());
  // allocate space for inputs
  for (auto i : graph.inputs()) {
    if (0 == initNames.count(i->uniqueName())) {

      memTable[i->uniqueName()] = neuron_offset + GLOBAL_NEURON_TAG;
      DEBUG(dbgs() << tab << i->uniqueName() << " = " << neuron_offset;);

      if (i->sizes().size() > 0) {
        int tensor_size = m_pTarget->sizeOfTensorType(i->elemType());
        DEBUG(dbgs() << " <";);
        for (auto &dim : i->sizes()) {
          DEBUG(dbgs() << dim.dim << ",";);
          tensor_size *= dim.dim;
        }
        DEBUG(dbgs() << ">" << std::endl;);
        neuron_offset += tensor_size;
      } else {
        DEBUG(dbgs() << std::endl;);
      }
    }
  }
  // allocate space for outputs
  for (auto i : graph.nodes()) {
    if (i->kind() == ::onnx::Symbol("Undefined"))
      continue;

    for (auto o : i->outputs()) {

      memTable[o->uniqueName()] = neuron_offset + GLOBAL_NEURON_TAG;
      DEBUG(dbgs() << tab << o->uniqueName() << " = " << neuron_offset;);

      if (o->sizes().size() > 0) {
        int tensor_size = m_pTarget->sizeOfTensorType(o->elemType());
        DEBUG(dbgs() << " <";);
        for (auto dim : o->sizes()) {
          DEBUG(dbgs() << dim.dim << ",";);
          tensor_size *= dim.dim;
        }
        DEBUG(dbgs() << ">" << std::endl;);
        neuron_offset += tensor_size;
      } else {
        DEBUG(dbgs() << "\n";);
      }
    }
  }
  DEBUG(dbgs() << tab << "weight size: " << weight_offset << "\n";);
  DEBUG(dbgs() << tab << "neuron size: " << neuron_offset << "\n";);
}

} // anonymous namespace

char TGMemAllocInfo::ID = 0;

ModulePass *onnc::createTGMemAllocInfoPass(TGBackend *target)
{
  return new TGMemAllocInfo(target);
}
