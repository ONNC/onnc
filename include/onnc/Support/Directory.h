//===- Directory.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_DIRECTORY_H
#define ONNC_SUPPORT_DIRECTORY_H
#include <onnc/Support/FileInfo.h>
#include <onnc/Support/DirIterator.h>
#include <onnc/Support/ErrorCode.h>
#include <cstddef>

namespace onnc {

/** \class onnc::Directory
 *  \brief abstract file system's directory.
 */
class Directory
{
public:
  typedef DirIterator const_iterator;

public:
  Directory();

  /// Open @ref pPath as a directory. The error status is set on status().
  Directory(const Path& pPath);

  Directory(const Directory& pCopy);

  Directory& operator=(const Directory& pCopy);

  virtual ~Directory();

  /// Open @ref pPath as a directory. The error status is set on status().
  SystemError open(const Path& pPath);

  /// Close the directory and reset the object.
  void close();

  /// @retval true If it succeeds to open directory handler.
  bool isGood() const;

  /// The status of opening.
  SystemError status() const { return m_Status; }

  const Path& path() const { return m_Path; }

  const_iterator begin() const;

  const_iterator end()   const;

  unsigned int size() const { return m_Size; }

  bool empty() const { return (0 == m_Size); }

  const FileInfoList& entryList() const { return m_EntryList; }

private:
  friend class DirIterator;

  void setStatus(SystemError pErr) { m_Status = pErr; }

  void reset();

  void setSize(unsigned int pN) { m_Size = pN; }

  FileInfoList& entryList() { return m_EntryList; }

  /// Platform-dependent open
  static void open(Directory& pDir);

  /// Platform-dependent close
  static void close(Directory& pDir);

protected:
  Path m_Path;
  SystemError m_Status;
  unsigned int m_Size;
  FileInfoList m_EntryList;
};

} // namespace of onnc

#endif
