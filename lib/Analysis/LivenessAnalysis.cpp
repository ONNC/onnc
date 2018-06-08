//===- LivenessAnalysis.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/LivenessAnalysis.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <unordered_map>
#include <iosfwd>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace onnc;

typedef std::unordered_map< ::onnx::Value *, unsigned> ValueIdxMap;
typedef std::unordered_map< ::onnx::Node *, unsigned> NodeIdxMap;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static void BuildVirtualIndex(::onnx::Graph &pGraph,
                              ValueIdxMap &pValueVirIdxMap,
                              NodeIdxMap &pNodeVirIdxMap)
{
  // Given:
  //    %1     = f(%d0, %d1)
  //    %2     = g(%1)
  //    %3, %4 = h(%2)
  // Result:
  //    VirIdx
  //    0       %1     = f(%d0, %d1)
  //    1       %2     = g(%1)
  //    2       %3, %4 = h(%2)
  //
  // Virtual index start from 'zero', the first node has index 0.
  unsigned virIdx = 0;

  // assign virtual index to each node's output.
  for (::onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == ::onnx::kUndefined)
      continue;

    for (::onnx::Value *v : n->outputs())
      pValueVirIdxMap[v] = virIdx;
    pNodeVirIdxMap[n] = virIdx++;
  }
}

//===----------------------------------------------------------------------===//
// LiveInterval
//===----------------------------------------------------------------------===//
LiveInterval::LiveInterval(SlotIndex pStart, SlotIndex pEnd,
                           const ::onnx::Value &pValue)
    : m_Start(pStart), m_End(pEnd), m_Value(pValue) {
  assert(m_Start <= m_End && "Invalid live interval.");
}

bool LiveInterval::intersect(const LiveInterval& pLive) const
{
  return !(pLive.m_End < m_Start || m_End < pLive.m_Start);
}

//===----------------------------------------------------------------------===//
// GraphLivenessAnalysis
//===----------------------------------------------------------------------===//
GraphLivenessAnalysis::GraphLivenessAnalysis()
  : ModulePass(ID), m_LiveIntervals() {
}

Pass::ReturnType GraphLivenessAnalysis::runOnModule(Module &pModule)
{
  clear();
  calculateLiveness(*pModule.getGraphIR());
  return kModuleNoChanged;
}

void GraphLivenessAnalysis::print(std::ostream& pOS) const
{
  for (const LiveInterval *li : m_LiveIntervals) {
    pOS << li->getValue().uniqueName()
        << " [" << li->getStart() << ", " << li->getEnd() << "]"
        << "\n";
  }
}

void GraphLivenessAnalysis::calculateLiveness(::onnx::Graph &pGraph)
{
  // Basically, node's virtual index value can be calculated from it's output
  // value (onnx::Value), but some of nodes may not have output, so use
  // nodeVirIdxMap to record each node's index.
  std::unordered_map< ::onnx::Value *, unsigned> valueVirIdxMap;
  std::unordered_map< ::onnx::Node *, unsigned> nodeVirIdxMap;
  BuildVirtualIndex(pGraph, valueVirIdxMap, nodeVirIdxMap);

  // Values that have been visited.
  std::unordered_set< ::onnx::Value *> visited;

  // calculate live range for each output of a node
  for (::onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == ::onnx::kUndefined)
      continue;

    for (::onnx::Value *v : n->outputs()) {
      unsigned start = valueVirIdxMap[v];
      unsigned end = start;
      for(auto u : v->uses())
        end = std::max(end, nodeVirIdxMap[u.user]);

      visited.insert(v);
      m_LiveIntervals.push_back(new LiveInterval(start, end, *v));
    }
  } // for each node

  // calculate live range graph's input parameters.
  // input parameters' live range dependent on the first and last node which
  // use this input.
  for (::onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == ::onnx::kUndefined)
      continue;

    unsigned nVid = nodeVirIdxMap[n];
    for (::onnx::Value *v : n->inputs()) {
      if (visited.count(v))
        continue;

      unsigned start = nVid;
      unsigned end = nVid;
      for(auto u : v->uses()) {
        unsigned uVid = nodeVirIdxMap[u.user];
        start = std::min(start, uVid);
        end = std::max(end, uVid);
      }

      visited.insert(v);
      m_LiveIntervals.push_back(new LiveInterval(start, end, *v));
    }
  } // for each node

  // Sort live intervals by start slot.
  std::sort(m_LiveIntervals.begin(), m_LiveIntervals.end(),
            [](const LiveInterval* lia, const LiveInterval* lib) {
              return lia->getStart() < lib->getStart();
            });
}

void GraphLivenessAnalysis::clear()
{
  for (LiveInterval* live: m_LiveIntervals) {
    delete live;
  }
  m_LiveIntervals.clear();
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char GraphLivenessAnalysis::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(GraphLivenessAnalysis, "GraphLivenessAnalysis")
}

GraphLivenessAnalysis *onnc::CreateLivenessAnalysisPass()
{
  return new GraphLivenessAnalysis();
}
