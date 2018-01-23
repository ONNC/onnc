//===- FileSystem.inc -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

static inline bool not_found_error(int pErrno)
{
  return (pErrno == ENOENT || pErrno == ENOTDIR);
}

static inline void stat2file(struct stat& pStat, onnc::FileStatus& pResult)
{
  if(S_ISDIR(pStat.st_mode))
    pResult.setType(onnc::FileStatus::kDirectoryFile);
  else if(S_ISREG(pStat.st_mode))
    pResult.setType(onnc::FileStatus::kRegularFile);
  else if(S_ISBLK(pStat.st_mode))
    pResult.setType(onnc::FileStatus::kBlockFile);
  else if(S_ISCHR(pStat.st_mode))
    pResult.setType(onnc::FileStatus::kCharacterFile);
  else if(S_ISFIFO(pStat.st_mode))
    pResult.setType(onnc::FileStatus::kFifoFile);
  else if(S_ISLNK(pStat.st_mode))
    pResult.setType(onnc::FileStatus::kSymlinkFile);
  else if(S_ISSOCK(pStat.st_mode))
    pResult.setType(onnc::FileStatus::kSocketFile);
  else
    pResult.setType(onnc::FileStatus::kTypeUnknown);

  pResult.setSize(pStat.st_size);
  pResult.setPermission(pStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}

void onnc::status(int pFD, onnc::FileStatus& pFileStatus)
{
  struct stat path_stat;
  if(fstat(pFD, &path_stat) != 0) {
    if(not_found_error(errno))
      pFileStatus.setType(FileStatus::kFileNotFound);
    else
      pFileStatus.setType(FileStatus::kError);
  }
  else
    stat2file(path_stat, pFileStatus);
}

void onnc::status(const onnc::Path& pPathName,
                      onnc::FileStatus& pFileStatus)
{
  struct stat path_stat;
  if(lstat(pPathName.c_str(), &path_stat) != 0) {
    if(not_found_error(errno)) 
      pFileStatus.setType(FileStatus::kFileNotFound);
    else
      pFileStatus.setType(FileStatus::kError);
  }
  else
    stat2file(path_stat, pFileStatus);
}

bool onnc::is_executable(const onnc::Path& pPath)
{
  struct stat path_stat;
  if (0 != stat(pPath.c_str(), &path_stat))
    return false;

  return (0 == access(pPath.native().c_str(), X_OK) &&
           (getuid() != 0 ||
             (path_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0));
}

bool onnc::is_readable(const onnc::Path& pPath)
{
  struct stat path_stat;
  if (0 != stat(pPath.c_str(), &path_stat))
    return false;

  return (0 == access(pPath.native().c_str(), R_OK));
}

bool onnc::is_writable(const onnc::Path& pPath)
{
  struct stat path_stat;
  if (0 != stat(pPath.c_str(), &path_stat))
    return false;

  return (0 == access(pPath.native().c_str(), W_OK));
}
