//===- MemoryMap.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_MEMORY_MAP_H
#define ONNC_MEMORY_MAP_H
#include <onnc/Support/DataTypes.h>
#include <onnc/Support/ErrorCode.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/MappedFileRegion.h>
#include <memory>
#include <sys/mman.h>

namespace onnc {

class MemoryMap
{
public:
  static std::unique_ptr<MemoryMap> mapFile(const std::string &pName);

  MemoryMap(int pFile, uint64_t pSize, uint64_t pOffset, SystemError &pErr)
      : m_memory(pFile, MappedFileRegion::readonly,
                 getMapSize(pSize, pOffset), getMapOffset(pOffset), pErr) {
    if (pErr.isGood()) {
      const char *start = getStart(pOffset);
      init(start, start + pSize);
    }
  }
  ~MemoryMap() {}

  const char *start() const { return m_pStart; }
  const char *end() const { return m_pEnd; }
  size_t size() const { return m_pEnd - m_pStart; }

  StringRef getBuffer() const
  {
    return StringRef(m_pStart, size());
  }

  StringRef request(size_t pOffset, size_t pLength)
  {
    return StringRef(start() + pOffset, pLength);
  }

private:
  void init(const char *pStart, const char *pEnd)
  {
    m_pStart = pStart;
    m_pEnd = pEnd;
  }

  static uint64_t getMapOffset(uint64_t pOffset)
  {
    return pOffset & ~(MappedFileRegion::alignment() - 1);
  }

  static uint64_t getMapSize(uint64_t pSize, uint64_t pOffset)
  {
    return pSize + (pOffset - getMapOffset(pOffset));
  }

  const char *getStart(uint64_t pOffset)
  {
    return m_memory.const_data() + (pOffset - getMapOffset(pOffset));
  }

  MappedFileRegion m_memory;
  const char *m_pStart;
  const char *m_pEnd;
};

}

#endif
