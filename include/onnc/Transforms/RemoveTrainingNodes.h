//===- RemoveTrainingNodes.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_REMOVE_TRAINING_NODES_H
#define ONNC_REMOVE_TRAINING_NODES_H

#include <onnc/Core/ModulePass.h>

namespace onnc {
class RemoveTrainingNodes : public ModulePass
{
public:
  static char ID;

public:
  RemoveTrainingNodes();

  StringRef getPassName() const override { return "RemoveTrainingNodes"; }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;
};

ModulePass *CreateRemoveTrainingNodesPass();
}

#endif // ONNC_REMOVE_TRAINING_NODES_H
