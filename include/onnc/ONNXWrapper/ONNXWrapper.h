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

namespace onnc {

class Module;

/// Infer (output) shape for onnx::Graph.
bool onnxInferShape(Module &pModule);

}

#endif
