//===- ComputeVisitorPass.h -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_VISITOR_PASS_H_INCLUDED
#define ONNC_IR_COMPUTE_VISITOR_PASS_H_INCLUDED

#include <onnc/Core/CustomPass.h>
#include <onnc/IR/ComputeVisitor.h>

#include <type_traits>
#include <utility>

namespace onnc {

template <typename VisitorT>
class ComputeVisitorPass : public CustomPass<ComputeVisitorPass<VisitorT>>
{
public:
  using VisitorType = VisitorT;

public:
  explicit ComputeVisitorPass(VisitorType& visitor)
    : visitor(visitor)
  {}

  Pass::ReturnType runOnComputeGraph(ComputeGraph& graph) override
  {
    for (ComputeOperator& computeOperator : graph) {
      computeOperator.accept(visitor);
    }

    return Pass::kModuleNoChanged;
  }

private:
  VisitorType& visitor;
};

template <typename VisitorT>
class ComputeVisitorPassAdaptor : public CustomPass<ComputeVisitorPassAdaptor<VisitorT>>
{
public:
  using VisitorType = VisitorT;

public:
  template <typename... Args>
  ComputeVisitorPassAdaptor(Args&&... args) noexcept(std::is_nothrow_constructible<VisitorType, Args...>::value)
    : visitor(std::forward<Args>(args)...)
    , pass(visitor)
  {}

  Pass::ReturnType runOnComputeGraph(ComputeGraph& graph) override { return pass.runOnComputeGraph(graph); }

private:
  VisitorType                     visitor;
  ComputeVisitorPass<VisitorType> pass;
};

} // namespace onnc

#endif
