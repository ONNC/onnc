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

MemOperand::MemOperand(std::string pName, const ::onnx::Value *pValue,
                       MemType pMemType)
    : name(pName), addr(0x0), size(0), type(pValue->elemType()),
      memType(pMemType), value(pValue)
{
  count = 1;
  auto &dim = pValue->sizes();
  for (size_t i = 0; i < dim.size(); i++) {
    count *= dim[i].dim;
  }
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
    if (pPMemLayout.find(i->name) != pPMemLayout.end())
      i->addr = pPMemLayout[i->name];
  }
}

void ComputeOperand2::print(OStream &pOS) const
{
  pOS << m_TypeName << " " << m_LayerName << "\n";
}

ComputeOperand2 *ComputeOperand2::addMemOperand(MemOperand *pMemOperand)
{
  m_MemOperands.push_back(pMemOperand);
  return this;
}

} // namespace onnc
