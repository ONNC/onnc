//===- FileHandle.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/FileHandle.h>
#include <onnc/Support/Path.h>
#include <onnc/Support/FileSystem.h>
#include <onnc/Config/Config.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

// Include the truly platform-specific parts.
#if defined(ONNC_ON_UNIX)
#include "Unix/FileHandle.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/FileHandle.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/FileHandle.inc"
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// Helper functions
//===----------------------------------------------------------------------===//
inline static int oflag(FileHandle::OpenMode pMode)
{
  int result = 0x0;
  if (FileHandle::kUnknown == pMode)
    return result;

  if (FileHandle::kReadWrite == (pMode & FileHandle::kReadWrite))
    result |= O_RDWR;
  else if (FileHandle::kReadOnly == (pMode & FileHandle::kReadOnly))
    result |= O_RDONLY;
  else if (FileHandle::kWriteOnly == (pMode & FileHandle::kWriteOnly))
    result |= O_WRONLY;

  if (FileHandle::kAppend == (pMode & FileHandle::kAppend))
    result |= O_APPEND;

  if (FileHandle::kCreate == (pMode & FileHandle::kCreate))
    result |= O_CREAT;

  if (FileHandle::kTruncate == (pMode & FileHandle::kTruncate))
    result |= O_TRUNC;

  return result;
}

inline static bool get_size(int pHandler, unsigned int& pSize)
{
  struct ::stat file_stat;
  if (-1 == ::fstat(pHandler, &file_stat)) {
    pSize = 0;
    return false;
  }
  pSize = file_stat.st_size;
  return true;
}

//===----------------------------------------------------------------------===//
// FileHandle
//===----------------------------------------------------------------------===//
FileHandle::FileHandle()
  : m_Path(), m_Handler(-1), m_Size(0), m_OpenMode(kNotOpen) {
}

FileHandle::~FileHandle()
{
  if (isOpen())
    close();
}

bool FileHandle::isOpen() const
{
  if (-1 != m_Handler && kNotOpen != m_OpenMode && kUnknown != m_OpenMode)
    return true;
  return false;
}

SystemError FileHandle::open(const Path& pPath,
                             FileHandle::OpenMode pMode,
                             FileHandle::Permission pPerm)
{
  if (isOpen())
    return SystemError::kPermissionDenied;

  m_OpenMode = pMode;
  if (kSystem == pPerm)
    m_Handler = onnc::open_file(pPath, oflag(pMode));
  else
    m_Handler = onnc::open_file(pPath, oflag(pMode), pPerm);

  m_Path = pPath;
  if (m_Handler == -1) {
    m_OpenMode = OpenMode(kNotOpen);
    return SystemError(errno);
  }

  if (!get_size(m_Handler, m_Size))
    return SystemError(errno);

  return SystemError::kSuccess;
}

SystemError FileHandle::delegate(int pFD, FileHandle::OpenModeEnum pMode)
{
  if (isOpen())
    return SystemError::kPermissionDenied;

  m_Handler = pFD;
  m_OpenMode = OpenMode(pMode);

  if (!get_size(m_Handler, m_Size))
    return SystemError(errno);

  return SystemError::kSuccess;
}

SystemError FileHandle::close()
{
  if (!isOpen())
    return SystemError::kPermissionDenied;

  if (-1 == ::close(m_Handler))
    return SystemError(errno);

  m_Path.native().clear();
  m_Size = 0;
  m_OpenMode = OpenMode(kNotOpen);
  return SystemError::kSuccess;
}

SystemError FileHandle::truncate(size_t pSize)
{
  if (!isOpen() || !isWritable())
    return SystemError::kPermissionDenied;

  if (-1 == ::ftruncate(m_Handler, pSize))
    return SystemError(errno);

  m_Size = pSize;
  return SystemError::kSuccess;
}

SystemError FileHandle::read(void* pMemBuffer, size_t pStartOffset, size_t pLength)
{
  if (!isOpen() || !isReadable())
    return SystemError::kPermissionDenied;

  if (pLength == 0)
    return SystemError::kSuccess;

  ssize_t read_bytes = ::pread(m_Handler, pMemBuffer, pLength, pStartOffset);

  if (-1 == read_bytes)
    return SystemError(errno);

  return SystemError::kSuccess;
}

SystemError FileHandle::write(const void* pMemBuffer,
                              size_t pStartOffset,
                              size_t pLength)
{
  if (!isOpen() || !isWritable())
    return SystemError::kPermissionDenied;

  if (pLength == 0)
    return SystemError::kSuccess;

  ssize_t write_bytes = pwrite(m_Handler, pMemBuffer, pLength, pStartOffset);

  if (-1 == write_bytes)
    return SystemError(errno);

  return SystemError::kSuccess;
}

// Assume Unknown OpenMode is readable
bool FileHandle::isReadable() const
{
  return (m_OpenMode & kReadOnly);
}

// Assume Unknown OpenMode is writable
bool FileHandle::isWritable() const
{
  return (m_OpenMode & kWriteOnly);
}

// Assume Unknown OpenMode is both readable and writable
bool FileHandle::isReadWrite() const
{
  return (FileHandle::kReadWrite == (m_OpenMode & FileHandle::kReadWrite));
}

SystemError FileHandle::lock(LockMode pMode)
{
  if (-1 == ::flock(m_Handler, pMode))
    return SystemError(errno);
  return SystemError::kSuccess;
}

SystemError FileHandle::unlock()
{
  if (-1 == ::flock(m_Handler, kUnlock))
    return SystemError(errno);
  return SystemError::kSuccess;
}

SystemError FileHandle::sync()
{
  if (-1 == ::fsync(m_Handler))
    return SystemError(errno);
  return SystemError::kSuccess;
}
