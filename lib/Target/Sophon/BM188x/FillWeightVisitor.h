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
#include <onnc/IR/Compute/Tensor.h>

namespace onnc {
namespace BM188X {

class FillWeightVisitor : public BM188xVisitor
{
public:
  using Weight = GenWeightPass::WeightType;

public:
  using BM188xVisitor::visit;

  void visit(const BM188X::Conv& pConv) override;

  void visit(const BM188X::SlicedConv& pSlicedConv) override;

  void visit(const BM188X::Gemm &pGemm) override;

  void visit(const BM188X::LRN &pLRN) override;

  void visit(const BM188X::PRelu &pPRelu) override;

  void visit(const BM188X::BMScale &pBMScale) override;

  FillWeightVisitor(Weight& pWeight);

private:
  /// remember the written TLConv's memory operands to prevent from
  /// duplicatedly written.
  typedef std::unordered_set<const onnc::Value *> DoneOpndSet;

private:
  static void Convert(Weight &pWeight, const std::vector<int8_t> &pData,
                      int pKS, int pIC, int pOC);

  static void Append8bit(Weight &pW, const std::vector<int8_t> &pData);

  static void Append16bit(Weight &pW, const std::vector<int16_t> &pData);

  bool isWritten(const onnc::Value &pValue) const;

  void setWritten(const onnc::Value &pValue);

private:
  Weight& m_Weight;
  DoneOpndSet m_DoneOpndSet;
};

} // namespace BM188X
} // namespace onnc

#endif
