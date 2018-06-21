//===- InitializePasses.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/InitializePasses.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Batch initialization
//===----------------------------------------------------------------------===//
void onnc::InitializeAnalysis(PassRegistry& pRegistry)
{
  InitializeGraphLivenessAnalysisPass(pRegistry);
  InitializeMemoryAllocationPass(pRegistry);
  InitializeUpdateGraphOutputSizePass(pRegistry);
}

void onnc::InitializeAnalysisPassOptions()
{
  InitializeUpdateGraphOutputSizePassOptions();
}
