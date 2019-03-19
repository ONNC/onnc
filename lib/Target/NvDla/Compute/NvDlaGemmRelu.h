//===- NvDlaGemmRelu.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_GEMM_RELU_H
#define TARGET_NVDLA_NVDLA_GEMM_RELU_H

#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/ComputeOperator.h>

namespace onnc {

class NvDlaGemmRelu : public ComputeOperator
{
public:
  static char ID;

public:
  NvDlaGemmRelu(Gemm& pGemm, Relu& pRelu)
    : ComputeOperator("GemmRelu", ID)
    , m_Gemm(pGemm)
    , m_Relu(pRelu)
  {}

  virtual ~NvDlaGemmRelu() {}

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

  Gemm m_Gemm;
  Relu m_Relu;
};

} // namespace onnc

#endif
