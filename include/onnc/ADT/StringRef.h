//===- StringRef.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_STRINGREF_H
#define ONNC_ADT_STRINGREF_H
#include <iosfwd>
#include <onnc/ADT/TypeTag.h>
#include <string>
#include <vector>

namespace onnc {

/** \class StringRef
 *  \brief StringRef represents a constant reference to a string, i.e.
 *  a character array and a length, which need not be null terminated.
 *
 *  This class does not own the string data, it is expected to be used in
 *  situations where the character data resides in some other buffer, whose
 *  lifetime extends past that of the StringRef. For this reason, it is not in
 *  general safe to store a StringRef.
 *
 *  StringRef may refer to a NULL pointer, an empty string ('\0') or a zero-
 *  terminated string. If a StringRef object refers to a NULL pointer, then
 *  empty() return true but isValid() return false. In another case, if a
 *  StringRef object refers to an empty string, then both empty() and
 *  isValid() return true.
 *
 *  @note Don't modify the string data by StringRef.
 */
class StringRef
{
public:
  typedef const char* iterator;
  typedef const char* const_iterator;
  typedef size_t size_type;
  static const size_type npos = ~size_type(0);

public:
  /// Construct an empty string ref.
  StringRef();

  /// Construct a string ref from a cstring.
  /// @param pStr C-style string. Assertion occurs when pStr is a NULL string.
  StringRef(const char* pStr);

  /// Construct a string ref from a pointer and length.
  /// @param pData a memory string (which may not be NULL terminated). Assertion
  /// occurs when pStr is a NULL strin or pLength is zero.
  StringRef(const char *pData, size_t pLength);

  /// Construct a string ref from an std::string.
  StringRef(const std::string &pStr);

  /// Copy constructor
  StringRef(const StringRef& pCopy);

  /// Assignment
  StringRef& operator=(const StringRef& pCopy);

  /// Destructor - detach the string
  ~StringRef();

  const_iterator begin() const { return m_Data; }
  iterator       begin()       { return m_Data; }
  const_iterator end()   const { return m_Data + m_Length; }
  iterator       end()         { return m_Data + m_Length; }

  /// data - Get a pointer to the start of the string (which may not be null
  /// terminated).
  const char* data() const { return m_Data; }

  /// Return C-style string
  /// XXX: since m_Data may not be null terminated, the returned c string
  /// is not necessarily the same as the real content of this StringRef.
  const char* c_str() const { return m_Data; }

  /// empty - Check if the string is empty.
  bool empty() const { return (0 == m_Length); }

  /// Does the data exist?
  bool isValid() const { return (NULL != m_Data); }

  /// size - Get the length of string
  size_t size() const { return m_Length; }

  /// length - Get the length of string
  size_t length() const { return m_Length; }

  /// front - Get the first character in the string.
  /// @note Don't modify string data by front().
  char front() const;

  /// back - Get the last character in the string.
  /// @note Don't modify string data by end().
  char back() const;

  /// Check for string equality, this is more efficient than
  /// compare() when the relative ordering of inequal strings isn't needed.
  bool equals(StringRef pRHS) const;

  /// compare - Compare two strings; the result is -1, 0, or 1 if this string
  /// is lexicographically less than, equal to, or greater than the \p RHS.
  int compare(StringRef pRHS) const;

  /// equals_lower - Check for string equality, ignoring case.
  bool equals_lower(const StringRef& pRHS) const;

  /// compare_lower - Compare two strings, ignoring case.
  int compare_lower(const StringRef& RHS) const;

  size_t count(char pC) const;
  size_t count(const StringRef& pStr) const;

  // Convert the given ASCII string to lowercase.
  std::string lower() const;

  /// Convert the given ASCII string to uppercase.
  std::string upper() const;

  /// str - Get the contents as an std::string.
  std::string str() const {
    if (NULL == m_Data) return std::string();
    return std::string(m_Data, m_Length);
  }

  operator std::string() const { return str(); }

  void toStr(std::ostream& pOS) const;

  // Return the charactor at position @ref pIndex.
  char operator[](size_t pIndex) const;

