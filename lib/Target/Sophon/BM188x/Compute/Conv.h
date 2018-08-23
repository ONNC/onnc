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

  bool isDoBias() const { return m_BiasIdx != -1; }

  bool isDoScale() const { return m_ScaleIdx != -1; }

  bool isDoScaleBias() const { return m_ScaleBiasIdx != -1; }

  bool isDoRelu() const { return m_DoRelu; }

  int getRShiftWidth() const { return m_RShiftWidth; }

  float getConvOuputThreshold() const { return m_ConvOutputThreshold; }

  void setRShiftWidth(int pValue) { m_RShiftWidth = pValue; }

  void setScaleRShiftWidth(int pV) { m_ScaleRShiftWidth = pV; }

  void setConvOutputThreshold(float pV) { m_ConvOutputThreshold = pV; }

  void setBiasIdx(int pIdx) { m_BiasIdx = pIdx; }

  void setScaleIdx(int pIdx) { m_ScaleIdx = pIdx; }

  void setScaleBiasIdx(int pIdx) { m_ScaleBiasIdx = pIdx; }

  void setDoRelu(bool pV) { m_DoRelu = pV; }

  int getScaleRShiftWidth() const { return m_ScaleRShiftWidth; }

  const Tensor *getBias() const
  {
    return m_BiasIdx == -1 ? nullptr : getInput(m_BiasIdx);
  }

  const Tensor *getScale() const
  {
    return m_ScaleIdx == -1 ? nullptr : getInput(m_ScaleIdx);
  }

  const Tensor *getScaleBias() const
  {
    return m_ScaleBiasIdx == -1 ? nullptr : getInput(m_ScaleBiasIdx);
  }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  int m_RShiftWidth;
  int m_ScaleRShiftWidth;
  float m_ConvOutputThreshold;
  // TODO add prelu_param
  bool m_DoRelu;
  int m_BiasIdx{ -1 };
  int m_ScaleIdx{ -1 };
  int m_ScaleBiasIdx{ -1 };
};

} // namespace BM188X
} // namespace onnc

#endif
