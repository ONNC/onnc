//===- LivenessAnalysis.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_LIVENESS_ANALYSIS_H
#define ONNC_LIVENESS_ANALYSIS_H

#include <onnc/Core/Pass.h>
#include <onnc/Core/ModulePass.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace onnx {
  struct Value;
  struct Node;
  struct Graph;
}

namespace onnc {

typedef unsigned SlotIndex;

/** \class LiveInterval
 */
class LiveInterval 
{
public:
  LiveInterval(SlotIndex pStart, SlotIndex pEnd, onnx::Value *pValue);
  LiveInterval() {}

  SlotIndex getStart() const { return m_Start; }
  SlotIndex getEnd() const { return m_End; }
  onnx::Value * getValue() const { return m_Value; }

protected:
  // Live interval = [start, end]
  SlotIndex m_Start;
  SlotIndex m_End;
  onnx::Value *m_Value;
};

/** \class GraphLivenessAnalysis
 */
class GraphLivenessAnalysis : public ModulePass
{
public:
  static char ID;

public:
  GraphLivenessAnalysis();
  bool runOnModule(Module &pModule) override;
  std::string toString() const;
  const std::vector<LiveInterval> & getLiveIntervals() const;

private:
  void buildVirtualIndex(onnx::Graph &graph,
                   std::unordered_map<onnx::Value *, unsigned> &pValueVirIdxMap,
                   std::unordered_map<onnx::Node *, unsigned> &pNodeVirIdxMap);
  void calculateLiveness(onnx::Graph &graph);

  std::vector<LiveInterval> m_LiveIntervals;
};

GraphLivenessAnalysis *createLivenessAnalysisPass();

} // namespace of onnc

#endif