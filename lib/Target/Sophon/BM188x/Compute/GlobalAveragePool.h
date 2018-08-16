//===- GlobalAveragePool.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGGLOBAL_AVERAGE_POOL_H
#define ONNC_TARGET_BM1880_TGGLOBAL_AVERAGE_POOL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/GlobalAveragePool.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class GlobalAveragePool : public onnc::GlobalAveragePool
{
public:
  static char ID;

public:
  GlobalAveragePool();

  ~GlobalAveragePool();

  void init(const xNode &pNode);

  int getRShiftWidth() const { return m_RShiftWidth; }

  void setRShiftWidth(int pValue) { m_RShiftWidth = pValue; }

  void setThresholdXQuantized(int pV) { m_ThresholdXQuantized = pV; }

  void setEnableRelu(const int pEnableRelu) { m_EnableRelu = pEnableRelu; }

  int getEnableRelu() const { return m_EnableRelu; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  int m_EnableRelu;
  int m_RShiftWidth;
  int m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
