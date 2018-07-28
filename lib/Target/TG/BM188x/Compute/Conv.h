//===- Conv.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGCONV_H
#define ONNC_TARGET_BM1880_TGCONV_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Conv.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class Conv : public onnc::Conv
{
public:
  static char ID;

public:
  Conv();

  ~Conv();

  bool isDoBias() const { return m_DoBias == 1; }

  bool isDoScale() const { return m_DoScale != 0; }

  bool isDoScaleBias() const { return m_DoScaleBias != 0; }

  int getDoBias() const { return m_DoBias; }

  int getDoScale() const { return m_DoScale; }

  int getDoScaleBias() const { return m_DoScaleBias; }

  int getRShiftWidth() const { return m_RShiftWidth; }

  int getBiasIdx() const { return m_BiasIdx; }

  int getScaleIdx() const { return m_ScaleIdx; }

  int getScaleBiasIdx() const { return m_ScaleBiasIdx; }

  float getConvOuputThreshold() const { return m_ConvOutputThreshold; }

  void setRShiftWidth(int pValue) { m_RShiftWidth = pValue; }

  void setDoBias(int pV) { m_DoBias = pV; }

  void setDoScale(int pV) { m_DoScale = pV; }

  void setDoScaleBias(int pV) { m_DoScaleBias = pV; }

  void setScaleRShiftWidth(int pV) { m_ScaleRShiftWidth = pV; }

  void setBiasIdx(int pV) { m_BiasIdx = pV; }

  void setScaleIdx(int pV) { m_ScaleIdx = pV; }

  void setScaleBiasIdx(int pV) { m_ScaleBiasIdx = pV; }

  void setConvOutputThreshold(float pV) { m_ConvOutputThreshold = pV; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  int m_DoBias;
  // int m_DoRelu;
  int m_DoScale;
  int m_DoScaleBias;
  int m_RShiftWidth;
  int m_ScaleRShiftWidth;
  int m_BiasIdx, m_ScaleIdx, m_ScaleBiasIdx;
  float m_ConvOutputThreshold;
  // TODO add prelu_param
};

} // namespace BM188X
} // namespace onnc

#endif
