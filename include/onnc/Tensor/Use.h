//===- Use.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TENSOR_USE_H
#define ONNC_TENSOR_USE_H
#include <string>

namespace onnc {
namespace tensor {

class Operator;
class Value;

class Use {
public:
  Value *get();
  Operator *getUser();
  unsigned getOperandNo() const;

private:
  Operator *m_User;
  unsigned int m_OperandNo;
  Value *m_Value;
};

} // namespace of tensor
} // namespace of onnc

#endif
