//===- NvDlaMaxPool.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_MAXPOOL_H
#define TARGET_NVDLA_NVDLA_MAXPOOL_H

#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/ComputeOperator.h>

namespace onnc {

class NvDlaMaxPool : public ComputeOperator
{
public:
  static char ID;

public:
  NvDlaMaxPool(MaxPool& pMaxPool)
    : ComputeOperator("ConvRelu", ID)
    , m_MaxPool(pMaxPool)
  {}

  virtual ~NvDlaMaxPool() {}

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);

  MaxPool m_MaxPool;
  IntAttr m_Group;
};

} // namespace onnc

#endif
