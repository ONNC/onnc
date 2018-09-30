//===- InterpreterPass.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "InterpreterPass.h"

#include "Interpreter.h"

#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>

#include <cassert>
#include <algorithm>

#define restrict __restrict__
extern "C" {
#include <onnc/Runtime/onnc-runtime.h>
}
#undef restrict

using namespace onnc;

//===----------------------------------------------------------------------===//
// InterpreterPass
//===----------------------------------------------------------------------===//
InterpreterPass::InterpreterPass(TargetBackend *pBackend,
                                 char *pInputMem,
                                 unsigned int pVerbose)
  : ModulePass(ID),
    m_pBackend(pBackend), m_pInputMem(pInputMem), m_Verbose(pVerbose) {
}

Pass::ReturnType InterpreterPass::runOnModule(Module &pModule)
{
  // TODO: Refactor
  void *runtime_context = ONNC_RUNTIME_init_runtime();

  Interpreter interpreter(runtime_context);

  // XXX: Use onnc-runtime to handle memory
  char *heap = NULL;

  // XXX: Use Pass or something to get internal memory size
  uint64_t max_size = 0;
  for (ComputeOperand *co : pModule.getComputeOperands()) {
    if (ComputeMemOperand *mem = dyn_cast<ComputeMemOperand>(co)) {
      Value *v = co->getValue();
      if (mem->isInput()) {
        // XXX: Multiple inputs
        interpreter.m_ATable[v] = m_pInputMem;
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
  for (ComputeOperand *co : pModule.getComputeOperands()) {
    if (ComputeMemOperand *mem = dyn_cast<ComputeMemOperand>(co)) {
      if (mem->isOutput() || mem->isInternal()) {
        interpreter.m_ATable[co->getValue()] = heap + mem->start();
      }
    }
  }

  for (ComputeOperator &cm : *pModule.getRootComputeGraph()) {
    if (m_Verbose > 0) {
      cm.print(outs());
      outs() << std::endl;
    }

    cm.accept(interpreter);
  }

  // Hack for that: Due to the wrong ComputeOperand design,
  //                there is no output ComputeOperand.
  //                So that I have to use the OutputOperator's input tensor to
  //                get the real output.
  for (ComputeOperator &cm : *pModule.getRootComputeGraph()) {
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
  free(heap);

  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char InterpreterPass::ID = 0;

InterpreterPass *onnc::CreateInterpreterPass(TargetBackend *pBackend,
                                             char *pInputMem,
                                             unsigned int pVerbose) {
  return new InterpreterPass(pBackend, pInputMem, pVerbose);
}
