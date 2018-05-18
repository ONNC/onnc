//===- ONNXUtils.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <onnc/IR/Module.h>
#include <onnx/common/ir_pb_converter.h>

namespace onnc {
namespace onnx {

void ExportModelProto(::onnx::ModelProto &pModelProto, const Module &pModule);

void ImportModelProto(Module &pModule, const ::onnx::ModelProto &pModelProto);

} // namespace onnx
} // namespace onnc
