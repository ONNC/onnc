//===- CodeEmitVisitor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM188X_QUANTIZE_VISITOR_H
#define ONNC_TARGET_BM188X_QUANTIZE_VISITOR_H
#include "BM188xVisitor.h"

namespace onnc {
namespace BM188X {

class CodeEmitVisitor : public BM188xVisitor
{
  using BM188X::BM188xVisitor::visit;
public:
  void visit(const BM188X::AveragePool& pAveragePool) override;

  void visit(const BM188X::Concat& pConcat) override;

  void visit(const BM188X::Conv& pConv) override;

  void visit(const BM188X::Gemm& pGemm) override;

  void visit(const BM188X::GlobalAveragePool& pGAP) override;

  void visit(const BM188X::LRN& pLRN) override;

  void visit(const BM188X::LeakyRelu& pLeakyRelu) override;

  void visit(const BM188X::Load& pLoad) override;

  void visit(const BM188X::MaxPool& pMaxPool) override;

  void visit(const BM188X::PRelu& pPRelu) override;

  void visit(const BM188X::Pool& pPool) override;

  void visit(const BM188X::Relu& pRelu) override;

  void visit(const BM188X::Scale& pScale) override;

  void visit(const BM188X::Store& pStore) override;

  void visit(const BM188X::Sum& pSum) override;

  void visit(const BM188X::Transpose& pTranspose) override;

  void visit(const onnc::Upsample& pUpsample) override;
};

} // namespace BM188X
} // namespace onnc

#endif
