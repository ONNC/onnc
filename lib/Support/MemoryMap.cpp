//===- MemoryMap.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/MemoryMap.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace onnc;

std::unique_ptr<MemoryMap>
MemoryMap::mapFile(const std::string &pName)
{
  int fd;
  fd = ::open(pName.c_str(), O_RDONLY);

  if (fd == -1)
    return nullptr;

  int stat_ret;
  struct stat stat_buf;
  stat_ret = ::fstat(fd, &stat_buf);
  if (stat_ret != 0)
    return nullptr;

  SystemError err;
  std::unique_ptr<MemoryMap> ret(new MemoryMap(fd, stat_buf.st_size, 0, err));

  ::close(fd);

  return ret;
}
