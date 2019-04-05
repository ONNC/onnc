#ifndef TARGET_CLANG_GEN_WEIGHT_FILE_PASS_H_INCLUDED
#define TARGET_CLANG_GEN_WEIGHT_FILE_PASS_H_INCLUDED

#include <onnc/Core/CustomPass.h>
#include <onnc/Support/Path.h>

#include "CLangMeta.h"

namespace onnc {

class CLangGenWeightFilePass final : public CustomPass<CLangGenWeightFilePass>
{
public:
  CLangGenWeightFilePass() = default;
  CLangGenWeightFilePass(const CLangMeta& meta, Path outputFile) noexcept
    : meta{meta}
    , outputFile{std::move(outputFile)}
  {}

  CLangGenWeightFilePass(const CLangGenWeightFilePass&) = default;
  CLangGenWeightFilePass(CLangGenWeightFilePass&&) = default;
  CLangGenWeightFilePass& operator=(const CLangGenWeightFilePass&) = default;
  CLangGenWeightFilePass& operator=(CLangGenWeightFilePass&&) = default;
  ~CLangGenWeightFilePass() = default;

  ReturnType runOnModule(Module& module) override;

private:
  const CLangMeta& meta;
  const Path       outputFile;
};

} // namespace onnc

#endif
