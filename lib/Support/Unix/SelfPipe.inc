//===- SelfPipe.inc -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <fcntl.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SelfPipe
//===----------------------------------------------------------------------===//
SelfPipe::SelfPipe()
  : Pipe() {

  /// Self-pipe should'nt be used in child process.
  /// set the pipe's file descriptors close-on-exec
  if (-1 == fcntl(m_FD[kRead], F_SETFL,
                  fcntl(m_FD[kRead], F_GETFL) | O_CLOEXEC)) {
    m_Error = errno;
    onnc::error(error_pipe_cannot_open) << getpid() << m_Error;
  }

  if (-1 == fcntl(m_FD[kWrite], F_SETFL,
                  fcntl(m_FD[kWrite], F_GETFL) | O_CLOEXEC)) {
    m_Error = errno;
    onnc::error(error_pipe_cannot_open) << getpid() << m_Error;
  }
}

SelfPipe::~SelfPipe()
{
  close<Pipe::kBoth>();
}

void SelfPipe::notify()
{
  int ret = -1;
  do {
    ret = ::write(side<Pipe::kWrite>(), "", 1);
  } while (-1 == ret && EINTR == errno);
}

void SelfPipe::wait()
{
  char c;
  int ret = -1;                                                                     
  do {
    ret = ::read(side<Pipe::kRead>(), &c, sizeof(c));
  } while (-1 == ret && EINTR == errno);  
}
