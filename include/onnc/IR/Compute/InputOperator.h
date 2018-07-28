//===- InputOperator.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_INPUT_OPERATOR_H
#define ONNC_IR_COMPUTE_OPERATOR_INPUT_OPERATOR_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

/** \class InputOperator
 *  \brief InputOperator is a kind of ComputeOperator represents inputs
 *  in ONNX.
 */
class InputOperator : public ComputeOperator
{
public:
  enum IOConst {
    kOutput = 0
  };

  static char ID;

public:
  InputOperator();

  InputOperator(const StringAttr& pName);

  ~InputOperator();

  void setNameAttr(const StringAttr& pName) { m_Name = pName; }

  const StringAttr& getNameAttr() const { return m_Name; }

  template<typename TensorType>
  TensorType* getOutput() { return static_cast<TensorType*>(m_Outputs[0]); }

  template<typename TensorType>
  const TensorType* getOutput() const { return static_cast<TensorType*>(m_Outputs[0]); }

  template<typename TensorType>
  TensorType* getTensor() { return getOutput<TensorType>(); }

  template<typename TensorType>
  const TensorType* getTensor() const { return getOutput<TensorType>(); }

  void setTensor(Tensor& pTensor);

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  StringAttr m_Name;
};

} // namespace of onnc

#endif
