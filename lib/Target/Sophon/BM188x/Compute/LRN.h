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
  static char ID;

public:
  LRN(const IntAttr& pSize);

  ~LRN();

  int getSumRightShftWidth() const { return m_SumRightShiftWidth; }

  int getLrnRightShiftWidth() const { return m_LrnRightShiftWidth; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  int m_SumRightShiftWidth;
  int m_LrnRightShiftWidth;
  int m_ThresholdXQuantized[2];
};

} // namespace BM188X
} // namespace onnc

#endif
