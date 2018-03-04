//===- FileHandle.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_FILE_HANDLE_H
#define ONNC_SUPPORT_FILE_HANDLE_H
#include <onnc/ADT/Flags.h>
#include <onnc/Support/Path.h>
#include <onnc/Support/ErrorCode.h>
#include <sys/file.h>

namespace onnc {

/** \class FileHandle
 *  \brief FileHandle class provides an interface for reading from and writing
 *  to files.
 *
 *  Operators of FileHandle should neither throw exceptions nor call expressive
 *  diagnostic output.
 */
class FileHandle
{
public:
  enum OpenModeEnum {
    kNotOpen = 0x00,
    kReadOnly = 0x01,
    kWriteOnly = 0x02,
    kReadWrite = kReadOnly | kWriteOnly,
    kAppend = 0x04,
    kCreate = 0x08,
    kTruncate = 0x10,
    kUnknown = 0xFF
  };

  typedef Flags<OpenModeEnum> OpenMode;

  enum PermissionEnum {
    kReadOwner = 0x0400,
    kWriteOwner = 0x0200,
    kExeOwner = 0x0100,
    kReadGroup = 0x0040,
    kWriteGroup = 0x0020,
    kExeGroup = 0x0010,
    kReadOther = 0x0004,
    kWriteOther = 0x0002,
    kExeOther = 0x0001,
    kSystem = 0xFFFF
  };

  typedef Flags<PermissionEnum> Permission;

  enum LockModeEnum {
    kSharedLock = LOCK_SH,
    kExclusiveLock = LOCK_EX,
    kNonBlocking = LOCK_NB,
    kUnlock = LOCK_UN
  };

  typedef Flags<LockModeEnum> LockMode;

public:
  FileHandle();

  ~FileHandle();

  /// open the file.
  SystemError open(const Path& pPath, OpenMode pMode, Permission pPerm = kSystem);

  SystemError delegate(int pFD, OpenModeEnum pMode = kUnknown);

  SystemError close();

  // truncate the file up to the size @ref pSize.
  SystemError truncate(size_t pSize);

  /// read the file content from @ref pStartOffset with lenght @ref pLength
  /// into the memory buffer @ref pMemBuffer
  SystemError read(void* pMemBuffer, size_t pStartOffset, size_t pLength);

  /// write the memory buffer @ref pMemBuffer into the file starting from
  /// @ref pStartOffset with length @ref pLength.
  SystemError write(const void* pMemBuffer, size_t pStartOffset, size_t pLength);

  /// map the file into memory @ref pMemBuffer
  /// @param[in] pStartOffset mapping the file starting at byte offset
  /// @param[in] pLength mapping for at most pLength bytes
  SystemError mmap(void*& pMemBuffer, size_t pStartOffset, size_t pLength);

  SystemError munmap(void* pMemBuffer, size_t pLength);

  const Path& path() const { return m_Path; }

  size_t size() const { return m_Size; }

  /// explore underlying file handler
  int handler() const { return m_Handler; }

  bool isOpen() const;

  bool isReadable() const;

  bool isWritable() const;

  bool isReadWrite() const;

  SystemError lock(LockMode pMode);

  SystemError unlock();

  SystemError sync();

private:
  Path m_Path;
  int m_Handler;
  unsigned int m_Size;
  OpenMode m_OpenMode;
};

} // namespace of onnc

#endif
