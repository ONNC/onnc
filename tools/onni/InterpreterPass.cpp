//===- InterpreterPass.cpp ------------------------------------------------===//
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
#include <onnc/Support/Timer.h>

#include <cassert>
#include <algorithm>

#define restrict __restrict__
extern "C" {
#include <onnc/Runtime/onnc-runtime.h>
}
#undef restrict

// TODO: ====== REMOVE THIS AFTER REWRITE Support/Timer.h ======
#include <time.h>
#if defined(HAVE_SYS_TIMES_H)
#include <sys/times.h>
#endif
#if defined(HAVE_SYS_TIME_H) && defined(ENABLE_GETTIMEOFDAY)
#include <sys/time.h>
#endif
namespace {
  onnc::Timer::Interval ns() {
#if defined(HAVE_CLOCK_GETTIME) && defined(ENABLE_CLOCK_GETTIME)
    struct timespec ts;
    int r = clock_gettime(CLOCK_MONOTONIC, &ts);
    return r == -1 ? -1 : ts.tv_sec * 1000000000LL + ts.tv_nsec;
#elif defined(HAVE_GETTIMEOFDAY) && defined(ENABLE_GETTIMEOFDAY)
    struct timeval tv;
    int r = gettimeofday(&tv, NULL);
    return r == -1 ? -1 : tv.tv_sec * 1000000000LL + (tv.tv_usec * 1000LL);
#else
    struct tms tm;
    clock_t r = times(&tm);
    return r == -1 ? -1 : r * 1000000000LL / g_ClkTick;
#endif
  }
}
// ========= REMOVE THIS AFTER REWRITE Support/Timer.h =========

using namespace onnc;

//===----------------------------------------------------------------------===//
// InterpreterPass
//===----------------------------------------------------------------------===//
InterpreterPass::InterpreterPass(TargetBackend *pBackend,
                                 char *pInputMem,
                                 unsigned int pVerbose,
                                 bool pIsDryRun)
  : ModulePass(ID),
    m_pBackend(pBackend), m_pInputMem(pInputMem),
    m_Verbose(pVerbose), m_DryRun(pIsDryRun) {
}

Pass::ReturnType InterpreterPass::runOnModule(Module &pModule)
{
  // XXX: Use Pass or something to get internal memory size
  uint64_t weight_memory_size = 0;
  uint64_t internal_memory_size = 0;
  for (ComputeOperand *co : pModule.getComputeOperands()) {
    if (ComputeMemOperand *mem = dyn_cast<ComputeMemOperand>(co)) {
      Value *v = co->getValue();
      if (mem->isInput()) {
        // XXX: Multiple inputs
        m_Interpreter.m_ATable[v] = m_pInputMem;
      } else if (mem->isWeight()) {
        // XXX
        FloatTensor *t = static_cast<FloatTensor *>(v);
        m_Interpreter.m_ATable[v] = t->getValues().data();
        weight_memory_size +=
            t->getValues().size() * sizeof(FloatTensor::ValueList::value_type);
      } else {
        internal_memory_size =
            std::max(internal_memory_size,
                     static_cast<uint64_t>(mem->start()) + mem->length());
      }
    }
  }
  if (m_Verbose >= 1) {
    outs() << "[v1] weight memory: " << weight_memory_size << std::endl;
    outs() << "[v1] internal memory: " << internal_memory_size << std::endl;
  }

  if (!m_DryRun) {
    // XXX: Use onnc-runtime to handle memory
    char *heap = NULL;

    // TODO: aligned_alloc after c++17
    // XXX: Refactor into interpreter
    // TODO: posix_memalign(&interpreter.m_mem,
    //                      backend->......,
    //                      internal_memory_size)
    int fail = posix_memalign(reinterpret_cast<void **>(&heap),
                              16,
                              internal_memory_size);
    assert((!fail) && "posix_memalign failed!");

    // Fixup memory address
    for (ComputeOperand *co : pModule.getComputeOperands()) {
      if (ComputeMemOperand *mem = dyn_cast<ComputeMemOperand>(co)) {
        if (mem->isOutput() || mem->isInternal()) {
          m_Interpreter.m_ATable[co->getValue()] = heap + mem->start();
        }
      }
    }

    Pass::ReturnType r = runInterpreter(pModule);

    // TODO: (use runtime) write output to file
    free(heap);

    return r;
  } else {
    return Pass::kModuleNoChanged;
  }
}

Pass::ReturnType InterpreterPass::runInterpreter(Module &pModule)
{
  // TODO: Refactor into Interpreter
  m_Interpreter.m_pContext = ONNC_RUNTIME_init_runtime();

  Timer::Interval total;
  // TODO: Timer can not nested. Should rewrite it.
  if (m_Verbose >= 1) total = ::ns();
  for (ComputeOperator &cm : *pModule.getRootComputeGraph()) {
    if (m_Verbose >= 2) {
      outs() << "[v2] ";
      cm.print(outs());
      outs() << std::endl;
    }

    Timer timer;

    if (m_Verbose >= 3) timer.start();
    cm.accept(m_Interpreter);
    if (m_Verbose >= 3) {
      timer.stop();
      outs() << "[v3] " << cm.name()
             << " runs in " << timer.interval() << ' ' << timer.unit()
             << std::endl;
    }
  }
  if (m_Verbose >= 1) {
    total = ns() - total;
    outs() << "[v1] total inference time: " << total << " ns" << std::endl;
  }


  // Hack for that: Due to the wrong ComputeOperand design,
  //                there is no output ComputeOperand.
  //                So that I have to use the OutputOperator's input tensor to
  //                get the real output.
  for (ComputeOperator &cm : *pModule.getRootComputeGraph()) {
    if (OutputOperator *out = dyn_cast<OutputOperator>(&cm)) {
      for (int i = 0; i < out->getNumOfInputs(); ++i) {
        Value *v = out->getInput(i);
        float *output = static_cast<float *>(m_Interpreter.m_ATable[v]);

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

  ONNC_RUNTIME_shutdown_runtime(m_Interpreter.m_pContext);

  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char InterpreterPass::ID = 0;

InterpreterPass *onnc::CreateInterpreterPass(TargetBackend *pBackend,
                                             char *pInputMem,
                                             unsigned int pVerbose,
                                             bool pIsDryRun) {
  return new InterpreterPass(pBackend, pInputMem, pVerbose, pIsDryRun);
}
