//===- CustomPass.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_CUSTOM_PASS_H
#define ONNC_CORE_CUSTOM_PASS_H

#include <onnc/Core/ModulePass.h>
#include <onnc/IR/ComputeGraph.h>

#include <type_traits>
#include <typeinfo>
#include <utility>

namespace onnc {

/** \class onnc::CustomPass
 *  \brief default module class which implements most of the pure virtual methods
 */
template <
  typename PassType,
  typename ParentType = ModulePass,
  typename = typename std::enable_if<
    std::is_class<PassType>::value
    && std::is_base_of<ModulePass, ParentType>::value
  >::type
>
class CustomPass : public ParentType
{
public:
  using BaseType = CustomPass;

  template <typename... Args>
  CustomPass(Args&&... args)
    noexcept(std::is_nothrow_constructible<ParentType, Args...>::value)
    : ParentType(std::forward<Args>(args)...)
  { }

  virtual ~CustomPass() = default;

  typename ParentType::AnalysisID getPassID() const override {
    return id();
  }

  static typename ParentType::AnalysisID id() noexcept {
    static const char var{};
    return reinterpret_cast<typename ParentType::AnalysisID>(&var);
  }

  StringRef getPassName() const override {
    return typeid(PassType).name();
  }

  typename ParentType::ReturnType runOnModule(Module& module) override {
    if (!std::is_same<ModulePass, ParentType>::value) {
      return ParentType::runOnModule(module);
    }

    typename ParentType::ReturnType result = Pass::kModuleNoChanged;

    for (Module::cg_iterator next = module.cgBegin(); next != module.cgEnd(); ++next) {
      if ((result & ParentType::kPassFailure) != 0 || (result & ParentType::kPassRetry) != 0) {
        break;
      }

      result |= runOnComputeGraph(*next->value());
    }

    return result;
  }

  virtual typename ParentType::ReturnType runOnComputeGraph(ComputeGraph& graph) {
    return ParentType::kModuleNoChanged;
  }
};

} // namespace of onnc

#endif
