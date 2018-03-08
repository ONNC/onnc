#include "OperatorVisitor.h"

namespace onnc {
namespace tensor {

void OperatorVisitor::visit(Operator& pOper) {
};

void OperatorVisitor::visit(Add& pOper) {
 this->visit(static_cast<Operator&>(pOper));
}


class DFSOperatorVisitor : public OperatorVisitor {
public:
  void visit(Operator& oper) override {
  };
};


} // namespace tensor
} // namespace onnc
