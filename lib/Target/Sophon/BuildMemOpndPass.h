//===- BuildMemOpndPass.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_BUILD_MEM_OPND_PASS_H
#define ONNC_TARGET_TG_BUILD_MEM_OPND_PASS_H
#include <onnc/Core/CustomPass.h>
#include <onnc/Core/PassSupport.h>
#include <vector>

namespace onnc {

class BuildMemOpnd : public CustomPass<BuildMemOpnd>
{
public:
  typedef std::map<const onnc::Value *, ComputeMemOperand *> ValMemOpndMap;

public:
  BuildMemOpnd() = default;

  StringRef getPassName() const override { return "BuildMemOpnd"; }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  const ValMemOpndMap &getMemOperandList() const
  {
    return m_ValMemOpndMap;
  }

private:
  void createMemOperandsOfNode(ComputeGraph &pCG, ComputeOperator &pNode,
                               ComputeOperand::Residence pResd);

  void createAllMemOperands(onnc::ComputeGraph &pCG);

  void clear();

private:
  ValMemOpndMap m_ValMemOpndMap;
};

ModulePass *CreateBuildMemOpndPass();

} // namespace onnc

#endif
