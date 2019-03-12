//===- NvDlaConvReluMaxPool.h ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_CONV_RELU_MAXPOOL_H
#define TARGET_NVDLA_NVDLA_CONV_RELU_MAXPOOL_H

#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/ComputeOperator.h>

namespace onnc {

class NvDlaConvReluMaxPool : public ComputeOperator
{
public:
  static char ID;

public:
  NvDlaConvReluMaxPool(Conv& pConv, Relu& pRelu, MaxPool& pMaxPool)
    : ComputeOperator("ConvReluMaxPool", ID)
    , m_Conv(pConv)
    , m_Relu(pRelu)
    , m_MaxPool(pMaxPool)
  {}

  virtual ~NvDlaConvReluMaxPool() {}

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

  Conv    m_Conv;
  Relu    m_Relu;
  MaxPool m_MaxPool;
};

} // namespace onnc

#endif
