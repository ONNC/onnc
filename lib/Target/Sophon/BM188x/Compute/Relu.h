//===- Relu.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_RELU_H
#define ONNC_TARGET_BM1880_RELU_H

#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Relu.h>

namespace onnc {
namespace BM188X {

class Relu : public onnc::Relu
{
public:
  static char ID;

public:
  Relu();

  const FloatAttr &getNegativeSlope() const { return m_NegativeSlope; }

  void setNegativeSlope(const FloatAttr &pNS) { m_NegativeSlope = pNS; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;
  
  static bool classof(const ComputeOperator* pOp);

private:
  FloatAttr m_NegativeSlope;
};

} // namespace BM188X
} // namespace onnc

#endif
