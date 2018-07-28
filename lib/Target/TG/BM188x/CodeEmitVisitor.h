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
public:
  void visit(const BM188X::AveragePool& pAveragePool) override;
};

} // namespace BM188X
} // namespace onnc

#endif
