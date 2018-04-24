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
#include <onnx/common/ir.h>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LiveInterval
//===----------------------------------------------------------------------===//
LiveInterval::LiveInterval(SlotIndex pStart, SlotIndex pEnd,
                           onnx::Value *pValue)
  : m_Start(pStart), m_End(pEnd), m_Value(pValue) {
  assert(m_Start <= m_End && "Invalid live interval.");
}

//===----------------------------------------------------------------------===//
// GraphLivenessAnalysis
//===----------------------------------------------------------------------===//
GraphLivenessAnalysis::GraphLivenessAnalysis()
  : ModulePass(ID) {
}

bool GraphLivenessAnalysis::runOnModule(Module &pModule)
{
  onnx::Graph *graph = pModule.getGraph();
  m_LiveIntervals.clear();
  calculateLiveness(*graph);

  return false;
}

std::string GraphLivenessAnalysis::toString() const
{
  std::ostringstream oss;
  for (const LiveInterval &li : m_LiveIntervals)
    oss << li.getValue()->uniqueName()
        << " [" << li.getStart() << ", " << li.getEnd() << "]"
        << std::endl;
  return oss.str();
}

const std::vector<LiveInterval> &
GraphLivenessAnalysis::getLiveIntervals() const
{
  return m_LiveIntervals;
}

void GraphLivenessAnalysis::buildVirtualIndex(
                  onnx::Graph &graph,
                  std::unordered_map<onnx::Value *, unsigned> &pValueVirIdxMap,
                  std::unordered_map<onnx::Node *, unsigned> &pNodeVirIdxMap)
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
  for (const onnx::Tensor &t : graph.initializers())
    initializerVirIdxMap[t.name()] = virIdx++;

  // assign virtual index to graph inputs.
  for (onnx::Value *v : graph.inputs()) {
    auto iter = initializerVirIdxMap.find(v->uniqueName());
    // parameters without initializer has virIdx = 0
    if (iter != initializerVirIdxMap.end())
      pValueVirIdxMap[v] = iter->second;
    else
      pValueVirIdxMap[v] = 0;
  }

  // assign virtual index to each node's output.
  for (onnx::Node *n : graph.nodes()) {
    for (onnx::Value *v : n->outputs())
      pValueVirIdxMap[v] = virIdx;
    pNodeVirIdxMap[n] = virIdx++;
  }
}

void GraphLivenessAnalysis::calculateLiveness(onnx::Graph &graph)
{
  // Basically, node's virtual index value can be calculated from it's output
  // value (onnx::Value), but some of nodes may not have output, so use
  // nodeVirIdxMap to record each node's index.
  std::unordered_map<onnx::Value *, unsigned>  valueVirIdxMap;
  std::unordered_map<onnx::Node *, unsigned> nodeVirIdxMap;
  buildVirtualIndex(graph, valueVirIdxMap, nodeVirIdxMap);

  // calculate initializer's live range
  for (onnx::Value *v : graph.inputs()) {
    unsigned start = valueVirIdxMap[v];
    unsigned end = start;
    for(auto u : v->uses())
      end = std::max(end, nodeVirIdxMap[u.user]);
    m_LiveIntervals.push_back(LiveInterval(start, end, v));
  }

  // calculate live range for each output of a node
  for (onnx::Node *n : graph.nodes()) {
    for (onnx::Value *v : n->outputs()) {
      unsigned start = valueVirIdxMap[v];
      unsigned end = start;
      for(auto u : v->uses())
        end = std::max(end, nodeVirIdxMap[u.user]);

      m_LiveIntervals.push_back(LiveInterval(start, end, v));
    }
  }
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char GraphLivenessAnalysis::ID = 0;

GraphLivenessAnalysis *onnc::createLivenessAnalysisPass()
{
  return new GraphLivenessAnalysis();
}
