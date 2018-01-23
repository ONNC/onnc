//===- FileSystem.inc -----------------------------------------------------===//
//
//                               Skymizer
//
// Copyright (C) 2013, 2014, Skymizer Inc.. All rights reserved.
//
//===----------------------------------------------------------------------===//
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

static inline bool not_found_error(int pErrno)
{
  return pErrno == ENOENT || pErrno == ENOTDIR;
}

void skymizer::status(int pFD, skymizer::FileStatus& pFileStatus)
{
  struct stat path_stat;
  if(fstat(pFD, &path_stat)!= 0) {
    if(not_found_error(errno)) {
      pFileStatus.setType(FileStatus::kFileNotFound);
    }
    else
      pFileStatus.setType(FileStatus::kError);
  }
  else if(S_ISDIR(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kDirectoryFile);
  else if(S_ISREG(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kRegularFile);
  else if(S_ISBLK(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kBlockFile);
  else if(S_ISCHR(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kCharacterFile);
  else if(S_ISFIFO(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kFifoFile);
  else if(S_ISSOCK(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kSocketFile);
  else
    pFileStatus.setType(FileStatus::kTypeUnknown);

  pFileStatus.setSize(path_stat.st_size);
}

void skymizer::status(const skymizer::Path& pPathName,
                      skymizer::FileStatus& pFileStatus)
{
  struct stat path_stat;
  if(stat(pPathName.c_str(), &path_stat)!= 0) {
    if(not_found_error(errno)) {
      pFileStatus.setType(FileStatus::kFileNotFound);
    }
    else
      pFileStatus.setType(FileStatus::kError);
  }
  else if(S_ISDIR(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kDirectoryFile);
  else if(S_ISREG(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kRegularFile);
  else if(S_ISBLK(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kBlockFile);
  else if(S_ISCHR(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kCharacterFile);
  else if(S_ISFIFO(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kFifoFile);
  else if(S_ISSOCK(path_stat.st_mode))
    pFileStatus.setType(FileStatus::kSocketFile);
  else
    pFileStatus.setType(FileStatus::kTypeUnknown);

  pFileStatus.setSize(path_stat.st_size);
}
