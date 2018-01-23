//===- Directory.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Directory.h>
#include <onnc/Config/Config.h>

// Include the truly platform-specific parts. 
#if defined(ONNC_ON_UNIX)
#include "Unix/Directory.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/Directory.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/Directory.inc"
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// Directory
//===----------------------------------------------------------------------===//
Directory::Directory()
  : m_Status(SystemError::kNotStartedYet), m_Size(0) {
}

Directory::Directory(const Path& pPath)
  : m_Path(pPath), m_Status(SystemError::kNotStartedYet), m_Size(0) {
  open(*this);
}

Directory::Directory(const Directory& pCopy)
  : m_Path(pCopy.m_Path),
    m_Status(pCopy.m_Status),
    m_Size(pCopy.m_Size),
    m_EntryList(pCopy.m_EntryList) {
}

Directory::~Directory()
{
  close();
}

SystemError Directory::open(const Path& pPath)
{
  this->reset();

  m_Path = pPath;
  open(*this);
  return m_Status;
}

void Directory::close()
{
  close(*this);
}

bool Directory::isGood() const
{
  return (status() == SystemError::kSuccess);
}

Directory::const_iterator Directory::begin() const
{
  if (!isGood())
    return DirIterator(*this, m_EntryList.size());
  return DirIterator(*this, 0);
}

Directory::const_iterator Directory::end() const
{
  return DirIterator(*this, m_EntryList.size());
}

void Directory::reset()
{
  m_Path.clear();
  m_Status = SystemError::kNotStartedYet;
  m_Size = 0;
  m_EntryList.clear();
}
