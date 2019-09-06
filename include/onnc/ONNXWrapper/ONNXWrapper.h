//===- ONNXWrapper.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// ONNC library use -fno-exceptions, but some of onnx API throw exceptions,
// so ONNC create a wrapper for those onnx API.
//
// [FIXME] Get rid of this one day.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ONNX_WRAPPER_H
#define ONNC_ONNX_WRAPPER_H
#include <memory>

#include <onnc/Config/ONNX.h>

namespace onnc {

class Module;

/// Import ModelProto into Module
std::unique_ptr<xGraph> onnxImportModelProto(const xProto &pModelProto);

/// Infer (output) shape for xGraph.
bool onnxInferShape(Module &pModule);

}

#endif
