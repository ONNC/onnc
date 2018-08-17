//===- FileStatus.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_FILE_STATUS_H
#define ONNC_SUPPORT_FILE_STATUS_H
#include <onnc/Support/DataTypes.h>

namespace onnc {

class Path;

/** \class onnc::FileStatus
 *  \brief summary of a file.
 *
 *  status() function sets the given FileStatus by the given path.
 */
class FileStatus
{
public:
  // type() returns one of these values
  enum Type {
   kError,
   kStatusUnknown = kError,
   kSuccess,
   kFileNotFound,
   kRegularFile,
   kDirectoryFile,
   kSymlinkFile,
   kBlockFile,
   kCharacterFile,
   kFifoFile,
   kSocketFile,
   kReparseFile,
   kTypeUnknown,
   kStatusKnown,
   kIsSymLink
  };

  // permission() returns the union of these values.
  enum Permission {
    kUserMask  = 0700,
    kUserRead  = 0400,
    kUserWrite = 0200,
    kUserExec  = 0100,
    kGrpMask   = 0070,
    kGrpRead   = 0040,
    kGrpWrite  = 0020,
    kGrpExec   = 0010,
    kAllMask   = 0007,
    kAllRead   = 0004,
    kAllWrite  = 0002,
    kAllExec   = 0001
  };

public:
  FileStatus()
    : m_Value(kStatusUnknown) {}

  explicit FileStatus(Type pType)
    : m_Value(pType) {}

  /// the size of file, in bytes.
  unsigned int size() const { return m_Size; }

  void setSize(unsigned int pSize) { m_Size = pSize; }

  Type type() const   { return m_Value; }

  void setType(Type pType)   { m_Value = pType; }

  uint32_t permission() const { return m_Permission; }

  void setPermission(uint32_t pPermission) { m_Permission = pPermission; }

private:
  Type m_Value;
  unsigned int m_Size;
  uint32_t m_Permission;
};

/// Checks the status of the \ref pPathName.
/// @param [in]  pPathName   the path name.
/// @param [out] pFileStatus the result of status.
void status(const Path& pPathName, FileStatus& pFileStatus);

/// Checks the status of the \ref pFD file descriptor.
/// @param [in]  pFD   the file descriptor
/// @param [out] pFileStatus the result of status.
void status(int pFD, FileStatus& pFileStatus);

} // namespace of onnc

#endif
