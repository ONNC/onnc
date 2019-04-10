#include <onnc/IR/ComputeOperator.h>

#include "CLangOperatorInvokeVisitor.h"

#include <onnc/IR/Compute.h>

#include <iostream>

#ifndef PP_GEN_CLASS_NAME
# define PP_GEN_CLASS_NAME() CLangOperatorInvokeVisitor
#endif

#ifndef PP_GEN_VISIT_DEF
# define PP_GEN_VISIT_DEF(type, param) \
  PP_GEN_VISIT_RETURN_TYPE() PP_GEN_CLASS_NAME()::visit(PP_GEN_VISIT_PARAM_TYPE(type) param)
#endif

namespace onnc {

void CLangOperatorInvokeVisitor::visit(const Module& module)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  for (const ComputeOperator& computeOperator : *module.getRootComputeGraph()) {
    computeOperator.accept(*this);
  }
}

PP_GEN_VISIT_DEF(Conv, conv)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(Gemm, gemm)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(LRN, lrn)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(MaxPool, maxPool)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(Relu, relu)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(Reshape, reshape)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

PP_GEN_VISIT_DEF(Softmax, softmax)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace onnc
