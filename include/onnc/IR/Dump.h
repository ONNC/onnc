//===- Dump.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements dump utilities to dump onnx objects.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ONNX_DUMP_H
#define ONNC_ONNX_DUMP_H
#include <onnc/Support/IOStream.h>
#include <onnx/common/ir.h>

namespace onnc {

class Module;

/// DumpGraph. Callable in GDB.
void DumpGraph(onnx::Graph& pGraph);
void DumpGraph(Module &pModule);

void PrintNode(OStream &pOS, onnx::Node& pNode);
void PrintGraph(OStream &pOS, onnx::Graph& pGraph);

}

#endif
