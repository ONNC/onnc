//===- Use.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_USE_H
#define ONNC_IR_COMPUTE_USE_H
#include <string>

namespace onnc {

class Operator;
class Value;

/** \class Use
 *  \brief Use represents an edge between its users and definition.
 *
 *  Unlike conventional compiler, inneural network, an operator may have
 *  multiple operands, thus, we need two fields - getUser() and getOperandNo()
 *  to locate one operand.
 */
class Use
{
public:
  Value *get();

  Operator *getUser();

  unsigned getOperandNo() const;

private:
  Operator *m_User;
  unsigned int m_OperandNo;
  Value *m_Value;
};

} // namespace of onnc

#endif