  // Return the charactor at position @ref pIndex with checking out of range.
  char at(size_t pIndex) const;

  /// Check if this string starts with the given \p Prefix.
  bool startswith(StringRef pPrefix) const;

  /// Check if this string ends with the given \p Suffix.
  bool endswith(StringRef pSuffix) const;

  /// @name String searching
  /// @{
  /// searches for the first charactor @ref pC in the string
  size_type find(char pC, size_type pFrom = 0) const;

  /// searches for the first sub-string @ref pStr in the string.
  size_type find(const StringRef& pStr, size_type pFrom = 0) const;

  size_type rfind(char pC, size_type pFrom = npos) const;
  size_type rfind(const StringRef& Str) const;
  size_type find_first_of(char pC, size_type pFrom = 0) const;
  size_type find_first_of(const StringRef& pSubstr, size_type pFrom = 0) const;
  size_type find_first_not_of(char pC, size_type pFrom = 0) const;
  size_type find_first_not_of(const StringRef& pSubstrs, size_type pFrom = 0) const;
  size_type find_last_of(char pC, size_type pFrom = npos) const;
  size_type find_last_of(const StringRef& pSubstr, size_type pFrom = npos) const;
  size_type find_last_not_of(char pC, size_type pFrom = npos) const;
  size_type find_last_not_of(const StringRef& pSubstr, size_type pFrom = npos) const;

  /// @}
  /// @name Sub-string Operation
  /// @{

  /// Return a reference to the substring from [Start, Start + N).
  StringRef substr(size_t pStart, size_t pN = npos) const;

  /// Return a StringRef equal to 'this' but with the first \p N elements
  /// dropped.
  StringRef drop_front(size_t pN = 1) const;

  /// Return a StringRef equal to 'this' but with the last \p N elements
  /// dropped.
  StringRef drop_back(size_t pN = 1) const;

  /// slice - Return a reference to the substring from [Start, End).
  ///
  /// \param Start The index of the starting character in the substring; if
  /// the index is npos or greater than the length of the string then the
  /// empty substring will be returned.
  ///
  /// \param End The index following the last character to include in the
  /// substring. If this is npos, or less than \p Start, or exceeds the
  /// number of characters remaining in the string, the string suffix
  /// (starting with \p Start) will be returned.
  StringRef slice(size_type pStart, size_type pEnd) const;

  /// Split into two substrings around the first occurrence of a separator
  /// character.
  ///
  /// If \p pSeparator is in the string, then the result is a pair (LHS, RHS)
  /// such that (*this == LHS + Separator + RHS) is true and RHS is
  /// maximal. If \p pSeparator is not in the string, then the result is a
  /// pair (LHS, RHS) where (*this == LHS) and (RHS == "").
  ///
  /// \param Separator The character to split on.
  /// \returns The split substrings.
  std::pair<StringRef, StringRef> split(char pSeparator) const;
  std::pair<StringRef, StringRef> split(StringRef pSeparator) const;

  /// Split into substrings around the occurrences of a separator string.
  ///
  /// Each substring is stored in \p pA. If \p pMaxSplit is >= 0, at most
  /// \p pMaxSplit splits are done and consequently <= \p pMaxSplit + 1
  /// elements are added to A.
  /// If \p pKeepEmpty is false, empty strings are not added to \p pA. They
  /// still count when considering \p pMaxSplit
  /// An useful invariant is that
  /// pSeparator.join(A) == *this if pMaxSplit == -1 and pKeepEmpty == true
  ///
  /// \param pA - Where to put the substrings.
  /// \param pSeparator - The string to split on.
  /// \param pMaxSplit - The maximum number of times the string is split.
  /// \param pKeepEmpty - True if empty substring should be added.
  void split(std::vector<StringRef> &pA, StringRef pSeparator,
             int pMaxSplit = -1, bool pKeepEmpty = true) const;

