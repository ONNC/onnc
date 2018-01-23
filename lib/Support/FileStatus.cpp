//===- FileStatus.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/FileStatus.h>
#include <onnc/Support/Path.h>
#include <onnc/Config/Config.h>
// Include the truly platform-specific parts. 
#if defined(ONNC_ON_UNIX)
#include "Unix/FileStatus.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/FileStatus.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/FileStatus.inc"
#endif

//===----------------------------------------------------------------------===//
// FileSystem
//===----------------------------------------------------------------------===//
bool onnc::exists(const Path &pPath)
{
  FileStatus file_status;
  status(pPath, file_status);

  return (file_status.type() != FileStatus::kError) &&
         (file_status.type() != FileStatus::kFileNotFound);
}

bool onnc::is_directory(const Path &pPath)
{
  FileStatus file_status;
  status(pPath, file_status);
  return (file_status.type() == FileStatus::kDirectoryFile);
}

bool onnc::is_regular(const Path& pPath)
{
  FileStatus file_status;
  status(pPath, file_status);
  return (file_status.type() == FileStatus::kRegularFile);
}

bool onnc::is_symlink(const Path& pPath)
{
  FileStatus file_status;
  status(pPath, file_status);
  return (file_status.type() == FileStatus::kSymlinkFile);
}
