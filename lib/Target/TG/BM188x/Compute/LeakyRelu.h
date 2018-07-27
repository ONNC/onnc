//===- LeakyRelu.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGLEAKYRELU_H
#define ONNC_TARGET_BM1880_TGLEAKYRELU_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/LeakyRelu.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class LeakyRelu : public onnc::LeakyRelu
{
public:
  LeakyRelu(const FloatAttr &pAlpha);

  ~LeakyRelu();

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override { pV.visit(*this); }

  void accept(ComputeVisitor &pV) const override { pV.visit(*this); }

private:
  int m_GTRShiftWidth;
  int m_LERShiftWidth;
  int m_GTScale;
  int m_LEScale;
};

} // namespace BM188X
} // namespace onnc

#endif