//===- FileSystem.inc -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

namespace onnc {

static inline bool not_found_error(int perrno)
{
  return perrno == ENOENT || perrno == ENOTDIR;
}

void status(int pFD, FileStatus& pFileStatus)
{
  struct ::_stat path_stat;
  if(::_fstat(pFD, &path_stat)!= 0) {
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
  else
    pFileStatus.setType(FileStatus::kTypeUnknown);

  pFileStatus.setSize(path_stat.st_size);
}

void status(const Path& pPath, FileStatus& pFileStatus)
{
  struct ::_stat path_stat;
  if(::_stat(pPath.c_str(), &path_stat)!= 0) {
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
  else
    pFileStatus.setType(FileStatus::kTypeUnknown);

  pFileStatus.setSize(path_stat.st_size);
}

} // namespace of onnc
