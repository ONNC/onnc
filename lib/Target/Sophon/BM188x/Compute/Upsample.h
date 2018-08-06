//===- Upsample.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGUPSAMPLE_H
#define ONNC_TARGET_BM1880_TGUPSAMPLE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Upsample.h>
#include <vector>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class Upsample : public onnc::Upsample
{
public:
  static char ID;

public:
  Upsample(int pScale, const FloatAttr& pHeightScale,
           const FloatAttr& pWidthScale);

  ~Upsample();

  int getScale() const { return m_Scale; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  int m_Scale;
};

} // namespace BM188X
} // namespace onnc

#endif
