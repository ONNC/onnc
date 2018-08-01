//===- FillWeightVisitor.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_BM188X_FILL_WEIGHT_VISITOR_H
#define ONNC_TARGET_TG_BM188X_FILL_WEIGHT_VISITOR_H
#include "BM188xVisitor.h"
#include "GenWeightPass.h"
#include <assert.h>

namespace onnc {
namespace BM188X {

class FillWeightVisitor : public BM188xVisitor
{
public:
  using Weight = GenWeightPass::WeightType;

public:
  using BM188xVisitor::visit;

  void visit(const onnc::Initializer &pOp) override;

  void visit(const BM188X::Conv& pConv) override;

  void visit(const BM188X::SlicedConv& pSlicedConv) override;

  FillWeightVisitor(Weight& pWeight);

private:
  /// remember the written TLConv's memory operands to prevent from
  /// duplicatedly written.
  typedef std::unordered_set<const onnx::Tensor*> DoneOpndSet;

private:
  static void Convert(Weight& pWeight, const std::string& pRaw,
                      int pKS, int pIC, int pOC);

  static void Append8bit(Weight& pW, const std::string &pData);

  static void Append16bit(Weight& pW, const std::string &pData);

  bool isWritten(const onnx::Tensor* pOpnd) const;

  void setWritten(const onnx::Tensor* pOpnd);

private:
  Weight& m_Weight;
  DoneOpndSet m_DoneOpndSet;
};

} // namespace BM188X
} // namespace onnc

#endif
