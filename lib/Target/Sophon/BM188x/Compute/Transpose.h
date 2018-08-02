//===- Transpose.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGTRANSPOSE_H
#define ONNC_TARGET_BM1880_TGTRANSPOSE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Transpose.h>
#include <vector>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class Transpose : public onnc::Transpose
{
public:
  static char ID;

public:
  Transpose();

  ~Transpose();

  void init(const ::onnx::Node &pNode);

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

  int getCorrectW() const { return m_W; }

  const std::vector<int>& getOrder() const { return m_Order; }

  const std::vector<int>& getOutputShape() const { return m_OutputShape; }

  bool needPermute() const { return m_NeedPermute; }

private:
  int m_W;
  std::vector<int> m_Order;
  std::vector<int> m_OutputShape;
  bool m_NeedPermute;
};

} // namespace BM188X
} // namespace onnc

#endif
