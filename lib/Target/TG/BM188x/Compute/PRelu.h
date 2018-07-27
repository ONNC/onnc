//===- PRelu.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGPRELU_H
#define ONNC_TARGET_BM1880_TGPRELU_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/PRelu.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class PRelu : public onnc::PRelu
{
public:
  PRelu();

  ~PRelu();

  const IntAttr &getGTRShiftWidth() const { return m_GTRShiftWidth; }

  const IntAttr &getLERShiftWidth() const { return m_LERShiftWidth; }

  const IntAttr &getGTScale() const { return m_GTScale; }

  void setGTRShiftWidth(const IntAttr &pGSW) { m_GTRShiftWidth = pGSW; }

  void setLERShiftWidth(const IntAttr &pLSW) { m_LERShiftWidth = pLSW; }

  void setGTScale(const IntAttr &pGS) { m_GTScale = pGS; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

private:
  IntAttr m_GTRShiftWidth;
  IntAttr m_LERShiftWidth;
  IntAttr m_GTScale;
};

} // namespace BM188X
} // namespace onnc

#endif
