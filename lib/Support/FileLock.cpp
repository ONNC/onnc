//===- FileLock.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/FileLock.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// FileLock
//===----------------------------------------------------------------------===//
FileLock::FileLock()
  : m_FilePath(), m_FD(-1) {
}

FileLock::FileLock(const Path& pPath)
  : m_FilePath(pPath), m_FD(-1) {
}

FileLock::~FileLock()
{
}

bool FileLock::apply(const Path& pFileName)
{
  if (!m_FilePath.empty())
    return false;

  m_FilePath = pFileName;
  return true;
}

void FileLock::lock()
{
  if (m_FilePath.empty())
    return;

  while(true) {
    m_FD = open(m_FilePath.c_str(), O_CREAT, 0600);
    if (-1 == m_FD) //< open on failure
      break;

    ::flock(m_FD, LOCK_EX);

    if (!isDirty(m_FD)) //< possible be unlink by the other processes.
      break;

    close(m_FD);
  }
}

void FileLock::unlock()
{
  if (m_FilePath.empty())
    return;

  unlink(m_FilePath.c_str());
  flock(m_FD, LOCK_UN);
}

bool FileLock::isDirty(int pFD) const
{
  struct stat st0, st1;
  ::fstat(m_FD, &st0);
  ::stat(m_FilePath.c_str(), &st1);
  return (st0.st_ino != st1.st_ino);
}
