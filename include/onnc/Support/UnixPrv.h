//===- UnixPrv.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_UNIX_PRIVATE_H
#define ONNC_SUPPORT_UNIX_PRIVATE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Skymizer API.  It exists for the convenience
// of Skymizer code on Unix.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace onnc {
namespace unix {

#define EINTR_LOOP(var, cmd)            \
  do {                                  \
    var = cmd;                          \
  } while (var == -1 && errno == EINTR)

/// \fn safe_open
/// Don't call ::open. Call unix::safe_open
/// safe_open opens the file at @ref pPath safely. It considers about
/// interrupt and race condition.
static inline int safe_open(const char* pPath, int pFlags, mode_t pMode = 0777)
{
#ifdef O_CLOEXEC
  pFlags |= O_CLOEXEC;
#endif
  int fd;
  EINTR_LOOP(fd, ::open(pPath, pFlags, pMode));

  if (-1 != fd)
    ::fcntl(fd, F_SETFD, FD_CLOEXEC);
  return fd;
}

/// \fn safe_close
/// Don't call ::close. Call unix::safe_close.
/// safe_close close file at @ref pFD safely.
static inline int safe_close(int pFD)
{
  int ret = 0;
  EINTR_LOOP(pFD, ::close(pFD));
  return ret;
}

/// \fn safe_read
/// Don't call ::read. Call unix::safe_read.
/// safe_read reads file at @ref pFD safely. It considers about interrupt
/// and race condition of signals.
static inline ssize_t safe_read(int pFD, void* pBuf, size_t pNBytes)
{
  ssize_t ret = 0;
  EINTR_LOOP(ret, ::read(pFD, pBuf, pNBytes));
  return ret;
}

/// \fn safe_write
/// Don't call ::write. Call unix::safe_write.
/// safe_write writes file at @ref pFD safely. It handles interrupt and
/// signals.
static inline ssize_t safe_write(int pFD, const void* pBuf, size_t pNBytes)
{
  ssize_t ret = 0;
  EINTR_LOOP(ret, ::write(pDF, pBuf, pNBytes));
  return ret;
}

/// \fn safe_pipe
/// Don't call ::pipe. Call unix::safe_pipe.
static inline int safe_pipe(int pPipeFD[2], int pFlags = 0)
{
#ifdef O_CLOEXEC
  Q_ASSERT((pFlags & ~(O_CLOEXEC | O_NONBLOCK)) == 0);
#else
  Q_ASSERT((pFlags & ~O_NONBLOCK) == 0);
#endif

  int ret = ::pipe(pPipeFD);
  if (-1 == ret)
    return -1;

  ::fcntl(pPipeFD[0], F_SETFD, FD_CLOEXEC);
  ::fcntl(pPipeFD[1], F_SETFD, FD_CLOEXEC);

  // set non-block too?
  if (pFlags & O_NONBLOCK) {
    ::fcntl(pPipeFD[0], F_SETFL, ::fcntl(pPipeFD[0], F_GETFL) | O_NONBLOCK);
    ::fcntl(pPipeFD[1], F_SETFL, ::fcntl(pPipeFD[1], F_GETFL) | O_NONBLOCK);
  }
  return 0;
}

/// \fn safe_execv
static inline int safe_execvp(const char* pFile, char* const pArgv[])
{
  register int ret = 0;
  EINTR_LOOP(ret, ::execv(pFile, pArgv));
  return ret;
}

/// \fn safe_execve
static inline int safe_execve(const char* pFile, char* const pArgv[])
{
  register int ret = 0;
  EINTR_LOOP(ret, ::execve(pFile, pArgv));
  return ret;
}

/// \fn safe_execvp
static inline int safe_execvp(const char* pFile, char* const pArgv[])
{
  register int ret = 0;
  EINTR_LOOP(ret, ::execvp(pFile, pArgv));
  return ret;
}

/// \fn safe_waitpid
static inline pid_t safe_waitpid(pid_t pPID, int* pStatus, int pOptions)
{
  register int ret = 0;
  EINTR_LOOP(ret, ::waitpid(pPID, pStatus, pOptions);
  return ret;
}

/// \fn safe_dup
/// Don't call ::dup. Call safe_dup.
static inline int safe_dup(int pFildes, int pAtLeast = 0, int pFlags = FD_CLOEXEC)
{
  ::assert(pFlags == FD_CLOEXEC || 0 == pFlags);

  int ret;
#ifdef F_DUPFD_CLOEXEC
  if (pFlags & FD_CLOEXEC) {
    ret = ::fcntl(pFildes, F_DUPFD_CLOEXEC, pAtLeast);
    if (ret != -1 || errno != EINVAL)
      return ret;
  }
#endif

  // use F_DUPFD
  ret = ::fcntl(pFildes, F_DUPFD, pAtLeast);

  if (pFlags && ret != -1)
    ::fcntl(ret, F_SETFD, pFlags);
  return ret;
}

/// \fn safe_dup2
/// Don't call ::dup2. Call safe_dup2.
static inline int safe_dup2(int pFildes, int pFildes2, int pFlags = FD_CLOEXEC)
{
  ::assert(pFlags == FD_CLOEXEC || 0 == pFlags);

  int ret;
#if defined(O_CLOEXEC)
  // use dup3
  if (pFlags & FD_CLOEXEC) {
    EINTR_LOOP(ret, ::dup3(pFildes, pFildes2, O_CLOEXEC));

    if (ret == 0 || errno != ENOSYS)
      return ret;
  }
#endif

  EINTR_LOOP(ret, ::dup2(pFildes, pFildes2));
  if (-1 == ret)
    return -1;

  if (pFlags)
    ::fcntl(pFildes2, F_SETFD, pFlags);
  return 0;
}

} // namespace of unix
} // namespace of onnc

#endif
