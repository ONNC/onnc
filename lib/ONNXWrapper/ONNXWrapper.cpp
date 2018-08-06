//===- ONNXWrapper.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/ModulePass.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/ONNXWrapper/ONNXWrapper.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

bool onnc::onnxInferShape(Module &pModule)
{
  // use onnx official shape inference implementation
  xProto modelProto;
  onnc::ExportModelProto(modelProto, pModule);
  try {
    xcheck_model(modelProto);
  } catch (xValidationError &e) {
    errs() << e.what() << "\n"
              << "ONNXShapeInference pass is not workable!!" << "\n";
    return false;
  }
  xInferShapes(modelProto, xOpSchemaRegistry::Instance());
  ::onnc::IRBuilder ir_b(pModule);
  ir_b.update(modelProto);
  return true;
}
