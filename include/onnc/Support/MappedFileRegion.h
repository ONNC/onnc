//===- MappedFileRegion.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_MAPPED_FILE_REGION_H
#define ONNC_MAPPED_FILE_REGION_H
#include <onnc/Support/DataTypes.h>
#include <onnc/Support/ErrorCode.h>

namespace onnc {

class MappedFileRegion
{
public:
  enum MapMode {
    readonly, ///< May only access map via const_data as read only.
    readwrite, ///< May access map via data and modify it. Written to path.
    priv ///< May modify via data, but changes are lost on destruction.
  };

public:
  MappedFileRegion(int pFD, MapMode pMode,
                   uint64_t pLength, uint64_t pOffset,
                   SystemError &pErr);

  ~MappedFileRegion();

  uint64_t size() const { return m_Size; }

  char *data() const { return reinterpret_cast<char*>(m_pMapping); }

  const char *const_data() const {
    return reinterpret_cast<const char*>(m_pMapping);
  }

  static int alignment();

private:
  SystemError init(int pFD, uint64_t pOffset, MapMode pMode);

  MappedFileRegion() = delete;
  MappedFileRegion(MappedFileRegion&) = delete;
  MappedFileRegion &operator =(MappedFileRegion&) = delete;

private:
  uint64_t m_Size;
  void *m_pMapping;
};

} // namespace of onnc

#endif
