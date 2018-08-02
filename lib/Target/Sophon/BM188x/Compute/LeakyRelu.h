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
  static char ID;

public:
  LeakyRelu(const FloatAttr &pAlpha);

  ~LeakyRelu();

  const IntAttr &getGTRShiftWidth() const { return m_GTRShiftWidth; }

  const IntAttr &getLERShiftWidth() const { return m_LERShiftWidth; }

  const IntAttr &getGTScale() const { return m_GTScale; }

  const IntAttr &getLEScale() const { return m_LEScale; }

  void setGTRShiftWidth(const IntAttr &pGSW) { m_GTRShiftWidth = pGSW; }

  void setLERShiftWidth(const IntAttr &pLSW) { m_LERShiftWidth = pLSW; }

  void setGTScale(const IntAttr &pGS) { m_GTScale = pGS; }

  void setLEScale(const IntAttr &pLS) { m_LEScale = pLS; }
  
  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  IntAttr m_GTRShiftWidth;
  IntAttr m_LERShiftWidth;
  IntAttr m_GTScale;
  IntAttr m_LEScale;
};

} // namespace BM188X
} // namespace onnc

#endif
