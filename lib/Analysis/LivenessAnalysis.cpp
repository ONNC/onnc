//===- LivenessAnalysis.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/LivenessAnalysis.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <unordered_map>
#include <iosfwd>
#include <cassert>
#include <algorithm>
#include <unordered_map>

using namespace onnc;

typedef std::unordered_map<onnx::Value *, unsigned> ValueIdxMap;
typedef std::unordered_map<onnx::Node*, unsigned> NodeIdxMap;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static void BuildVirtualIndex(onnx::Graph &pGraph,
                              ValueIdxMap& pValueVirIdxMap,
                              NodeIdxMap& pNodeVirIdxMap)
{
  // graph-ir example:
  // graph net(tensor<..> %data_0,
  //           tensor<..> %conv1_w_0, ..) {
  //   initialize input %conv1_w_0 = data tensor<..>
  //   ..
  // }
  //
  // graph inputs (parameters) = %data_0, %conv1_w_0, ...
  // data initialize = %conv1_w_0, ..
  //
  // If the inputs have no initializer, give it '0' index.

  // assign a virtual index to each input and onnx::Value.
  // '0' is preserved for input parameters.
  unsigned virIdx = 1;

  // assign virtual index to data initializers.
  std::unordered_map<std::string, unsigned> initializerVirIdxMap;
  for (const onnx::Tensor &t : pGraph.initializers())
    initializerVirIdxMap[t.name()] = virIdx++;

  // assign virtual index to graph inputs.
  for (onnx::Value *v : pGraph.inputs()) {
    auto iter = initializerVirIdxMap.find(v->uniqueName());
    // parameters without initializer has virIdx = 0
    if (iter != initializerVirIdxMap.end())
      pValueVirIdxMap[v] = iter->second;
    else
      pValueVirIdxMap[v] = 0;
  }

  // assign virtual index to each node's output.
  for (onnx::Node *n : pGraph.nodes()) {
    for (onnx::Value *v : n->outputs())
      pValueVirIdxMap[v] = virIdx;
    pNodeVirIdxMap[n] = virIdx++;
  }
}

//===----------------------------------------------------------------------===//
// LiveInterval
//===----------------------------------------------------------------------===//
LiveInterval::LiveInterval(SlotIndex pStart, SlotIndex pEnd,
                           const onnx::Value& pValue)
  : m_Start(pStart), m_End(pEnd), m_Value(pValue) {
  assert(m_Start <= m_End && "Invalid live interval.");
}

//===----------------------------------------------------------------------===//
// GraphLivenessAnalysis
//===----------------------------------------------------------------------===//
GraphLivenessAnalysis::GraphLivenessAnalysis()
  : ModulePass(ID), m_LiveIntervals() {
}

bool GraphLivenessAnalysis::runOnModule(Module &pModule)
{
  clear();
  calculateLiveness(*pModule.getGraph());
  return false;
}

void GraphLivenessAnalysis::print(std::ostream& pOS) const
{
  for (const LiveInterval *li : m_LiveIntervals) {
    pOS << li->getValue().uniqueName()
        << " [" << li->getStart() << ", " << li->getEnd() << "]"
        << std::endl;
  }
}

void GraphLivenessAnalysis::calculateLiveness(onnx::Graph &pGraph)
{
  // Basically, node's virtual index value can be calculated from it's output
  // value (onnx::Value), but some of nodes may not have output, so use
  // nodeVirIdxMap to record each node's index.
  std::unordered_map<onnx::Value *, unsigned>  valueVirIdxMap;
  std::unordered_map<onnx::Node *, unsigned> nodeVirIdxMap;
  BuildVirtualIndex(pGraph, valueVirIdxMap, nodeVirIdxMap);

  // calculate initializer's live range
  for (onnx::Value *v : pGraph.inputs()) {
    unsigned start = valueVirIdxMap[v];
    unsigned end = start;
    for(auto u : v->uses())
      end = std::max(end, nodeVirIdxMap[u.user]);
    m_LiveIntervals.push_back(new LiveInterval(start, end, *v));
  }

  // calculate live range for each output of a node
  for (onnx::Node *n : pGraph.nodes()) {
    for (onnx::Value *v : n->outputs()) {
      unsigned start = valueVirIdxMap[v];
      unsigned end = start;
      for(auto u : v->uses())
        end = std::max(end, nodeVirIdxMap[u.user]);

      m_LiveIntervals.push_back(new LiveInterval(start, end, *v));
    }
  } // for each node
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

GraphLivenessAnalysis *onnc::CreateLivenessAnalysisPass()
{
  return new GraphLivenessAnalysis();
}
