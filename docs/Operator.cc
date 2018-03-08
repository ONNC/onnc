#include "Operator.h"
#include "OperatorVisitor.h"

namespace onnc {
namespace tensor {

void Operator::accept(OperatorVisitor& pV) {
  pV.visit(*this);
};

} // namespace tensor
} // namespace onnc
