//===- GlobalMemAllocPass.h -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_GLOBAL_MEMORY_ALLOCATION_PASS_H
#define ONNC_TARGET_TG_GLOBAL_MEMORY_ALLOCATION_PASS_H
#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <vector>
#include <unordered_set>
#include <utility>

class GlobalMemAllocVisitor;

namespace onnc {
namespace BM188X {

class GlobalMemAlloc : public ModulePass
{
  friend class ::GlobalMemAllocVisitor;
public:
  typedef std::vector<std::pair<ComputeMemOperand*, onnc::Value*>>
          MemOperandValList;

public:
  static char ID;

public:
  GlobalMemAlloc(TGBackend *pTarget);

  StringRef getPassName() const override { return "GlobalMemAlloc"; }
  
  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

private:
  void createMemOperandsOfNode(ComputeGraph &pCG, ComputeOperator &pNode,
                               ComputeOperand::Residence pResd);

  void createAllMemOperands(onnc::ComputeGraph &pCG);

  void allocGlobalMem();

  void linearScanAlloMem();

  void clear();

private:
  TGBackend *m_pTarget; // NOLINT
  MemOperandValList m_ValOperandMap;
};

} // namespace BM188X

ModulePass *CreateGlobalMemAllocPass(TGBackend *pTarget);

} // namespace onnc

#endif
