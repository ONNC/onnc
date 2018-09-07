//===- LiveValueMatrix.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_LIVE_VALUE_MATRIX_H
#define ONNC_CODEGEN_LIVE_VALUE_MATRIX_H
#include <onnc/ADT/IListNode.h>
#include <onnc/CodeGen/LiveInterval.h>

namespace onnc {

/** \class LiveValueMatrix
 *  \brief Track live interference between onnc::Value pairs. Provide utilities
 *         for live interference queries.
 *
 *         Live interference is used by memory allocation pass to correctly
 *         allocate non-overlapping physical memory for values that have
 *         interfering live range. 
 */
class LiveValueMatrix : public ModulePass
{
public:
  struct LiveSegment
  {
    LiveInterval* m_LI;
    LiveRange::Segment m_Segment;

    LiveSegment(LiveInterval* pLI, const LiveRange::Segment& pSeg)
      : m_LI(pLI), m_Segment(pSeg) {
    }
  };

  static char ID;

  typedef std::vector<LiveInterval*> LIs;

  typedef std::unordered_map<LiveInterval*, LIs> OverlapLIs;

  typedef std::vector<LiveSegment> LiveSegs;

  typedef std::vector<LiveSegs> ListOfLiveSegs;

  typedef std::set<LiveSegment> LiveSegSet;

  typedef std::unordered_map<LiveInterval*, unsigned> LICount;

public:
  LiveValueMatrix()
    : ModulePass(ID) {
  }

  virtual ~LiveValueMatrix() { clear(); }

  /// Interference query interface.
  /// Time Complexity: O(1)
  const LIs& getInterferingLiveIntervals(const Value* pV) const;

  const LIs& getInterferingLiveIntervals(const LiveInterval* pLI) const;

  ReturnType runOnModule(Module& pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

  StringRef getPassName() const override { return "LiveValueMatrix"; }

  void print(std::ostream& pOS) const;

  void dump() const;

private:
  /// Delete LiveInterval object.
  void clear();

  /// For each SlotIndex (ComputeOperator):
  /// 1. Find live intervals which start from SlotIndex i, then add these
  ///    intervals' segments to m_StartWith[i]
  ///
  /// 2. Find live intervals which end with SlotIndex i, then add these
  ///    intervals' segments to m_EndWith[i]
  ///
  /// Time Complexity: O(#LiveSegments)
  void buildStartWithEndWith();

  void runOnComputeGraph(ComputeGraph& pCG);

  /// Time Complexity:
  ///   O(#LiveSegments ^ 2)
  void calculateOverlapSet(unsigned pSlotIdx);

private:
  /// Overlaps result.
  /// If i is a LiveInterval, then m_OverlapLIs[i] =
  ///    { Live Intervals set which overlap with i }
  OverlapLIs m_OverlapLIs;

  /// Intermediate data structure for calculating overlaps.

  /// m_StartWith[i]: LiveIntervals which begin at SlotIndex i
  /// m_EndWith[i]: LiveIntervals which end at SlotIndex i
  ListOfLiveSegs m_StartWith, m_EndWith;

  /// Current live segment set
  LiveSegSet m_CurLiveSegSet;

  /// User count of a LiveInterval in current live segment set.
  LICount m_CurLISet;

  LiveIntervals* m_LIPass;
};

LiveValueMatrix* CreateLiveValueMatrixPass();

} // namespace of onnc

#endif