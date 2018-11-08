//===- PerfEvent.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_PERFEVENT_H
#define ONNC_SUPPORT_PERFEVENT_H
enum PerfEvent {
// type is PERF_TYPE_HARDWARE
  CPU_CYCLES, // = 0
  INSTRUCTIONS,
  CACHE_REFERENCES,
  CACHE_MISSES,
  BRANCH_INSTRUCTIONS,
  BRANCH_MISSES,
  BUS_CYCLES,
  STALLED_CYCLES_FRONTEND,
  STALLED_CYCLES_BACKEND,
  REF_CPU_CYCLES, // = 9
// type is PERF_TYPE_SOFTWARE
  CPU_CLOCK, // = 10
  TASK_CLOCK,
  PAGE_FAULTS,
  CONTEXT_SWITCHES,
  CPU_MIGRATIONS,
  PAGE_FAULTS_MIN,
  PAGE_FAULTS_MAJ,
  ALIGNMENT_FAULTS,
  EMULATION_FAULTS // = 18
};

enum PerfEventCache{
// type is PERF_TYPE_HW_CACHE
// TODO: add CPU cache events
};

#endif
