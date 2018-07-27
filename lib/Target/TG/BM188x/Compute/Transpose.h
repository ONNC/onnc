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
  Transpose();

  ~Transpose();

  void init(const ::onnx::Node &pNode);

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

private:
  int m_W;
  std::vector<int> m_Order;
  std::vector<int> m_OutputShape;
  bool m_NeedPermute;
};

} // namespace BM188X
} // namespace onnc

#endif
