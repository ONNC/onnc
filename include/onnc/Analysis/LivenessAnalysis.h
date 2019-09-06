//===- LivenessAnalysis.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_LIVENESS_ANALYSIS_H
#define ONNC_LIVENESS_ANALYSIS_H
#include <onnc/Core/CustomPass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Config/ONNX.h>
#include <string>
#include <iosfwd>
#include <vector>

namespace onnc {

namespace analysis {

/** \class LiveInterval
 */
class LiveInterval
{
public:
  typedef unsigned SlotIndex;

public:
  LiveInterval(SlotIndex pStart, SlotIndex pEnd, const xValue& pValue);

  SlotIndex getStart() const { return m_Start; }

  SlotIndex getEnd() const { return m_End; }

  const xValue& getValue() const { return m_Value; }

  /// return true if two live intervals have intersection
  bool intersect(const LiveInterval& pLive) const;

protected:
  // Live interval = [start, end]
  SlotIndex m_Start;
  SlotIndex m_End;
  const xValue& m_Value;
};

} // namespace analysis

/** \class GraphLivenessAnalysis
 */
class GraphLivenessAnalysis : public CustomPass<GraphLivenessAnalysis>
{
public:
  typedef std::vector<analysis::LiveInterval*> LiveIntervalList;

public:
  GraphLivenessAnalysis() = default;

  ReturnType runOnModule(Module& pModule) override;

  ReturnType runOnGraph(xGraph &pGraph);

  const LiveIntervalList& getLiveIntervals() const { return m_LiveIntervals; }

  void print(OStream& pOS, const Module* pModule) const override;

  /// delete LiveIntervals in m_LiveIntervals
  void clear() override;

private:
  void calculateLiveness(xGraph &pGraph);

private:
  LiveIntervalList m_LiveIntervals;
};

GraphLivenessAnalysis *CreateLivenessAnalysisPass();

} // namespace of onnc

#endif
