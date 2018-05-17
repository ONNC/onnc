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

//===----------------------------------------------------------------------===//
// Declarations of every single pass
//===----------------------------------------------------------------------===//
void InitializeGraphLivenessAnalysisPass(PassRegistry&);
void InitializeMemoryAllocationPass(PassRegistry&);
void InitializeUpdateGraphOutputSizePass(PassRegistry&);

} // namespace of onnc

#endif
