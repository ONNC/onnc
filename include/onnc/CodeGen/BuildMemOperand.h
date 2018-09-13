//===- BuildMemOperandPass.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_BUILD_MEM_OPERAND_H
#define ONNC_CODEGEN_BUILD_MEM_OPERAND_H
#include <onnc/Core/ModulePass.h>

namespace onnc {

/** \class BuildMemOperand
 *  \brief In onnc, ComputeMemOperand refers to an edge of an input-output-pair.
 *         This pass build memory operands for a given compute graph.
 *
 *         ComputeMemOperand has two usages:
 *         1. Allows later pass can use dfs/bfs traversing algorithms
 *            provided by ComputeGraph to traverse graph.
 *
 *         2. Memory allocation pass write allocation result into
 *            ComputeMemOperand, code emit pass or onnc-jit can use it to
 *            allocate memory on target.
 *
 *  \note  ComputeMemOperands which hold the same onnc::Value should have the
 *         same start and length, so even though AN onnc::Value maps to
 *         M x onnc::ComputeMemOperand, but these M x onnc::ComputeMemOperand
 *         should have the same memory allocation information.
 */
class BuildMemOperand : public ModulePass
{
public:
  static char ID;

public:
  BuildMemOperand()
    : ModulePass(ID) {
  }

  StringRef getPassName() const override { return "BuildMemOperand"; }

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  void createMemOperandsOfNode(ComputeGraph &pCG, ComputeOperator &pNode,
                               ComputeOperand::Residence pResd);

  void createMemOperandsOfGraph(ComputeGraph &pCG);
};

ModulePass* CreateBuildMemOperandPass();

} // namespace onnc

#endif