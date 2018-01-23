//===- ConstBuffer.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/ConstBuffer.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConstBuffer
//===----------------------------------------------------------------------===//
ConstBuffer::ConstBuffer()
  : m_Data(nullptr), m_Length(0) {
}

ConstBuffer::ConstBuffer(const void* pData, size_type pSize)
  : m_Data(pData), m_Length(pSize) {
}

ConstBuffer::ConstBuffer(const Buffer& pBuffer)
  : m_Data(pBuffer.raw()), m_Length(pBuffer.size()) {
}
