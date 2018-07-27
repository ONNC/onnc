//===- LRN.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGLRN_H
#define ONNC_TARGET_BM1880_TGLRN_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/LRN.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class LRN : public onnc::LRN
{
public:
  LRN(const IntAttr& pSize);

  ~LRN();

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override { pV.visit(*this); }

  void accept(ComputeVisitor &pV) const override { pV.visit(*this); }

private:
  int m_SumRightShiftWidth;
  int m_LrnRightShiftWidth;
  int m_ThresholdXQuantized[2];
};

} // namespace BM188X
} // namespace onnc

#endif