  /// Split into substrings around the occurrences of a separator character.
  ///
  /// Each substring is stored in \p pA. If \p pMaxSplit is >= 0, at most
  /// \p pMaxSplit splits are done and consequently <= \p pMaxSplit + 1
  /// elements are added to A.
  /// If \p pKeepEmpty is false, empty strings are not added to \p pA. They
  /// still count when considering \p pMaxSplit
  /// An useful invariant is that
  /// pSeparator.join(A) == *this if pMaxSplit == -1 and pKeepEmpty == true
  ///
  /// \param pA - Where to put the substrings.
  /// \param pSeparator - The string to split on.
  /// \param pMaxSplit - The maximum number of times the string is split.
  /// \param pKeepEmpty - True if empty substring should be added.
  void split(std::vector<StringRef> &pA, char pSeparator, int pMaxSplit = -1,
             bool pKeepEmpty = true) const;

  /// Return string with consecutive characters in \p pChars starting from
  /// the left and right removed.
  StringRef trim(StringRef pChars = " \t\n\v\f\r") const {
    return ltrim(pChars).rtrim(pChars);
  }

  /// @}

private:
  // Workaround PR5482: nearly all gcc 4.x miscompile StringRef and std::min()
  // Changing the arg of min to be an integer, instead of a reference to an
  // integer works around this bug.
  static size_t min(size_t a, size_t b) { return a < b ? a : b; }
  static size_t max(size_t a, size_t b) { return a > b ? a : b; }

  /// Return string with consecutive characters in \p pChars starting from
  /// the left removed.
  StringRef ltrim(StringRef pChars = " \t\n\v\f\r") const {
    return drop_front(std::min(m_Length, find_first_not_of(pChars)));
  }

  /// Return string with consecutive characters in \p pChars starting from
  /// the right removed.
  StringRef rtrim(StringRef pChars = " \t\n\v\f\r") const {
    return drop_back(m_Length - std::min(m_Length, find_last_not_of(pChars) + 1));
  }

private:
  /// The start of the string, in an external buffer.
  const char* m_Data;

  /// The length of the string.
  size_t m_Length;
  /// @}
};

/// @name StringRef Comparison Operators
/// @{

inline bool operator==(StringRef pLHS, StringRef pRHS) {
  return pLHS.equals(pRHS);
}

inline bool operator!=(StringRef pLHS, StringRef pRHS) {
  return !(pLHS == pRHS);
}

inline bool operator<(StringRef pLHS, StringRef pRHS) {
  return (-1 == pLHS.compare(pRHS));
}

inline bool operator<=(StringRef pLHS, StringRef pRHS) {
  return (1 != pLHS.compare(pRHS));
}

inline bool operator>(StringRef pLHS, StringRef pRHS) {
  return (1 == pLHS.compare(pRHS));
}

inline bool operator>=(StringRef pLHS, StringRef pRHS) {
  return (-1 != pLHS.compare(pRHS));
}

inline std::string &operator+=(std::string &pBuffer, StringRef pString) {
  return pBuffer.append(pString.data(), pString.size());
}

/// @}

/// StrToULongLong - Workhorse method that converts a integer character
/// sequence of radix up to 36 to an unsigned long long value.
/// @param [in]     pStr    The converted string
/// @param [in]     pRadix  The radix
/// @param [in,out] pResult The result unsigned long long value
/// @return false only if any error occurs.
bool StrToULongLong(StringRef pStr, unsigned int pRadix,
                    unsigned long long& pResult);

/// StrToLongLong - Workhorse method that converts a integer character
/// sequence of radix up to 36 to an long long value.
/// @param [in]     pStr    The converted string
/// @param [in]     pRadix  The radix
/// @param [in,out] pResult The result long long value
/// @return false only if any error occurs.
bool StrToLongLong(StringRef pStr, unsigned int pRadix,
                   long long& pResult);

template<> TypeTag type_tag<StringRef>();

} // namespace onnc

namespace std {

inline ostream& operator<<(ostream& pOS, onnc::StringRef pStr)
{
  pStr.toStr(pOS);
  return pOS;
}

inline string to_string(onnc::StringRef pValue)
{
  return pValue.str();
}

} // namespace std

#endif
