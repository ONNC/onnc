//===-- TargetBackend.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Transforms/TensorSel.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// TargetBackend
//===----------------------------------------------------------------------===//
void TargetBackend::addStandardTensorSel(PassManager& pPM) {
  // ONNC is currently for Inferencing only. So we just remove the nodes that
  // only operate at training phase.
  pPM.add(CreateRemoveTrainingNodesPass());
  // Do the shape inference. The standard lower needs shape information.
  pPM.add(CreateUpdateGraphOutputSizePass());
  // Remove unused nodes. Standard Lower doesn't handle the "undefined" node.
  pPM.add(CreateDeadNodeEliminationPass());

  // Create ComputeGraph.
  pPM.add(CreateBookONNXGraphs());
  // Build the Initializer Operator (for the Weights).
  pPM.add(CreateBuildInitializers());
  // Build the Input Operator (for the Inputs).
  pPM.add(CreateBuildInputOperators());
  // Do the standard Tensor Selection, which uses LowerRegistry to get the
  // standard Lower, so you have to implement TargetBackend::RegisterLowers().
  pPM.add(CreateTensorSel(this));
  // Build the Output Operator (for the Outputs).
  pPM.add(CreateBuildOutputOperators());
}
