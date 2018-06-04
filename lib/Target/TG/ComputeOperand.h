#pragma once

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
  MemOperand(std::string pName, const std::vector< ::onnx::Dimension> &pDim,
             ::onnx::TensorProto_DataType pType, MemType pMemType);
};

std::ostream &operator<<(std::ostream &pOS, const MemOperand &pMem);

class ComputeOperand2
{
public:
  ComputeOperand2(const ::onnx::Node &pNode, const std::string &pTypeName);

  virtual ~ComputeOperand2() = default;

  // FIXME changed to use ENUM when compute ir finish
  const std::string &getTypeName() { return m_TypeName; }

  const std::string &getLayerName() { return m_LayerName; };

  std::vector<MemOperand *> &getMemOprnds() { return m_MemOperands; };

  virtual void emit(void) const = 0;

  virtual void memAlloc(MemTable &p_MemLayout);

  virtual void print(OStream &pOS) const;

  ComputeOperand2 *addMemOperand(MemOperand *pMemOperand);

private:
  std::string m_TypeName;
  std::string m_LayerName;

protected:
  std::vector<MemOperand *> m_MemOperands;
};

} // namespace onnc
