//===- Transpose.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_TRANSPOSE_H
#define ONNC_IR_COMPUTE_OPERATOR_TRANSPOSE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Transpose : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kTransposed = 0
  };

public:
  Transpose();

  Transpose(const IntsAttr& pPerm);

  ~Transpose() { }

  const IntsAttr& getPerm() const { return m_Perm; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData() { return getInput(kData); }

  Tensor* getTransposed() { return getOutput(kTransposed); }

  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }

  void setTransposed(Tensor& pTensor) { m_Outputs[kTransposed] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

private:
  IntsAttr m_Perm;
};

} // namespace of onnc

#endif
