#include "CLangOperatorInvokeVisitor.h"

#include <onnc/ADT/StringRef.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute.h>

#include <iostream>
#include <string>

#define PP_GEN_CLASS_NAME() CLangOperatorInvokeVisitor
#define PP_GEN_VISIT_DEF(type, param) \
  PP_GEN_VISIT_RETURN_TYPE() PP_GEN_CLASS_NAME()::visit(PP_GEN_VISIT_PARAM_TYPE(type) param) { \
    auto&& target = internal::getTarget(PP_STRINGIFY(type)); \
    stream << indent_ << "// " << target << '\n'; \
    PP_ENTER_SCOPE(stream, indent_); \
    visitImpl(param, indent_ + 1, target); \
    PP_LEAVE_SCOPE(stream, indent_); \
  } \
  PP_GEN_VISIT_RETURN_TYPE() PP_GEN_CLASS_NAME()::visitImpl(PP_GEN_VISIT_PARAM_TYPE(type) param, \
                                                            internal::Indent indent, const std::string& target)

#define PP_ENTER_SCOPE(stream, indent) stream << indent << "{\n"
#define PP_LEAVE_SCOPE(stream, indent) stream << indent << "}\n"
#define PP_GET_OPERATOR_FUNCAME_NAME(type) "ONNC_RUNTIME_" type "_float"

namespace onnc {
namespace internal {
  std::string getTarget(const char* op)
  {
    std::ostringstream stream;
    stream << "ONNC_RUNTIME_" << StringRef{op}.lower() << "_float";
    return stream.str();
  }
} // namespace internal

using Identifier = std::string;
using Expression = std::string;

void CLangOperatorInvokeVisitor::visit(const Module& module)
{
  for (const ComputeOperator& computeOperator : *module.getRootComputeGraph()) {
    computeOperator.accept(*this);
  }
}

#include "internal/Conv.inc"
#include "internal/Gemm.inc"
#include "internal/LRN.inc"
#include "internal/MaxPool.inc"
#include "internal/Relu.inc"
#include "internal/Reshape.inc"
#include "internal/Softmax.inc"

} // namespace onnc
