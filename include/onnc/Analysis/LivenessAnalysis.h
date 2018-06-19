//===- LivenessAnalysis.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_LIVENESS_ANALYSIS_H
#define ONNC_LIVENESS_ANALYSIS_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>
#include <string>
#include <iosfwd>
#include <vector>

namespace onnc {

/** \class LiveInterval
 */
class LiveInterval
{
public:
  typedef unsigned SlotIndex;

public:
  LiveInterval(SlotIndex pStart, SlotIndex pEnd, const ::onnx::Value& pValue);

  SlotIndex getStart() const { return m_Start; }

  SlotIndex getEnd() const { return m_End; }

  const ::onnx::Value& getValue() const { return m_Value; }

  /// return true if two live intervals have intersection
  bool intersect(const LiveInterval& pLive) const;

protected:
  // Live interval = [start, end]
  SlotIndex m_Start;
  SlotIndex m_End;
  const ::onnx::Value& m_Value;
};

/** \class GraphLivenessAnalysis
 */
class GraphLivenessAnalysis : public ModulePass
{
public:
  static char ID;

  typedef std::vector<LiveInterval*> LiveIntervalList;

public:
  GraphLivenessAnalysis();

  ReturnType runOnModule(Module& pModule) override;

  ReturnType runOnGraph(onnx::Graph &pGraph);

  const LiveIntervalList& getLiveIntervals() const { return m_LiveIntervals; }

  void print(std::ostream& pOS) const;

private:
  void calculateLiveness(::onnx::Graph &pGraph);

  /// delete LiveIntervals in m_LiveIntervals
  void clear();

private:
  LiveIntervalList m_LiveIntervals;
};

GraphLivenessAnalysis *CreateLivenessAnalysisPass();

} // namespace of onnc

#endif
