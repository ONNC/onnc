#include "ComputeOperand.h"
#include <iomanip>

namespace onnc {

std::ostream &operator<<(std::ostream &pOS, const MemOperand &pMem)
{
  pOS << "<" << std::dec << pMem.count << " x "
      << TensorProto_DataType_Name(pMem.type) << ">(0x" << std::hex << pMem.addr
      << ") %" << pMem.name;
  return pOS;
}

MemOperand::MemOperand(std::string pName,
                       const std::vector< ::onnx::Dimension> &pDim,
                       ::onnx::TensorProto_DataType pType, int pTag)
{
  name = pName;
  addr = 0x0;
  count = 1;
  for (size_t i = 0; i < pDim.size(); i++) {
    count *= pDim[i].dim;
  }
  size = 0;
  type = pType;
  tag = pTag;
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
