//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "ComputeOperator.h"
#include <iomanip>

using namespace onnc;

//===---------------------------------------------------------------------===//
// Non-member function
//===---------------------------------------------------------------------===//
std::ostream &onnc::operator<<(std::ostream &pOS, const MemOperand &pMem)
{
  pOS << "<" << std::dec << pMem.m_Count << " x "
      << TensorProto_DataType_Name(pMem.m_Type) << ">(0x" << std::hex
      << pMem.m_Addr << ") %" << pMem.m_Name;
  return pOS;
}

//===---------------------------------------------------------------------===//
// MemOperand
//===---------------------------------------------------------------------===//
MemOperand::MemOperand(std::string pName, const ::onnx::Value *pValue,
                       MemType pMemType)
    : m_Name(pName), m_Addr(0x0), m_Size(0), m_Type(pValue->elemType()),
      m_MemType(pMemType), m_Value(pValue), m_IsWrittenInBin(false)
{
  m_Count = 1;
  auto &dim = pValue->sizes();
  for (size_t i = 0; i < dim.size(); i++) {
    m_Count *= dim[i].dim;
  }
}

//===---------------------------------------------------------------------===//
// ComputeOperator2
//===---------------------------------------------------------------------===//
ComputeOperator2::ComputeOperator2(const ::onnx::Node &pNode,
                                   const std::string &pTypeName)
    : m_TypeName(pTypeName)
{
  m_LayerName = const_cast< ::onnx::Node &>(pNode).output()->uniqueName();
}

void ComputeOperator2::memAlloc(MemTable &pPMemLayout)
{
  for (auto &i : m_MemOperands) {
    if (pPMemLayout.find(i->m_Name) != pPMemLayout.end())
      i->m_Addr = pPMemLayout[i->m_Name];
  }
}

ComputeOperator2 *ComputeOperator2::addMemOperand(MemOperand *pMemOperand)
{
  m_MemOperands.push_back(pMemOperand);
  return this;
}

MemOperand *ComputeOperator2::getMemOperand(unsigned int pIdx)
{
  return m_MemOperands[pIdx];
}

const MemOperand *ComputeOperator2::getMemOperand(unsigned int pIdx) const
{
  return m_MemOperands[pIdx];
}
