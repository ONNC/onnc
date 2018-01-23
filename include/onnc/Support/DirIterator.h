//===- DirIterator.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_DIRECTORY_ITERATOR_H
#define ONNC_SUPPORT_DIRECTORY_ITERATOR_H
#include <iterator>
#include <cstddef>

namespace onnc {

class FileInfo;
class Directory;

/** \class onnc::DirIterator
 *  \brief traverse all entries in a @ref Directory.
 *
 *  @see Directory
 */
class DirIterator
{
public:
  typedef Directory value_type;
  typedef std::input_iterator_tag iterator_category;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

public:
  DirIterator();

  DirIterator(const DirIterator& pCopy);

  DirIterator& operator=(const DirIterator& pCopy);

  DirIterator& next();

  const FileInfo& fileInfo() const;

  const FileInfo& operator*() const { return fileInfo(); }

  bool operator==(const DirIterator& pOther) const;

  bool operator!=(const DirIterator& pOther) const;

  bool isValid() const { return (NULL != m_pParent); }

private:
  friend class Directory;

  DirIterator(const Directory& pParent, unsigned int pIdx);

private:
  const Directory* m_pParent;
  unsigned int m_Idx;
};

} // namespace of onnc

#endif
