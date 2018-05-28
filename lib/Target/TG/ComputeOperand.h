#pragma once

#include <onnc/Support/IOStream.h>
#include <onnx/common/ir.h>
#include <string>

namespace onnc {

using MemTable = std::map<std::string, uint64_t>;

struct MemOperand {
  std::string name;
  uint64_t addr;
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

  virtual void emit(void) const = 0;

  virtual void memAlloc(MemTable &p_MemLayout);

  virtual void print(OStream &pOS) const;

private:
  std::string m_TypeName;
  std::string m_LayerName;

protected:
  std::vector<MemOperand> m_MemOperands;
};

} // namespace onnc
