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
  static char ID;

public:
  Concat(const IntAttr& pAxis);

  ~Concat();

  void setNeedQuantizeNum(int pN)
  {
    m_NeedQuantizeNum = pN;
    m_RShiftWidth.resize(pN);
    m_ThresholdXQuantized.resize(pN);
  }

  int getNeedQuantizeNum() const { return m_NeedQuantizeNum; }

  void setRShiftWidth(unsigned int pIdx, int pValue);

  const std::vector<int>& getRShiftWidth() const;

  void setThresholdXQuantized(unsigned int pIdx, int pValue);

  const std::vector<int>& getThresholdXQuantized() const;

  void setInputDims(const std::vector<int>& pInDims) { m_InputDims = pInDims;}

  void setOutputDims(const std::vector<int>& pODims) { m_OutputDims = pODims;}

  const std::vector<int>& getInputDims() const { return m_InputDims; }

  const std::vector<int>& getOutputDims() const { return m_OutputDims; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override;

  void accept(ComputeVisitor& pVisitor) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  std::vector<int> m_InputDims;
  std::vector<int> m_OutputDims;
  int m_NeedQuantizeNum;
  std::vector<int> m_RShiftWidth;
  std::vector<int> m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
