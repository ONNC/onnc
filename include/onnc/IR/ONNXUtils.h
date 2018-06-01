//===- ONNXUtils.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef ONNXUTILS_H
#define ONNXUTILS_H

#define ONNX_NAMESPACE onnx

#include <onnc/IR/Module.h>
#include <onnx/common/ir_pb_converter.h>

namespace onnc {

void SerializeToString(std::string &output, const Module &pModule);

void ExportModelProto(::onnx::ModelProto &pModelProto, const Module &pModule);

/// Factory of Module
/// @param [in] pModuleProto The prototex of the module.
Module* CreateModule(const ::onnx::ModelProto &pModelProto);

/// Factory of Module. destroy the module.
void DestroyModule(Module*& pModule);

size_t getTotalCount(const std::vector<int64_t> &pDim);

const ::onnx::Tensor &getTensor(std::string name, const ::onnx::Graph &graph);

} // namespace onnc

#endif
