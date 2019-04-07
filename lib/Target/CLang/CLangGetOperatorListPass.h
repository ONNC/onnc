#ifndef TARGET_CLANG_GET_OPERATOR_LIST_PASS_H_INCLUDED
#define TARGET_CLANG_GET_OPERATOR_LIST_PASS_H_INCLUDED

#include "CLangMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {

class CLangGetOperatorListPass final : public CustomPass<CLangGetOperatorListPass>
{
public:
  CLangGetOperatorListPass() = default;
  explicit CLangGetOperatorListPass(CLangMeta& meta) noexcept
    : meta{meta}
  {}

  CLangGetOperatorListPass(const CLangGetOperatorListPass&) = default;
  CLangGetOperatorListPass(CLangGetOperatorListPass&&)      = default;
  CLangGetOperatorListPass& operator=(const CLangGetOperatorListPass&) = default;
  CLangGetOperatorListPass& operator=(CLangGetOperatorListPass&&) = default;
  ~CLangGetOperatorListPass()                                     = default;

  ReturnType runOnModule(Module& module) override;

private:
  CLangMeta& meta;
};

} // namespace onnc

#endif
