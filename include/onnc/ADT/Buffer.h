//===- Buffer.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_BUFFER_H
#define ONNC_ADT_BUFFER_H
#include <vector>
#include <array>

namespace onnc {

/** \class Buffer
 *  \brief Buffer provides a interface to maintain a memory buffer.
 */
class Buffer
{
public:
  typedef size_t size_type;

public:
  Buffer();

  Buffer(void* pData, size_type pSize);

  /// Buffer keeps the memory pointer only and isn't responsible for declaiming
  /// the memory space.
  ~Buffer() { }

  /// constructor from std::vector
  template <typename PodType, typename Allocator>
  Buffer(std::vector<PodType, Allocator>& pData) {
    m_Data   = (pData.empty())? nullptr: &pData[0];
    m_Length = (pData.empty())? 0: pData.size() * sizeof(PodType);
  }

  /// constructor from std::vector with limited size in bytes.
  template <typename PodType, typename Allocator>
  Buffer(std::vector<PodType, Allocator>& pData, size_t pMaxSizeInBytes) {
    m_Data = (pData.empty())? nullptr: &pData[0];

    size_t size = pData.size() * sizeof(PodType);
    m_Length = (size < pMaxSizeInBytes)? size: pMaxSizeInBytes;
  }

  template <typename PodType, size_t N>
  Buffer(PodType (&pData)[N])
    : m_Data(pData), m_Length(N*sizeof(PodType)) {
  }

  template <typename PodType, size_t N>
  Buffer(PodType (&pData)[N], size_type pMaxSizeInBytes)
    : m_Data(pData) {
    size_type size = N*sizeof(PodType);
    m_Length = (size < pMaxSizeInBytes)? size: pMaxSizeInBytes;
  }

  template <typename PodType, size_t N>
  Buffer(std::array<PodType, N>& pData)
    : m_Data(pData.c_array()), m_Length(pData.size() * sizeof(PodType)) {
  }

  template <typename PodType, size_t N>
  Buffer(std::array<PodType, N>& pData, size_type pMaxSizeInBytes)
    : m_Data(pData.c_array()) {
    size_type size = N*sizeof(PodType);
    m_Length = (size < pMaxSizeInBytes)? size: pMaxSizeInBytes;
  }

  size_type size() const { return m_Length; }

  void* raw() { return m_Data; }

  const void* raw() const { return m_Data; }

  void assign(void* pData, size_type pSize) {
    m_Data = pData;
    m_Length = pSize;
  }

private:
  void* m_Data;
  size_type m_Length;
};

/// Free the memory space hold by a buffer.
void free(Buffer& pBuffer);

} // end of namespace onnc

#endif
