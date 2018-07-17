//===- DeadNodeElimination.h ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DEAD_NODE_ELIMINATION_H
#define ONNC_DEAD_NODE_ELIMINATION_H
#include <onnc/Core/ModulePass.h>

namespace onnc {

/** \class DeadNodeElimination
 *  \brief Eliminating orphan nodes in ONNX. E.g undefined Node.
 */
class DeadNodeElimination : public ModulePass
{
public:
  static char ID;

public:
  DeadNodeElimination();

  virtual ~DeadNodeElimination() { }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  StringRef getPassName() const override { return "DeadNodeElimination"; }
};

ModulePass* CreateDeadNodeEliminationPass();

} // namespace of onnc

#endif

