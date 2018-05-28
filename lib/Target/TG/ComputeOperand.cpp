#include "ComputeOperand.h"
#include <iomanip>

namespace onnc {

std::ostream &operator<<(std::ostream &pOS, const MemOperand &pMem)
{
  pOS << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0')
      << pMem.addr;
  return pOS;
}

ComputeOperand2::ComputeOperand2(const ::onnx::Node &pNode,
                               const std::string &pTypeName)
    : m_TypeName(pTypeName)
{
  m_LayerName = const_cast< ::onnx::Node &>(pNode).output()->uniqueName();
}

void ComputeOperand2::memAlloc(MemTable &pPMemLayout)
{
  for (auto &i : m_MemOperands) {
    if (pPMemLayout.find(i.name) != pPMemLayout.end())
      i.addr = pPMemLayout[i.name];
  }
}

void ComputeOperand2::print(OStream &pOS) const
{
  pOS << m_TypeName << " " << m_LayerName << "\n";
}

} // namespace onnc
