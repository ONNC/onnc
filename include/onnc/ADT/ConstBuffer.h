//===- ConstBuffer.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_CONSTANT_BUFFER_H
#define ONNC_ADT_CONSTANT_BUFFER_H
#include <vector>
#include <array>
#include <string>
#include <onnc/ADT/Buffer.h>

namespace onnc {

class ConstBuffer
{
public:
  typedef size_t size_type;

public:
  ConstBuffer();

  ConstBuffer(const void* pData, size_type pSize);

  ConstBuffer(const Buffer& pBuffer);

  /// constructor from std::vector
  template <typename PodType, typename Allocator>
  ConstBuffer(const std::vector<PodType, Allocator>& pData) {
    m_Data   = (pData.empty())? nullptr: &pData[0];
    m_Length = (pData.empty())? 0: pData.size() * sizeof(PodType);
  }

  /// constructor from std::vector with limited size in bytes.
  template <typename PodType, typename Allocator>
  ConstBuffer(const std::vector<PodType, Allocator>& pData, size_t pMaxSizeInBytes) {
    m_Data = (pData.empty())? nullptr: &pData[0];

    size_t size = pData.size() * sizeof(PodType);
    m_Length = (size < pMaxSizeInBytes)? size: pMaxSizeInBytes;
  }

  template <typename PodType, size_t N>
  ConstBuffer(const PodType (&pData)[N])
    : m_Data(pData), m_Length(N*sizeof(PodType)) {
  }

  template <typename PodType, size_t N>
  ConstBuffer(const PodType (&pData)[N], size_type pMaxSizeInBytes)
    : m_Data(pData) {
    size_type size = N*sizeof(PodType);
    m_Length = (size < pMaxSizeInBytes)? size: pMaxSizeInBytes;
  }

  template <typename PodType, size_t N>
  ConstBuffer(const std::array<PodType, N>& pData)
    : m_Data(pData.c_array()), m_Length(pData.size() * sizeof(PodType)) {
  }

  template <typename PodType, size_t N>
  ConstBuffer(const std::array<PodType, N>& pData, size_type pMaxSizeInBytes)
    : m_Data(pData.c_array()) {
    size_type size = N*sizeof(PodType);
    m_Length = (size < pMaxSizeInBytes)? size: pMaxSizeInBytes;
  }

  template <typename CharT, typename Traits, typename Allocator>
  ConstBuffer(const std::basic_string<CharT, Traits, Allocator>& pData)
    : m_Data(pData.data()), m_Length(pData.size()*sizeof(CharT)) {
  }

  template <typename CharT, typename Traits, typename Allocator>
  ConstBuffer(const std::basic_string<CharT, Traits, Allocator>& pData,
              size_type pMaxSizeInBytes)
    : m_Data(pData.data()) {
    size_type size = pData.size() * sizeof(CharT);
    m_Length = (size < pMaxSizeInBytes)? size: pMaxSizeInBytes;
  }

  size_type size() const { return m_Length; }

  const void* raw() const { return m_Data; }

private:
  const void* m_Data;
  size_type m_Length;
};

} // end of namespace onnc

#endif
