//===- ONNXWrapper.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/ModulePass.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/ONNXWrapper/ONNXWrapper.h>
#include <onnc/Support/Contract.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/String.h>

using namespace onnc;

std::unique_ptr<xGraph> onnc::onnxImportModelProto(const xProto &pModelProto)
{
  // use onnx official shape inference implementation
  try {
    // Catch all onnx exception
    return xImportModelProto(pModelProto);
  } catch (std::exception &e) {
    // Can not allow any error throw
    fatal(onnx_cannot_import) << e.what();
  }
}

bool onnc::onnxInferShape(Module &pModule)
{
  // use onnx official shape inference implementation
  try {
    // Catch all onnx exception
    xProto modelProto;
    onnc::ExportModelProto(modelProto, pModule);
    xcheck_model(modelProto);
    xInferShapes(modelProto, xOpSchemaRegistry::Instance());
    ::onnc::IRBuilder ir_b(pModule);
    ir_b.update(modelProto);
  } catch (std::exception &e) {
    // Can not allow any error throw
    fatal(onnx_shape_inference_fail) << e.what();
  }
  return true;
}
