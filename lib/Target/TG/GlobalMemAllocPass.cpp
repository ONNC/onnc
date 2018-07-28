//===- GlobalMemAllocPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
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
using namespace onnc::BM188X;

// For handle special cases.
class GlobalMemAllocVisitor : public BM188X::BM188xVisitor
{
  using BM188X::BM188xVisitor::visit;
public:
  GlobalMemAllocVisitor(onnc::ComputeGraph &pCG, GlobalMemAlloc &pGMemAllocPass)
    : m_Processed(false), m_CG(pCG), m_GMemAllocPass(pGMemAllocPass) {
  }

  void visit(onnc::Initializer &pInitializer) override
  {
    m_Processed = true;
    m_GMemAllocPass.createMemOperandsOfNode(m_CG, pInitializer,
                                            ComputeOperand::kWeightResidence);
  }

  bool isProcessed() const { return m_Processed; }

  void reset() { m_Processed = false; }

  // TLLoad TLStore

private:
  bool m_Processed;
  onnc::ComputeGraph &m_CG;
  GlobalMemAlloc &m_GMemAllocPass;
};

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
  clear();
  allocGlobalMem(); // remove this later.

  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg)
    createAllMemOperands(*cg->value());

  return Pass::kModuleNoChanged;
}

void GlobalMemAlloc::createAllMemOperands(onnc::ComputeGraph &pCG)
{
  GlobalMemAllocVisitor gmallocVisitor(pCG, *this);
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    // process special cases.
    gmallocVisitor.reset();
    nodeIt->accept(gmallocVisitor);
    if (gmallocVisitor.isProcessed())
      continue;

    // process general case.
    createMemOperandsOfNode(pCG, *nodeIt, ComputeOperand::kOutputResidence);
  }
}

void GlobalMemAlloc::createMemOperandsOfNode(ComputeGraph &pCG,
                                             ComputeOperator &pNode,
                                             ComputeOperand::Residence pResd)
{
  unsigned int out_size = pNode.getNumOfOutputs();
  for (unsigned int i = 0; i < out_size; ++i) {
    onnc::Value* value = pNode.getOutput(i);
    onnc::Value::UseList& use_list = value->getUses();
    onnc::Value::UseList::iterator use, uEnd = use_list.end();
    for (use = use_list.begin(); use != uEnd; ++use) {
      ComputeMemOperand *memOperand =
        pCG.addOperand<ComputeMemOperand>(pNode, *use->getUser(),
                                          *value, pResd);
      m_ValOperandMap.emplace_back(memOperand, value);
    }
  }
}

void GlobalMemAlloc::clear()
{
  m_ValOperandMap.clear();
}

int64_t getNumElems(const onnc::Value *pV)
{
  int64_t n = 1;
  onnc::Tensor *tensor = (onnc::Tensor *)pV;
  for (unsigned i = 0; i < tensor->getNumOfDimensions(); ++i)
    n *= tensor->dimension(i);
  return n;
}

void GlobalMemAlloc::linearScanAlloMem()
{
  unsigned int weight_offset = 0;
  unsigned int neuron_offset = 0;

  // FIXME memory allocation only need to traverse MemOperands in order
  // but currently CodeEmitter's prepareWeight function can't save the weight
  // on the address of MemOperand. So we need to sync the traverse order
  // between MemAlloc and prepareWeight now.
  std::unordered_map<const onnc::Value *, ComputeMemOperand *> allocatedValue;
  for (auto memOpVal : m_ValOperandMap) {
    ComputeMemOperand *memOp = memOpVal.first;
    onnc::Value *memVal = memOpVal.second;
    if (allocatedValue.count(memVal)) {
      memOp->setStart(allocatedValue[memVal]->start());
      memOp->setLength(allocatedValue[memVal]->length());
      continue;
    }

    ::onnx::TensorProto_DataType ty =
      (::onnx::TensorProto_DataType)memVal->kind();

    int tensor_size = m_pTarget->sizeOfTensorType(ty) * getNumElems(memVal);

    if (memOp->residence() != ComputeOperand::kWeightResidence) {
      memOp->setStart(neuron_offset);
      neuron_offset += tensor_size;
    } else {
      memOp->setStart(weight_offset);
      weight_offset += tensor_size;
    }
    memOp->setLength(tensor_size);
    allocatedValue.insert({memVal, memOp});
  }
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
