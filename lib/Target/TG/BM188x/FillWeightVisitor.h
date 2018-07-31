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

namespace onnc {
namespace BM188X {

class FillWeightVisitor : public BM188xVisitor
{
public:
  using BM188xVisitor::visit;

  void visit(const onnc::Initializer &pOp) override;

  void visit(const BM188X::Conv& pConv) override;

  void visit(const BM188X::SlicedConv& pSlicedConv) override;

  FillWeightVisitor(GenWeightPass::WeightType& pWeight);

private:
  GenWeightPass::WeightType& m_Weight;
};

} // namespace BM188X
} // namespace onnc

#endif
