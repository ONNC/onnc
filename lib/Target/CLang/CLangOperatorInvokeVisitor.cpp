#include <onnc/IR/ComputeOperator.h>

#include "CLangOperatorInvokeVisitor.h"

#include <onnc/IR/Compute.h>

#include <iostream>
#include <string>

#define PP_GEN_CLASS_NAME() CLangOperatorInvokeVisitor
#define PP_GEN_VISIT_DEF(type, param) \
  PP_GEN_VISIT_RETURN_TYPE() PP_GEN_CLASS_NAME()::visit(PP_GEN_VISIT_PARAM_TYPE(type) param) { \
    PP_ENTER_SCOPE(stream, indent_); \
    visitImpl(param, indent_ + 1);   \
    PP_LEAVE_SCOPE(stream, indent_); \
  }                                  \
  PP_GEN_VISIT_RETURN_TYPE() PP_GEN_CLASS_NAME()::visitImpl(PP_GEN_VISIT_PARAM_TYPE(type) param, internal::Indent indent)

#define PP_ENTER_SCOPE(stream, indent) stream << indent << '{'
#define PP_LEAVE_SCOPE(stream, indent) stream << indent << '}'

namespace onnc {

using Identifier = std::string;
using Expression = std::string;

void CLangOperatorInvokeVisitor::visit(const Module& module)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  for (const ComputeOperator& computeOperator : *module.getRootComputeGraph()) {
    computeOperator.accept(*this);
  }
}

PP_GEN_VISIT_DEF(Conv, conv)
{
  std::cout << indent << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(Gemm, gemm)
{
  std::cout << indent << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(LRN, lrn)
{
  std::cout << indent << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(MaxPool, maxPool)
{
  std::cout << indent << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(Relu, relu)
{
  std::cout << indent << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(Reshape, reshape)
{
  std::cout << indent << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(Softmax, softmax)
{
  std::cout << indent << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace onnc
