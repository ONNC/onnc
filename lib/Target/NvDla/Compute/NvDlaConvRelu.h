//===- NvDlaConvRelu.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_CONV_RELU_H
#define TARGET_NVDLA_NVDLA_CONV_RELU_H

#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Relu.h>

namespace onnc {

class NvDlaConvRelu : public ComputeOperator
{
public:
  static char ID;

public:
  NvDlaConvRelu(Conv& pConv, Relu& pRelu)
    : ComputeOperator("ConvRelu", ID), m_Conv(pConv), m_Relu(pRelu) {
  }

  virtual ~NvDlaConvRelu() { }

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

  Conv m_Conv;
  Relu m_Relu;
};

} // namespace of onnc

#endif
