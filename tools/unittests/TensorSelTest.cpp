//===- TensorSelTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/Core/PassManager.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/ADT/Rope.h>
#include <onnc/Support/OFStream.h>

#include <onnc/Transforms/TensorSel/Standards/ATenLower.h>
#include <onnc/Transforms/TensorSel/Standards/AveragePoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/SumLower.h>
#include <onnc/Transforms/TensorSel/Standards/AbsLower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>
#include <onnc/Transforms/TensorSel/Standards/UpsampleLower.h>
#include <onnc/Transforms/TensorSel/Standards/AcosLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConcatLower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxPoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/XorLower.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConvLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/AffineLower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReshapeLower.h>
#include <onnc/Transforms/TensorSel/Standards/AndLower.h>
#include <onnc/Transforms/TensorSel/Standards/LRNLower.h>
#include <onnc/Transforms/TensorSel/Standards/SoftmaxLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Any Test
//===----------------------------------------------------------------------===//
SKYPAT_F(TensorSelTest, alexnet)
{
  Path path(TOPDIR);
  path.append("tools")
      .append("unittests")
      .append("data")
      .append("bvlc_alexnet")
      .append("model.onnx");
  onnc::Module module;
  onnc::onnx::Reader reader;
  SystemError err = reader.parse(path, module);
  ASSERT_TRUE(err.isGood());

  module.dump();

  PassRegistry registry;
  PassManager pm(registry);
  PassManager::State state;

  pm.add(CreateRemoveTrainingNodesPass(), state);
  pm.add(CreateUpdateGraphOutputSizePass(), state);
  pm.add(CreateDeadNodeEliminationPass(), state);
  pm.add(CreateBookONNXGraphs(), state);
  pm.add(CreateBuildInitializers(), state);
  pm.add(CreateBuildInputOperators(), state);

  /// create tensor selection
  TensorSel* tensor_selection = new TensorSel();
  tensor_selection->getLowerRegistry().emplace<onnc::ATenLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AveragePoolLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::SumLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AbsLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::BatchNormalizationLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::UpsampleLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AcosLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::ConcatLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::MaxPoolLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::XorLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AddLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::ConvLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::ReluLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AffineLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::GemmLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::ReshapeLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AndLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::LRNLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::SoftmaxLower>();
  pm.add(tensor_selection, state);
  pm.add(CreateBuildOutputOperators(), state);

  // RemoveTrainingNodesPass
  int counter = 0;
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // UpdateGraphOutputSizePass
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // DeadNodeEliminationPass
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // BookONNXGraphs
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // BuildInitializers
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // BuildInputOperators
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // TensorSel 
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // BuildOutputOperators
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  //ASSERT_TRUE(state.execution.empty());
  //ASSERT_TRUE(state.changed);
  //int run_result = pm.run(module);
  //ASSERT_TRUE(EXIT_SUCCESS == run_result);
}
