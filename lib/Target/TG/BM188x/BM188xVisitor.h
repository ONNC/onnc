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

class Sum;
class Store;
class Scale;
  
class BM188xVisitor : public ComputeVisitor
{
public:
  static char ID;

public:
  using ComputeVisitor::visit;

  virtual void visit(const Store& pStore) { }

  virtual void visit(const Sum& pSum) { }

  virtual void visit(Store& pStore) { }

  virtual void visit(Sum& pSum) { }

  virtual void visit(Scale& pScale) { }
  
  BM188xVisitor();

  /// for dyn_cast
  static bool classof(const ComputeVisitor* pVisitor);
};

} // namespace BM188X
} // namespace onnc

#endif
