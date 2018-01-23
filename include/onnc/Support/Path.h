//===- Path.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This file declares the onnc::Path. It follows TR2/boost
// filesystem (v3), but modified to remove exception handling and RTTI.
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_PATH_H
#define ONNC_SUPPORT_PATH_H
#include <onnc/Support/DataTypes.h>
#include <onnc/ADT/TypeTag.h>
#include <iosfwd>
#include <string>

namespace onnc {

/** \class Path
 *  \brief Path provides an abstraction for the path to a file or directory in
 *   the operating system's filesystem.
 *
 *  Path is concerned only with the lexical and syntactic aspects of a path
 *  name. The path name does not necessarily exist in external storage, and
 *  path name is not necessarily valid for the current operating system or for
 *  a particular file system.
 *
 *  Native path name format is implementation-defined. Current storage type of
 *  native format is std::string.
 */
class Path
{
public:
  typedef char            ValueType;
  typedef std::string     StringType;

public:
  static const char       separator;
  static const char       preferred_separator;
  static const StringType separator_str;
  static const char       colon;
  static const char       dot;
  static const StringType dylib_extension;

public:
  /// empty path.
  Path();

  /// Stores the path name to the native format.
  Path(const ValueType* pPathName);

  /// Stores the path name to the native format.
  Path(const StringType &pPathName);

  /// Copy the native format from @ref pCopy.
  Path(const Path& pCopy);

  /// Fill constructor.
  /// Fills the string with @ref pN consecutive copies of character @ref pChar.
  Path(size_t pN, char pChar);

  /// Destructor
  virtual ~Path();

  /// Store the content [begin, end) in path name by native format.
  template <class InputIterator>
  Path& assign(InputIterator begin, InputIterator end);

  /// Assign \ref pPathName in native format.
  Path& assign(const Path& pPathName);

  /// Assign \ref pPathName in native format.
  Path& assign(const ValueType* s, unsigned int length);

  /// Append the content [begin, end) in the native format. The append
  /// operations also appends the platform's preferred dictionary
  /// separator when needed. The preferred dictionary separator is
  /// implementation defined. In Unix platforms, it's '/' in general.
  template <class InputIterator>
  Path& append(InputIterator begin, InputIterator end);

  /// Append the \ref pPathName in the native format. The append
  /// operations also appends the platform's preferred dictionary
  /// separator when needed. The preferred dictionary separator is
  /// implementation defined. In Unix platforms, it's '/' in general.
  Path& append(const Path& pPathName);

  /// Check if the native format is empty.
  bool empty() const;

  /// Clear the path.
  void clear() { m_PathName.clear(); }

  /// Check if the path is absolutely starting from root.
  bool isFromRoot() const;

  /// Check if the path is starting from the current working folder.
  bool isFromPWD() const;

  /// Returns the native format.
  const StringType& native() const { return m_PathName; }

  /// Returns the native format.
  StringType&       native()       { return m_PathName; }

  /// @retval true if the native string is equal to pString
  bool equals(const char* pString) const;

  /// @retval true if the native string is equal to pString
  bool equals(const std::string& pString) const
  { return equals(pString.c_str()); }

  /// Compares the value of the string object.
  /// @param [in] pString the target string.
  /// @return If they compare equal, then return 0;
  ///         If \ref pString is longer, then return > 0;
  ///         If \ref pString is shorter, then return < 0;
  int compare(const char* pString) const;

  /// Compares the value of the string object.
  /// @param [in] pString the target string.
  /// @return If they compare equal, then return 0;
  ///         If \ref pString is shorter, then return < 0;
  ///         If \ref pString is longer, then return > 0;
  int compare(const std::string& pString) const
  { return compare(pString.c_str()); }

  /// Compares the value of the string object.
  /// @param [in] pString the target string.
  /// @return If they compare equal, then return 0;
  ///         If \ref pString is shorter, then return < 0;
  ///         If \ref pString is longer, then return > 0;
  int compare(const Path& pString) const { return compare(pString.c_str()); }

  /// Returns the native format by C-style string.
  const ValueType* c_str() const { return m_PathName.c_str(); }

  unsigned int size() const { return m_PathName.size(); }

  /// @name Decomposition
  /// @{

  /// Returns the parent path. For example, if the path name is ./abc/XYZ/file,
  /// then its parent path is './abc/XYZ'; if the path name is ./abc/XYZ/, then
  /// its parent is './abc/XYZ'
  Path parent() const;

