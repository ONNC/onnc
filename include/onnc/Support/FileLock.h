//===- FileLock.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_FILE_LOCK_H
#define ONNC_SUPPORT_FILE_LOCK_H
#include <onnc/Support/Path.h>

namespace onnc {

/** \class FileLock
 *  \brief Advisory lock on a file
 */
class FileLock
{
public:
  FileLock();

  /// apply the lock on file @ref pFileName
  FileLock(const Path& pFileName);

  ~FileLock();

  /// Apply the lock on file @ref pFileName
  /// Can't apply the same lock object on multiple files.
  /// @retval false If the lock had been applied on the only file.
  bool apply(const Path& pFileName);

  void lock();

  void unlock();

  /// @return The filename locked on.
  const Path& file() const { return m_FilePath; }

private:
  /// @retval true If @ref pFD and file() refer to the same inode
  bool isDirty(int pFD) const;

private:
  Path m_FilePath;
  int m_FD;
};

} // namespace of onnc

#endif
