//===- ONNXUtils.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef ONNXUTILS_H
#define ONNXUTILS_H

#include <onnc/IR/Module.h>
#include <onnx/common/ir_pb_converter.h>

namespace onnc {

typedef std::vector<::onnx::Dimension> TensorSizes;
typedef std::vector<int64_t> LongInts;

void SerializeToString(std::string &output, const Module &pModule);

void ExportModelProto(::onnx::ModelProto &pModelProto, const Module &pModule);

/// Factory of Module
/// @param [in] pModuleProto The prototex of the module.
Module* CreateModule(const ::onnx::ModelProto &pModelProto);

/// Factory of Module. destroy the module.
void DestroyModule(Module*& pModule);

size_t getTotalCount(const std::vector<int64_t> &pDim);

const ::onnx::Tensor &getTensor(std::string name, const ::onnx::Graph &graph);

bool OutputSizeIsInputSize(::onnx::Node& pNode);

void GetAttrVals(::onnx::Node& pNode, ::onnx::BuiltinSymbol pAttr,
                 LongInts& pVal);

void GetPads(::onnx::Node& pNode, LongInts& pPadsB, LongInts& pPadsE);

void GetConvKernelShape(::onnx::Node& pNode, LongInts& pKShape);

bool IsTranspose(const ::onnx::Node& pNode, const ::onnx::BuiltinSymbol pAttr);

LongInts GetValueSizes(const ::onnx::Value& pVal);

} // namespace onnc

#endif
