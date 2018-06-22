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
#include <onnx/checker.h>
#include <onnx/shape_inference/implementation.h>

using namespace onnc;

bool onnc::onnxInferShape(Module &pModule)
{
  // use onnx official shape inference implementation
  ::onnx::ModelProto modelProto;
  ::onnc::ExportModelProto(modelProto, pModule);
  try {
    ::onnx::checker::check_model(modelProto);
  } catch (::onnx::checker::ValidationError &e) {
    errs() << e.what() << "\n"
              << "ONNXShapeInference pass is not workable!!" << "\n";
    return false;
  }
  ::onnx::shape_inference::InferShapes(modelProto);
  ::onnc::IRBuilder ir_b(pModule);
  ir_b.update(modelProto);
  return true;
}
