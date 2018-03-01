//===- Value.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TENSOR_VALUE_H
#define ONNC_TENSOR_VALUE_H
#include <vector>
#include <onnc/Tensor/Use.h>

namespace onnc {
namespace tensor {

class Value {
public:
  Value(StringRef p_name): m_Name(p_name) {}

  StringRef getName();

  Define* getDefine();

  unsigned getDefineNo();

  // TODO: Iterator
  std::vector<Use> *getUses();

  void replaceAllUsesWith(Value *v);

private:
  Define* m_Define;
  unsigned int m_DefineNo;
  std::vector<Use> *m_Uses;
  std::string m_Name;
};

} // namespace of tensor
} // namespace of onnc

#endif
