//===- OutputOperator.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_OUTPUT_OPERATOR_H
#define ONNC_IR_COMPUTE_OPERATOR_OUTPUT_OPERATOR_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

/** \class OutputOperator
 *  \brief OutputOperator is a kind of ComputeOperator represents inputs
 *  in ONNX.
 */
class OutputOperator : public ComputeOperator
{
public:
  static char ID;

public:
  OutputOperator();

  OutputOperator(const StringAttr& pName);

  ~OutputOperator();

  void setNameAttr(const StringAttr& pName) { m_Name = pName; }

  const StringAttr& getNameAttr() const { return m_Name; }

  template<typename TensorType>
  TensorType* getValue(unsigned int pIdx) {
    return static_cast<TensorType*>(m_Inputs[pIdx]);
  }

  template<typename TensorType>
  const TensorType* getValue(unsigned int pIdx) const {
    return static_cast<TensorType*>(m_Inputs[pIdx]);
  }

  template<typename TensorType>
  TensorType* getTensor(unsigned int pIdx) {
    return getValue<TensorType>(pIdx);
  }

  template<typename TensorType>
  const TensorType* getTensor(unsigned int pIdx) const {
    return getValue<TensorType>(pIdx);
  }

  void addTensor(Tensor& pTensor);

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  StringAttr m_Name;
};

} // namespace of onnc

#endif
