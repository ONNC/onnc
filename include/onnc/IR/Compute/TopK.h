//===- TopK.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_TOPK_H
#define ONNC_IR_COMPUTE_OPERATOR_TOPK_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class TopK : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kValues = 0,
    kIndices = 1
  };

  static char ID;

public:
  TopK(const IntAttr& pK);

  // clang-format off
  TopK(const IntAttr& pAxis,
       const IntAttr& pK);

  // clang-format on

  // shallow copy constructor.
  TopK(const TopK &pCopy);

  ~TopK() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getAxis() const { return m_Axis; }

  const IntAttr& getK() const { return m_K; }


  // Attributes setters
  void setAxis(const IntAttr& pAxis) { m_Axis = pAxis; }

  void setK(const IntAttr& pK) { m_K = pK; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getX() { return getInput(kX); }


  // Outputs getters
  Tensor* getValues() { return getOutput(kValues); }

  Tensor* getIndices() { return getOutput(kIndices); }


  // Inputs setters
  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }


  // Outputs setters
  void setValues(Tensor& pTensor) { m_Outputs[kValues] = &pTensor; }

  void setIndices(Tensor& pTensor) { m_Outputs[kIndices] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntAttr m_Axis;
  IntAttr m_K;
  // clang-format on
};

} // namespace of onnc

#endif
