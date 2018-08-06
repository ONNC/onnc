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
#include <onnc/Config/ONNX.h>

namespace onnc {

class Module;

/// DumpGraph. Callable in GDB.
void DumpGraph(xGraph &pGraph);
void DumpGraph(Module &pModule);
void DumpModule(Module &pModule);

void PrintNode(OStream &pOS, xNode &pNode);
void PrintGraph(OStream &pOS, xGraph &pGraph);
}

#endif
