//===- InitializePasses.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_INITIALIZE_PASSES_H
#define ONNC_CORE_INITIALIZE_PASSES_H

namespace onnc {

class PassRegistry;

/// Initialize all passes in Analysis library.
void InitializeAnalysis(PassRegistry&);

/// Initialize all pass options in Analysis library.
void InitializeAnalysisPassOptions();

//===----------------------------------------------------------------------===//
// Declarations of every single pass
//===----------------------------------------------------------------------===//
/// Analysis Pass:
void* InitializeGraphLivenessAnalysisPass(PassRegistry&);
void* InitializeMemoryAllocationPass(PassRegistry&);
void* InitializeUpdateGraphOutputSizePass(PassRegistry&);
void* InitializeNodeIRSchedulerPass(PassRegistry&);

/// CodeGen Pass:
void* InitializeBuildMemOperandPass(PassRegistry&);
void* InitializeBuildSlotIndexesPass(PassRegistry&);
void* InitializeFuseInplaceValuePass(PassRegistry&);
void* InitializeLinearScanMemAllocPass(PassRegistry&);
void* InitializeLiveIntervalsPass(PassRegistry&);
void* InitializeLiveIntervalsDataPass(PassRegistry&);
void* InitializeLiveValueMatrixPass(PassRegistry&);
void* InitializeMemAllocDataPass(PassRegistry&);
void* InitializeSetMemOperandPass(PassRegistry&);

void InitializeUpdateGraphOutputSizePassOptions();

} // namespace of onnc

#endif
