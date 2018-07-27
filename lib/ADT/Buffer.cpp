//===- Buffer.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/Buffer.h>
#include <cstdlib>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
void onnc::free(Buffer& pBuffer)
{
  std::free(pBuffer.raw());
  pBuffer.assign(nullptr, 0);
}

//===----------------------------------------------------------------------===//
// Buffer
//===----------------------------------------------------------------------===//
Buffer::Buffer()
  : m_Data(nullptr), m_Length(0) {
}

Buffer::Buffer(void* pData, size_type pSize)
  : m_Data(pData), m_Length(pSize) {
}
