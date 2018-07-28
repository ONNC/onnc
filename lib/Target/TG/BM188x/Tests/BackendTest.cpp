//===- BackendTest.cpp -----------------------------------------------------===//
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

#include "../BM188xBackend.h"
#include "../QuantizePass.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// Backend Test
//===----------------------------------------------------------------------===//
SKYPAT_F(BM188xTest, bm188x_pass_management)
{
  Path path(TOPDIR);
  path.append("tools")
      .append("unittests")
      .append("data")
      .append("lenet")
      .append("model.onnx");
  onnc::onnx::Reader reader;

  TargetOptions options;
  options.m_AddDummyWeight = true;

  // BM188xBackend passes, the golden.
  onnc::Module golden_module;
  BM1880Backend golden_backend(options);
  {
    SystemError err = reader.parse(path, golden_module);
    ASSERT_TRUE(err.isGood());

    PassRegistry registry;
    PassManager pm(registry);

    golden_backend.addTensorSel(pm);
    golden_backend.addTensorSched(pm);
    golden_backend.addMemAlloc(pm);
    golden_backend.addCodeEmit(pm, "-");

    // Print pass names in execution order.
    errs() << "ExecutionOrder: ";
    PassManager::ExecutionOrder passes = pm.state().execution;
    for (PassManager::ExecutionOrder::iterator it = passes.begin(); it != passes.end(); ++it) {
      errs() << " -> " << pm.lookup(*it)->getPassName();
    }
    errs() << std::endl;

    // Run passes step by step
    for (size_t i = 0; i < pm.size(); ++i) {
      pm.step(golden_module);
      errs() << pm.state().pass->getPassName() << std::endl;
      //golden_module.print(errs());
    }
  }
  
  // passes under test
  onnc::Module test_module;
  BM1880Backend test_backend(options);
  {
    SystemError err = reader.parse(path, test_module);
    ASSERT_TRUE(err.isGood());
    
    PassRegistry registry;
    PassManager pm(registry);

    pm.add(CreateRemoveTrainingNodesPass());
    pm.add(createAddDummyWeightPass( &test_backend ));
    pm.add(CreateUpdateGraphOutputSizePass());
    pm.add(createPrepareCtablePass( &test_backend ));
    pm.add(createONNXFuseOptPass( &test_backend ));
    pm.add(createTargetLoweringPass( &test_backend ));
    pm.add(CreateQuantizePass( &test_backend ));
    pm.add(createUpdateCtablePass( &test_backend ));
    pm.add(CreateGlobalMemAllocPass( &test_backend ));
    pm.add(createTGCodeEmitPass( &test_backend, "-" ));

    // Print pass names in execution order.
    errs() << "ExecutionOrder: ";
    PassManager::ExecutionOrder passes = pm.state().execution;
    for (PassManager::ExecutionOrder::iterator it = passes.begin(); it != passes.end(); ++it) {
      errs() << " -> " << pm.lookup(*it)->getPassName();
    }
    errs() << std::endl;

    // Run passes step by step
    for (size_t i = 0; i < pm.size(); ++i) {
      pm.step(test_module);
      errs() << pm.state().pass->getPassName() << std::endl;
      //test_module.print(errs());
    }
  }

  // Compare instructions.
  typedef std::vector<std::unique_ptr<ComputeOperator2> > InstVector;
  InstVector& golden_insts = golden_backend.getInsts();
  InstVector& test_insts = test_backend.getInsts();
  ASSERT_TRUE(golden_insts.size() > 0);
  ASSERT_TRUE(golden_insts.size() == test_insts.size());

  errs() << "Insts (# = " << golden_insts.size() << "): ";
  for (InstVector::iterator it = golden_insts.begin(); it != golden_insts.end(); ++it) {
    const std::string& name = (*it)->getLayerName();
    errs() << " " << name;

    bool name_is_found = false;
    for (InstVector::iterator t_it = test_insts.begin(); t_it != test_insts.end(); ++t_it) {
      if (name == (*t_it)->getLayerName()) {
        name_is_found = true;
        break;
      }
    }
    ASSERT_TRUE(name_is_found);
  }
  errs() << std::endl;
  
  // Compare memory operands
  typedef std::vector<MemOperand *> MemVector;
  MemVector& golden_mem = golden_backend.getMemOperands();
  MemVector& test_mem = test_backend.getMemOperands();
  ASSERT_TRUE(golden_mem.size() == test_mem.size());

  errs() << "MemOperands (# = " << golden_mem.size() << "): ";
  for (MemVector::iterator it = golden_mem.begin(); it != golden_mem.end(); ++it) {
    const std::string& name = (*it)->m_Name;
    errs() << " " << name;
    
    bool name_is_found = false;
    for (MemVector::iterator t_it = test_mem.begin(); t_it != test_mem.end(); ++t_it) {
      if (name == (*t_it)->m_Name) {
        name_is_found = true;
        break;
      }
    }
    ASSERT_TRUE(name_is_found);
  }
  errs() << std::endl;

}


