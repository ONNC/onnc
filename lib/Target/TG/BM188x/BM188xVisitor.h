//===- BM188xVisitor.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM188X_VISITOR_H
#define ONNC_TARGET_BM188X_VISITOR_H
#include <onnc/IR/ComputeVisitor.h>

namespace onnc {
namespace BM188X {

class AveragePool;
class Concat;
class Conv;
class Gemm;
class GlobalAveragePool;
class LRN;
class LeakyRelu;
class Pool;
class PRelu;
class Sum;
class Store;
class Scale;
class Relu;
  
class BM188xVisitor : public ComputeVisitor
{
public:
  static char ID;

public:
  using ComputeVisitor::visit;

  virtual void visit(const BM188X::AveragePool& pAveragePool) { }

  virtual void visit(const BM188X::Pool& pPool) { }

  virtual void visit(const BM188X::Store& pStore) { }

  virtual void visit(const BM188X::Sum& pSum) { }

  virtual void visit(const BM188X::Scale& pScale) { }

  virtual void visit(const BM188X::Relu& pRelu) { }

  virtual void visit(BM188X::AveragePool& pAveragePool) { }

  virtual void visit(BM188X::Pool& pPool) { }

  virtual void visit(BM188X::Store& pStore) { }

  virtual void visit(BM188X::Sum& pSum) { }

  virtual void visit(BM188X::Scale& pScale) { }

  virtual void visit(BM188X::Relu& pRelu) { }
  
  BM188xVisitor();

  /// for dyn_cast
  static bool classof(const ComputeVisitor* pVisitor);
};

} // namespace BM188X
} // namespace onnc

#endif
