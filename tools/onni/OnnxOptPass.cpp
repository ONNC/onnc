//===- OnnxOptPass.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "OnnxOptPass.h"

#include <onnc/Config/ONNX.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/ONNXWrapper/ONNXWrapper.h>

#include <onnx/optimizer/optimize.h>

#include <memory>



using namespace onnc;

//===----------------------------------------------------------------------===//
// OnnxOptPass
//===----------------------------------------------------------------------===//
Pass::ReturnType OnnxOptPass::runOnModule(Module &pModule)
{
  onnxInferShape(pModule);

  xProto mp;
  onnc::ExportModelProto(mp, pModule);
  mp = onnx::optimization::Optimize(mp, {
    "extract_constant_to_initializer",
    "fuse_add_bias_into_conv",
    "fuse_bn_into_conv",
    "fuse_consecutive_squeezes",
    "fuse_consecutive_transposes",
    "fuse_transpose_into_gemm",
    "eliminate_identity",
    "eliminate_nop_pad",
    "eliminate_nop_transpose",
    "eliminate_unused_initializer"
  });
  pModule.delegate(xImportModelProto(mp));

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char OnnxOptPass::ID = 0;

OnnxOptPass *onnc::CreateOnnxOptPass() {
  return new OnnxOptPass();
}
