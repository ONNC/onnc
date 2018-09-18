//===- SlotIndexes.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/Core/PassSupport.h>
#include <iomanip>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BuildSlotIndexes
//===----------------------------------------------------------------------===//
Pass::ReturnType BuildSlotIndexes::runOnModule(Module& pModule)
{
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg)
    runOnComputeGraph(*cg->value());
  return Pass::kModuleNoChanged;
}

void BuildSlotIndexes::runOnComputeGraph(ComputeGraph& pCG)
{
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    ComputeOperator *node = nodeIt;
    TimeSlot* ts = createTimeSlot(*node, m_StartIdx);
    m_TSList.push_back(ts);
    m_COpToSlotIdx.insert({node, SlotIndex(ts)});
    m_StartIdx += OperatorDist;
  }
}

bool BuildSlotIndexes::hasSlotIndex(const ComputeOperator* pOp) const
{
  return m_COpToSlotIdx.find((ComputeOperator*)pOp) != m_COpToSlotIdx.end();
}

SlotIndex BuildSlotIndexes::getSlotIndex(const ComputeOperator* pOp) const
{
  assert(hasSlotIndex(pOp) && "Can't find slotIndex.");
  return m_COpToSlotIdx.find((ComputeOperator*)pOp)->second;
}

TimeSlot* BuildSlotIndexes::createTimeSlot(ComputeOperator& pOp, unsigned pIdx)
{
  return new TimeSlot(pOp, pIdx);
}

void BuildSlotIndexes::clear()
{
  m_COpToSlotIdx.clear();
  while (!m_TSList.empty()) {
    TimeSlot* ts = &m_TSList.front();
    m_TSList.pop_front();
    delete ts;
  }
  m_StartIdx = 0;
}

void BuildSlotIndexes::print(OStream& pOS, const Module* pModule) const
{
  if (m_TSList.empty())
    return;

  std::stringstream dbgstr;

  dbgstr << "SlotIndexes:\n";
  for (const TimeSlot& ts : m_TSList) {
    dbgstr << std::left << std::setw(20) << ts.getOperator()->name()
           << std::right << std::setw(4) << ts.getIndex() << "\n";
  }
  pOS << dbgstr.str();
}

//===----------------------------------------------------------------------===//
// BuildSlotIndexes Factory method
//===----------------------------------------------------------------------===//
char BuildSlotIndexes::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(BuildSlotIndexes, "BuildSlotIndexes")
}

BuildSlotIndexes* onnc::CreateBuildSlotIndexesPass()
{
  return new BuildSlotIndexes();
}