//===- EliminateCast.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ELIMINATE_CAST_H
#define ONNC_ELIMINATE_CAST_H

#include <onnc/Core/CustomPass.h>

namespace onnc {
class EliminateCast : public CustomPass<EliminateCast>
{

public:
  EliminateCast() = default;

  StringRef getPassName() const override { return "EliminateCast"; }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;
};

ModulePass *CreateEliminateCastPass();
}

#endif // ONNC_ELIMINATE_CAST_H

