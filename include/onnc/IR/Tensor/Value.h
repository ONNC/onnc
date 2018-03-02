//===- Value.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_TENSOR_VALUE_H
#define ONNC_IR_TENSOR_VALUE_H
#include <onnc/Tensor/Use.h>
#include <vector>
#include <string>

namespace onnc {
namespace tensor {

/** \class Value
 *  \brief Value represents all values computed by a program. Use-define chain
 *  is a very important concept in compiler theory. A value may be used as an
 *  operand in operators.
 */
class Value
{
public:
  typedef std::vector<Use> UseList;

public:
  Value(const std::string& pName)
    : m_Name(pName) {
  }

  const std::string& getName() const { return m_Name; }

  Define* getDefine() { return m_pDefine; }

  unsigned int getDefineNo() { return m_DefineNo; }

  // TODO: Iterator
  const UseList& getUses() const return m_UseList; }

  // TODO: Iterator
  UseList& getUses() return m_UseList; }

  void replaceAllUsesWith(Value *v);

private:
  Define* m_pDefine;
  unsigned int m_DefineNo;
  UseList m_Uses;
  std::string m_Name;
};

} // namespace of tensor
} // namespace of onnc

#endif
