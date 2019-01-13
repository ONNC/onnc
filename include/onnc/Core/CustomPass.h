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

#include <type_traits>
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
    : ParentType(std::forward<Args>(args)...)
  { }

  virtual ~CustomPass() = default;

  typename ParentType::AnalysisID getPassID() const override {
    return id();
  }

  static typename ParentType::AnalysisID id() {
    static const char var{};
    return reinterpret_cast<typename ParentType::AnalysisID>(&var);
  }

  StringRef getPassName() const override {
    return typeid(PassType).name();
  }
};

} // namespace of onnc

#endif
