//===- LiveInterval.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/LiveInterval.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ValueIndex
//===----------------------------------------------------------------------===//
ValueIndex::ValueIndex(onnc::Value& pValue)
  : m_pValue(&pValue) {
}

//===----------------------------------------------------------------------===//
// TimeSlot
//===----------------------------------------------------------------------===//
TimeSlot::TimeSlot(ComputeOperator& pOp, unsigned int pIdx)
  : m_Idx(pIdx), m_pOperator(&pOp) {
}

//===----------------------------------------------------------------------===//
// SlotIndex
//===----------------------------------------------------------------------===//
SlotIndex::SlotIndex()
  : m_pTimeSlot(nullptr) {
}

SlotIndex::SlotIndex(TimeSlot* pSlot)
  : m_pTimeSlot(pSlot) {
}
