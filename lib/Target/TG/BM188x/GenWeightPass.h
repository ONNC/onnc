//===- GenWeightPass.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_GEN_WEIGHT_PASS_H
#define ONNC_TARGET_TG_GEN_WEIGHT_PASS_H
#include "../TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Support/Path.h>
#include <onnc/Support/DataTypes.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <vector>
#include <unordered_set>

namespace onnc {
namespace BM188X {

class GenWeightPass : public ModulePass
{
public:
  static char ID;

  typedef int8_t DataType;
  typedef std::vector<DataType> WeightType;

public:
  GenWeightPass(const Path &pOutFile);

  Pass::ReturnType runOnModule(Module &pModule) override;

  const WeightType& weights() const { return m_Weight; }

  /// fill weight field by Module.
  void fillWeight(const Module &pModule);

private:
  /// remember the written TLConv's memory operands to prevent from
  /// duplicatedly written.
  typedef std::unordered_set<const ComputeMemOperand*> DoneOpndSet;

private:
  bool isWritten(const ComputeMemOperand* pOpnd) const;

  void setWritten(const ComputeMemOperand* pOpnd);

private:
  Path m_OutFile;
  WeightType m_Weight;
  DoneOpndSet m_DoneOpndSet;
};

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass* CreateGenWeightPass(const Path& pOutFile);

} // namespace BM188X
} // namespace onnc

#endif
