//===- TargetStandardPasses.h ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_STANDARD_PASSES_H
#define ONNC_TARGET_TARGET_STANDARD_PASSES_H
#include <onnc/Option/CommandLine.h>

namespace onnc {

extern cl::opt<std::string> LinearScanAlgo;

class PassManager;
class TargetBackend;

/// This is helper function to add passes for building up standard ONNC IR.
void addStandardTensorSel(PassManager& pPM, TargetBackend& pTB);

/// Add standard passes for creating value's live interval.
///
/// Input: Module
/// Output: LiveIntervalsData
void addStandardCreateLiveIntervals(PassManager& pPM);

/// Add standard memory allocation passes.
///
/// Input: LiveIntervalsData
/// Output: MemAllocsData
void addStandardMemoryAllocation(PassManager& pPM, TargetBackend& pTB);

/// Add standard passes for setting memory operands according to memory
/// allocation result.
///
/// Input: MemAllocsData
/// Output: Virtual memory address for each memory operands.
void addStandardSetMemOperands(PassManager& pPM);

} // namespace of onnc

#endif
