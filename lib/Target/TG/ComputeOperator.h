//===- ComputeOperator.h---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef COMPUTE_OPERATOR_H
#define COMPUTE_OPERATOR_H

#include <onnc/IR/ONNXUtils.h>
#include <onnc/Support/IOStream.h>
#include <onnx/common/ir.h>
#include <string>

namespace onnc {

using MemTable = std::map<std::string, uint64_t>;

enum class MemType { NEURON, WEIGHT };

struct MemOperand {
  std::string name;
  uint64_t addr;
  size_t count;
  size_t size;
  ::onnx::TensorProto_DataType type;
  MemType memType;
  const ::onnx::Value *value;
  MemOperand(std::string pName, const ::onnx::Value *pValue, MemType pMemType);
};

std::ostream &operator<<(std::ostream &pOS, const MemOperand &pMem);

class ComputeOperator2
{
public:
  ComputeOperator2(const ::onnx::Node &pNode, const std::string &pTypeName);

  virtual ~ComputeOperator2() = default;

  // FIXME changed to use ENUM when compute ir finish
  const std::string &getTypeName() { return m_TypeName; }

  const std::string &getLayerName() { return m_LayerName; };

  std::vector<MemOperand *> &getMemOperands() { return m_MemOperands; };

  virtual void emit(void) const = 0;

  virtual void memAlloc(MemTable &p_MemLayout);

  virtual void print(OStream &pOS) const;

  ComputeOperator2 *addMemOperand(MemOperand *pMemOperand);

private:
  std::string m_TypeName;
  std::string m_LayerName;

protected:
  std::vector<MemOperand *> m_MemOperands;
};

} // namespace onnc

#endif // COMPUTE_OPERATOR_H
