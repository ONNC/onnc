//===- DirIterator.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/DirIterator.h>
#include <onnc/Support/Directory.h>
#include <onnc/Support/FileInfo.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// DirIterator
//===----------------------------------------------------------------------===//
DirIterator::DirIterator(const Directory& pParent, unsigned int pIdx)
  : m_pParent(&pParent), m_Idx(pIdx) {
}

DirIterator::DirIterator()
  : m_pParent(nullptr), m_Idx(0) {
}

DirIterator::DirIterator(const DirIterator& pCopy)
  : m_pParent(pCopy.m_pParent), m_Idx(pCopy.m_Idx) {
}

DirIterator& DirIterator::operator=(const DirIterator& pCopy)
{
  m_pParent = pCopy.m_pParent;
  m_Idx = pCopy.m_Idx;
  return *this;
}

const FileInfo&	DirIterator::fileInfo() const
{
  if (nullptr == m_pParent)
    fatal(fatal_invalid_iterator);

  if (m_Idx > m_pParent->size())
    fatal(fatal_out_of_range) << m_Idx << m_pParent->size();

  return m_pParent->entryList()[m_Idx];
}

DirIterator& DirIterator::next()
{
  ++m_Idx;
  return *this;
}

bool DirIterator::operator==(const DirIterator& pOther) const
{
  return (m_pParent == pOther.m_pParent && m_Idx == pOther.m_Idx);
}

bool DirIterator::operator!=(const DirIterator& pOther) const
{
  return !(this->operator==(pOther));
}
