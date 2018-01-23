//===- Pipe.inc -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <unistd.h>
#include <errno.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Pipe
//===----------------------------------------------------------------------===//
Pipe::Pipe()
  : m_FD { 0, 0 }, m_Error(SystemError::kNotStartedYet) {
  if (-1 == ::pipe(m_FD)) {
    m_Error = errno;
    onnc::error(error_pipe_cannot_open) << getpid() << m_Error;
  }
  else
    m_Error = SystemError::kSuccess;
}

FD& Pipe::operator[](Side pSide)
{
  assert(pSide < 2 && "Out of range");
  return m_FD[pSide];
}

FD Pipe::operator[](Side pSide) const
{
  assert(pSide < 2 && "Out of range");
  return m_FD[pSide];
}

//===----------------------------------------------------------------------===//
// Template implementation
//===----------------------------------------------------------------------===//
template<> FD& Pipe::side<Pipe::kBoth>()
{
  unreachable(unreach_pipe_access);
}

template<> FD Pipe::side<Pipe::kBoth>() const
{
  unreachable(unreach_pipe_access);
}

template<> void Pipe::close<Pipe::kBoth>()
{
  close<kRead>();
  close<kWrite>();
  m_Error = SystemError::kNotStartedYet;
}

template<> void Pipe::play<Pipe::kWrite>()
{
  close<kRead>();
}

template<> void Pipe::play<Pipe::kRead>()
{
  close<kWrite>();
}

template<> void Pipe::connect<Pipe::kBoth>(int pFD)
{
  unreachable(unreach_pipe_access);
}
