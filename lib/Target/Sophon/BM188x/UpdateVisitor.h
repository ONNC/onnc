//===- UpdateVisitor.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_SOPHON_UPDATE_VISITOR_H
#define ONNC_TARGET_SOPHON_UPDATE_VISITOR_H
#include "BM188xBackend.h"
#include "BM188xVisitor.h"
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>

namespace onnc {
namespace BM188X {

class UpdateVisitor : public BM188xVisitor
{
public:
  using BM188xVisitor::visit;

  void visit(BM188X::Conv &pConv) override;

  void visit(BM188X::MaxPool &pMaxPool) override;

  void visit(BM188X::AveragePool &pAvgPool) override;

  void visit(BM188X::GlobalAveragePool &pGlobalAvgPool) override;

  void visit(BM188X::Gemm &pGemm) override;

  void visit(BM188X::LRN &pLRN) override;

  void visit(BM188X::PRelu &pPRelu) override;

  void visit(BM188X::Sum &pSum) override;

  void visit(BM188X::LeakyRelu &pLRelu) override;

  void visit(BM188X::BMScale &pBMScale) override;

  void visit(BM188X::Concat &pConcat) override;

  UpdateVisitor(const BM1880Backend *pBackend);

private:
  const BM1880Backend::LayerCtable *getLayerCtable(const ComputeOperator *node);

private:
  const BM1880Backend *m_pBackend; // NOLINT
};

} // namespace BM188X
} // namespace onnc

#endif
