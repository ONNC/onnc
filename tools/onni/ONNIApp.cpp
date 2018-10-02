//===- ONNIApp.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNIApp.h"
#include "Interpreter.h"
#include <cstdlib>
#include <onnc/Target/TargetSelect.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Target/TargetOptions.h>
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/IR/Module.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Core/PassManager.h>
#include <onnc/ADT/Color.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Casting.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <cassert>

extern "C" {
#include "onnc-runtime.h"
}

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNIApp
//===----------------------------------------------------------------------===//
ONNIApp::ONNIApp(int pArgc, char* pArgv[])
  : onnc::CoreApplication(pArgc, pArgv),
    m_Options() {
  InitializeAllPlatforms();
  InitializeAllBackends();
}

ONNIApp::~ONNIApp()
{
}

int ONNIApp::run()
{
  onnc::onnx::Reader reader;
  Module module;
  SystemError err = reader.parse(options().model(), module);
  if (!err.isGood()) {
    // TODO: show error message
    return EXIT_FAILURE;
  }

  std::string error;
  std::string quadruple;
  options().quadruple().canonical(quadruple);
  const onnc::Target* target = TargetRegistry::Lookup(quadruple, error);
  if (nullptr == target) {
    errs() << Color::RED << "Error" << Color::RESET
           << ": can not found target `" << quadruple << "`: " << error
           << std::endl;
    return EXIT_FAILURE;
  }

  PassManager pm;
  TargetBackend* backend = target->createBackend(options().target());
  backend->addTensorSel(pm);
  backend->addMemAlloc(pm);

  pm.run(module);

  // TODO: Refactor
  void *runtime_context = ONNC_RUNTIME_init_runtime();

  Interpreter interpreter(runtime_context);

  // XXX: Use onnc-runtime to handle memory
  char *input_mem = NULL;
  char *heap = NULL;
  // FIXME: Use onnc-runtime to handle input
  {
    ::onnx::TensorProto tensor;
    std::ifstream input_fin(options().input().native());
    tensor.ParseFromIstream(&input_fin);
    const std::string &raw_data_str = tensor.raw_data();
    input_mem = new char[raw_data_str.size()];
    memcpy(input_mem, raw_data_str.data(), raw_data_str.size());
  }

  // XXX: Refactor
  uint64_t max_size = 0;
  for (ComputeOperand *co : module.getComputeOperands()) {
    if (ComputeMemOperand *mem = dyn_cast<ComputeMemOperand>(co)) {
      Value *v = co->getValue();
      if (mem->isInput()) {
        // XXX: Multiple inputs
        interpreter.m_ATable[v] = input_mem;
      } else if (mem->isWeight()) {
        // XXX
        FloatTensor *t = static_cast<FloatTensor *>(v);
        interpreter.m_ATable[v] = t->getValues().data();
      } else {
        max_size = std::max(max_size, static_cast<uint64_t>(mem->start()) + mem->length());
      }
    }
  }

  // TODO: aligned_alloc after c++17
  // XXX: Refactor
  // TODO: posix_memalign(&interpreter.m_mem, backend->......, max_size)
  int fail = posix_memalign(reinterpret_cast<void **>(&heap), 16, max_size);
  assert((!fail) && "posix_memalign failed!");

  // Fixup memory address
  for (ComputeOperand *co : module.getComputeOperands()) {
    if (ComputeMemOperand *mem = dyn_cast<ComputeMemOperand>(co)) {
      if (mem->isOutput() || mem->isInternal()) {
        interpreter.m_ATable[co->getValue()] = heap + mem->start();
      }
    }
  }

  for (ComputeOperator &cm : *module.getRootComputeGraph()) {
    if (options().verbose() > 0) {
      cm.print(outs());
      outs() << std::endl;
    }
    cm.accept(interpreter);
  }

  // Hack for that: Due to the wrong ComputeOperand design,
  //                there is no output ComputeOperand.
  //                So that I have to use the OutputOperator's input tensor to
  //                get the real output.
  for (ComputeOperator &cm : *module.getRootComputeGraph()) {
    if (OutputOperator *out = dyn_cast<OutputOperator>(&cm)) {
      for (int i = 0; i < out->getNumOfInputs(); ++i) {
        Value *v = out->getInput(i);
        float *output = static_cast<float *>(interpreter.m_ATable[v]);

        Tensor *t = static_cast<Tensor *>(v);
        size_t size = 1;
        for (auto i: t->getDimensions()) {
          size *= i;
        }
        outs() << '[';
        for (size_t i = 0; i < size; ++i) {
          outs() << std::fixed << output[i] << ", ";
        }
        outs() << ']' << std::endl;
      }
    }
  }

  ONNC_RUNTIME_shutdown_runtime(runtime_context);

  // TODO: write output to file

  return EXIT_SUCCESS;
}
