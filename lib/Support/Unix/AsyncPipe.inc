//===- AsyncPipe.inc ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// AsyncPipe
//===----------------------------------------------------------------------===//
AsyncPipe::AsyncPipe()
  : Pipe() {
  if (isGood()) {
    int flags = ::fcntl(m_FD[0], F_GETFL, 0);
    ::fcntl(m_FD[0], F_SETFL, flags | O_NONBLOCK);

    flags = ::fcntl(m_FD[1], F_GETFL, 0);
    ::fcntl(m_FD[1], F_SETFL, flags | O_NONBLOCK);
  }
}
