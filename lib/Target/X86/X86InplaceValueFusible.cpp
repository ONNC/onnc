//===- X86InplaceValueFusible.cpp -----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86InplaceValueFusible.h"
#include <onnc/IR/Compute/Initializer.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// InplaceValueFusible
//===----------------------------------------------------------------------===//
namespace {

class InplaceValueFusible : public ComputeVisitor
{
public:
  InplaceValueFusible()
    : m_bFusible(false) {
  }

  void visit(const Initializer& pInitializer) { m_bFusible = true; }

  bool isFusible() const { return m_bFusible; }

private:
  bool m_bFusible;
};

} // anonymous namespace

bool onnc::x86::IsInplaceValueFusible(const ComputeOperator& pOp)
{
  InplaceValueFusible fusible;
  pOp.accept(fusible);
  return fusible.isFusible();
}
