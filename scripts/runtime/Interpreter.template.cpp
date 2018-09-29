//===- Interpreter.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Interpreter.h"
#include <onnc/Support/IOStream.h>

${ComputeIR_includes}

#define restrict __restrict__
extern "C" {
#include <onnc/Runtime/onnc-runtime.h>
}
#undef restrict

using namespace onnc;

//===----------------------------------------------------------------------===//
// Interpreter
//===----------------------------------------------------------------------===//
${interpreter_visitors}
