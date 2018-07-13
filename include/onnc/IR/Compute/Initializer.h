//===- Initializer.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_INITIALIZER_H
#define ONNC_IR_COMPUTE_OPERATOR_INITIALIZER_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

/** \class Initializer
 *  \brief Initializer is a kind of ComputeOperator represents initializers
 *  in ONNX.
 *
 *  Initializer stores name and the data copied from ::onnx::Tensor.
 *  The data is stored in Initializer's output.
 */
class Initializer : public ComputeOperator
{
public:
  Initializer();

  Initializer(const StringAttr& pName);

  ~Initializer();

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

private:
  StringAttr m_Name;
};

} // namespace of onnc

#endif
