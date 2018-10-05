//===- X86ConvRelu.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_X86_X86_CONV_RELU_H
#define TARGET_X86_X86_CONV_RELU_H

#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>

namespace onnc {

class X86ConvRelu : public ComputeOperator
{
public:
  static char ID;

public:
  X86ConvRelu()
    : ComputeOperator("X86ConvRelu", ID) {
  }

  virtual ~X86ConvRelu() { }

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

  static bool classof(const ComputeOperator* pOp);
};

} // namespace of onnc

#endif
