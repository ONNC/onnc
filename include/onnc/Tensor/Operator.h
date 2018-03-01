//===- Operator.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TENSOR_OPERATOR_H
#define ONNC_TENSOR_OPERATOR_H
#include <vector>
#include <onnc/ADT/StringRef.h>

namespace onnc {
namespace tensor {

class Value;

class Operator : public Define
{
public:
  Operator(StringRef p_name): Define(p_name) {}

  Value *getInput(unsigned index);

  Value *getOutput(unsigned index);

private:
  std::vector<Value*> m_Inputs;
  std::vector<Value*> m_Outputs;
};

} // namespace of tensor
} // namespace of onnc

#endif
