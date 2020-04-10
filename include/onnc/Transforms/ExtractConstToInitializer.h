//===- ExtractConstToInitializer.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_EXTRACT_CONST_TO_INITIALIZER_H
#define ONNC_EXTRACT_CONST_TO_INITIALIZER_H

#include <onnc/Core/CustomPass.h>

namespace onnc {
class ExtractConstToInitializer : public CustomPass<ExtractConstToInitializer>
{

public:
  ExtractConstToInitializer() = default;

  StringRef getPassName() const override { return "ExtractConstToInitializer"; }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;
};

ModulePass *CreateExtractConstToInitializerPass();
}

#endif // ONNC_EXTRACT_CONST_TO_INITIALIZER_H

