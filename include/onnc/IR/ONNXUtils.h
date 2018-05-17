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

void SerializeToString(std::string &output, const Module &pModule);
void ExportModelProto(::onnx::ModelProto &pModelProto, const Module &pModule);

void ImportModelProto(Module &pModule, const ::onnx::ModelProto &pModelProto);

size_t getTotalCount(const std::vector<int64_t> &pDim);
const ::onnx::Tensor &getTensor(std::string name, const ::onnx::Graph &graph);

} // namespace onnx
} // namespace onnc
