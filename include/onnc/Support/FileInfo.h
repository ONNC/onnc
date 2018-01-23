//===- FileInfo.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_FILE_INFO_H
#define ONNC_SUPPORT_FILE_INFO_H
#include <onnc/Support/FileStatus.h>
#include <onnc/Support/Path.h>
#include <vector>

namespace onnc {

/** \class onnc::FileInfo
 *  \brief provides system-independent file information.
 *
 *  FileInfo provides information about a file's name, position (path) and
 *  status in the file system.
 */
class FileInfo
{
public:
  FileInfo();

  explicit FileInfo(const Path& pPath);

  FileInfo(const FileInfo& pCopy);

  ~FileInfo() { }

  FileInfo& operator=(const FileInfo& pCopy);

  FileInfo& assign(const Path& pPath);

  FileInfo& operator=(const Path& pPath) { return assign(pPath); }

  const Path& path() const { return m_Path; }

  const FileStatus status() const { return m_Status; }

private:
  Path m_Path;
  FileStatus m_Status;
};

/// Synonym for std::vector<FileInfo>.
typedef std::vector<FileInfo> FileInfoList;

} // namespace of onnc

namespace std {

inline bool operator==(const onnc::FileInfo& pX, const onnc::FileInfo& pY) {
  return (pX.path() == pY.path());
}

inline bool operator!=(const onnc::FileInfo& pX, const onnc::FileInfo& pY) {
  return !(pX == pY);
}

} // namespace of std

#endif
