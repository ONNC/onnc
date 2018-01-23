//===- FileInfo.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/FileInfo.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// FileInfo
//===----------------------------------------------------------------------===//
FileInfo::FileInfo()
  : m_Path(), m_Status() {
}

FileInfo::FileInfo(const Path& pPath)
  : m_Path(pPath), m_Status() {
  onnc::status(m_Path, m_Status);
}

FileInfo::FileInfo(const FileInfo& pCopy)
  : m_Path(pCopy.m_Path), m_Status(pCopy.m_Status) {
}

FileInfo& FileInfo::operator=(const FileInfo& pCopy)
{
  m_Path = pCopy.m_Path;
  m_Status = pCopy.m_Status;
  return *this;
}

FileInfo& FileInfo::assign(const Path& pPath)
{
  m_Path = pPath;
  onnc::status(m_Path, m_Status);
  return *this;
}
