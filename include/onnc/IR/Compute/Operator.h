//===- Operator.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_TENSOR_OPERATOR_H
#define ONNC_IR_TENSOR_OPERATOR_H
#include <string>
#include <vector>

namespace onnc {
namespace tensor {

class Value;

/** \class Operator
 *  \brief Operator represents an operator for tensor flows.
 */
class Operator : public Define
{
public:
  Operator(const std::string& pName)
    : Define(pName) {
  }

  Value *getInput(unsigned pIdx);

  const Value *getInput(unsigned pIdx) const;

  Value *getOutput(unsigned pIdx);

  const Value *getOutput(unsigned pIdx) const;

private:
  typedef std::vector<Value*> ValueRefList;

private:
  ValueRefList m_Inputs;  //< all values used as inputs in this operator
  ValueRefList m_Outputs; //< all values used as outputs in this operator
};

} // namespace of tensor
} // namespace of onnc

#endif