  /// Return the path of above folder. For example, if the path name is
  /// ./abc/XYZ/file, then its above folder is './abc/XYZ'; if the path name is
  /// ./abc/XYZ/, then its above folder is './abc'
  Path above() const;

  /// Returns the file name. For example, if the path name is ./abc/XYZ/file,
  /// then its file name is 'file'.
  Path filename() const;

  /// Returns the file name or directory portion of pathname. For example,
  /// if the path name is ./abc/XYZ/file, then its base name is 'file'.
  /// And if the path name is ./abc/XYZ/dir/, then its base name is 'dir'.
  Path basename() const;

  /// Return the extension of the file name. For example, if the path name is
  /// ./abc/XYZ/stem.extension, then its extension is 'extension'.
  /// @return the substring after the last dot.
  Path extension() const;

  /// Return the stem of the file name. For example, if the path name is
  /// ./abc/XYZ/stem.extension, then its stem is 'stem'.
  /// @return the substring between the last separator and extension.
  Path stem() const;
  /// @}

  /// Change the native format to generic form.
  /// A generic form indicates the following two conditions:
  ///   1. UNIX platform: generic form is equals to native form.
  ///   2. Windows platform: generic form will replace the `\\` with `/` in
  ///      the native form.
  StringType generic_string() const;

  /// Append paths
  Path& operator+=(const Path& pOther) { return append(pOther); }

  /// @retval true pOther contains substring of the path.
  bool inside(const Path& pOther) const;

protected:
  StringType::size_type AppendSeparatorAsNeeded();
  void EraseRedundantSeparator(StringType::size_type sep_pos);

protected:
  StringType m_PathName;
};

bool operator==(const Path& pLHS,const Path& pRHS);
bool operator!=(const Path& pLHS,const Path& pRHS);
Path operator+(const Path& pLHS, const Path& pRHS);

/// Checks whether @ref pPath exists or not.
bool exists(const Path &pPath);

/// Checks if @ref pPath is a directory.
bool is_directory(const Path &pPath);

/// Checks if @ref pPath is a regular file
bool is_regular(const Path& pPath);

/// Check if @ref pPath is a symbolic link
bool is_symlink(const Path& pPath);

/// Checks executive/search permission on @ref pPath.
bool is_executable(const Path& pPath);

/// Checks readable permission on @ref pPath.
bool is_readable(const Path& pPath);

/// Checks writable permission on @ref pPath.
bool is_writable(const Path& pPath);

//===----------------------------------------------------------------------===//
// System dependent non-member functions
//===----------------------------------------------------------------------===//
namespace sys {

/// Check if \ref pValue is the separator of the path in the system.
bool is_separator(char pValue);

} // namespace of sys

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
template <class Char, class Traits>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& pOS, const Path& pPath)
{
  return pOS << pPath.native();
}

template <class Char, class Traits>
inline std::basic_istream<Char, Traits>&
operator>>(std::basic_istream<Char, Traits>& pOS, Path& pPath)
{
  return pOS >> pPath.native();
}

//===----------------------------------------------------------------------===//
// class path member template implementation
//===----------------------------------------------------------------------===//
template <class InputIterator>
Path& Path::assign(InputIterator begin, InputIterator end)
{
  m_PathName.clear();
  if (begin != end)
    m_PathName.append<InputIterator>(begin, end);
  return *this;
}

template <class InputIterator>
Path& Path::append(InputIterator begin, InputIterator end)
{
  if (begin == end)
    return *this;
  StringType::size_type sep_pos(AppendSeparatorAsNeeded());
  m_PathName.append<InputIterator>(begin, end);
  if (sep_pos)
    EraseRedundantSeparator(sep_pos);
  return *this;
}

template<> TypeTag type_tag<Path>();

} // namespace of onnc

//===----------------------------------------------------------------------===//
// STL compatible functions
//===----------------------------------------------------------------------===//
namespace std {

/// Specification class of std::less<T> for @ref onnc::Path.
template<>
class less<onnc::Path> : public binary_function<onnc::Path,
                                                     onnc::Path,
                                                     bool>
{
public:
  bool operator() (const onnc::Path& pX,const onnc::Path& pY) const {
    if (pX.generic_string().size() < pY.generic_string().size())
      return true;
    return (pX.generic_string() < pY.generic_string());
  }
};

} // namespace of std

#endif
