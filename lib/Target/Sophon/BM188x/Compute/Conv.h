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

  bool isDoBias() const { return m_Bias != nullptr; }

  bool isDoScale() const { return m_Scale != nullptr; }

  bool isDoScaleBias() const { return m_ScaleBias != nullptr; }

  bool isDoRelu() const { return m_DoRelu; }

  int getRShiftWidth() const { return m_RShiftWidth; }

  float getConvOuputThreshold() const { return m_ConvOutputThreshold; }

  void setRShiftWidth(int pValue) { m_RShiftWidth = pValue; }

  void setScaleRShiftWidth(int pV) { m_ScaleRShiftWidth = pV; }

  void setConvOutputThreshold(float pV) { m_ConvOutputThreshold = pV; }

  void setBias(Tensor* pBias) { m_Bias = pBias; }

  void setScale(Tensor* pScale) { m_Scale = pScale; }

  void setScaleBias(Tensor* pSB) { m_ScaleBias = pSB; }

  void setDoRelu(bool pV) { m_DoRelu = pV; }

  int getScaleRShiftWidth() const { return m_ScaleRShiftWidth; }

  Tensor* getBias() const { return m_Bias; }

  Tensor* getScale() const { return m_Scale; }

  Tensor* getScaleBias() const { return m_ScaleBias; }

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
  Tensor* m_Bias;
  Tensor* m_Scale;
  Tensor* m_ScaleBias;
};

} // namespace BM188X
} // namespace onnc

#endif
