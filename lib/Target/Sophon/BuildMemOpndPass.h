//===- BuildMemOpndPass.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_BUILD_MEM_OPND_PASS_H
#define ONNC_TARGET_TG_BUILD_MEM_OPND_PASS_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <vector>

namespace onnc {

class BuildMemOpnd : public ModulePass
{
public:
  typedef std::vector<std::pair<ComputeMemOperand*, onnc::Value*>>
          MemOperandValList;
public:
  static char ID;

public:
  BuildMemOpnd();

  StringRef getPassName() const override { return "BuildMemOpnd"; }
  
  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  const MemOperandValList &getMemOperandList() const
  {
    return m_ValOperandMap;
  }

private:
  void createMemOperandsOfNode(ComputeGraph &pCG, ComputeOperator &pNode,
                               ComputeOperand::Residence pResd);

  void createAllMemOperands(onnc::ComputeGraph &pCG);

  void clear();

private:
  MemOperandValList m_ValOperandMap;
};

ModulePass *CreateBuildMemOpndPass();

} // namespace onnc

#endif