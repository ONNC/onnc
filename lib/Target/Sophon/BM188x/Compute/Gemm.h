//===- Gemm.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGGEMM_H
#define ONNC_TARGET_BM1880_TGGEMM_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Gemm.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class Gemm : public onnc::Gemm
{
public:
  static char ID;

public:
  enum ActivationMethod { RELU = 0, SIGMOID, TANH, ELU, PRELU };

public:
  Gemm();

  ~Gemm();

  void init(const ::onnx::Node &pNode);

  int getRShiftWidth() const { return m_RShiftWidth; }

  void setRShiftWidth(int pValue) { m_RShiftWidth = pValue; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  int m_InRowNum;
  int m_InColNum;
  int m_OutColNum;
  int m_HaveBias;
  bool m_WeightTp;
  bool m_EnableRelu;
  int m_RShiftWidth;
};

} // namespace BM188X
} // namespace onnc

#endif
