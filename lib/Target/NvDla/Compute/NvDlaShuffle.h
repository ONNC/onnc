//===- NvDlaShuffle.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===--------------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_SHUFFLE_H
#define TARGET_NVDLA_NVDLA_SHUFFLE_H

#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Transpose.h>
#include <onnc/IR/ComputeOperator.h>

namespace onnc {

class NvDlaShuffle : public ComputeOperator
{
public:
  static char ID;

public:
  NvDlaShuffle(int axis, int group)
    : ComputeOperator("NvDlaShuffle", ID)
  {
    m_Axis.setValue(1); // channel

    m_Group.setValue(group);
  }

  virtual ~NvDlaShuffle() {}

  // Paramater
  const IntAttr& getAxis() const { return m_Axis; }

  const IntAttr& getGroup() const { return m_Group; }

  // Input & Ouput Tensor
  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  IntAttr m_Axis;
  IntAttr m_Group;
};

} // namespace onnc

#endif
