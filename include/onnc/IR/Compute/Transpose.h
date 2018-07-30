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

  static char ID;

public:
  Transpose();

  // clang-format off
  Transpose(const IntsAttr& pPerm);

  // clang-format on

  // shallow copy constructor.
  Transpose(const Transpose &pCopy);

  ~Transpose() { }

  // clang-format off
  // Attributes getters
  const IntsAttr& getPerm() const { return m_Perm; }


  // Attributes setters
  void setPerm(const IntsAttr& pPerm) { m_Perm = pPerm; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getData() { return getInput(kData); }


  // Outputs getters
  Tensor* getTransposed() { return getOutput(kTransposed); }


  // Inputs setters
  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }


  // Outputs setters
  void setTransposed(Tensor& pTensor) { m_Outputs[kTransposed] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntsAttr m_Perm;
  // clang-format on
};

} // namespace of onnc

#endif
