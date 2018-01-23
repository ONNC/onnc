//===- Path.inc -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/StringRef.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

namespace onnc {

//===----------------------------------------------------------------------===//
// Directory
//===----------------------------------------------------------------------===//
void Directory::open(Directory& pDir)
{
  dirent** namelist = NULL;
  int size = scandir(pDir.path().c_str(), &namelist, NULL, alphasort);
  if (-1 == size) {
    pDir.setStatus(errno);
    return;
  }
  pDir.setSize(size);
  pDir.setStatus(SystemError::kSuccess);

  for (int i = 0; i < size; ++i) {
#if defined(__APPLE__)
    StringRef name(namelist[i]->d_name, namelist[i]->d_namlen);
#else
    StringRef name(namelist[i]->d_name);
#endif
    if (name.equals(".") || name.equals(".."))
      continue;

    Path path(name.str());
    free(namelist[i]);

    FileInfo file_info(path);
    pDir.entryList().push_back(file_info);
  }
  free(namelist);
}

void Directory::close(Directory& pDir)
{
  pDir.reset();
}

} // namespace onnc
