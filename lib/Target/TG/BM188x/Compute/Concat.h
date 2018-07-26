//===- Concat.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGCONCAT_H
#define ONNC_TARGET_BM1880_TGCONCAT_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Concat.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class Concat : public onnc::Concat
{
public:
  Concat(const IntAttr& pAxis);

  ~Concat();

  void setNeedQuantizeNum(int pN) { m_NeedQuantizeNum = pN; }

  int getNeedQuantizeNum() const { return m_NeedQuantizeNum; }

  void setRShiftWidth(unsigned int pIdx, int pValue);

  const int* getRShiftWidth() const;

  void setThresholdXQuantized(unsigned int pIdx, int pValue);

  const int* getThresholdXQuantized() const;

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override;

  void accept(ComputeVisitor& pVisitor) const override;

private:
  int m_NeedQuantizeNum;
  std::vector<int> m_RShiftWidth;
  std::vector<int> m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
