//===- StringRef.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/DataTypes.h>
#include <cassert>
#include <cstring>
#include <bitset>
#include <climits>
#include <ostream>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
// MSVC emits references to this into the translation units which reference it.
#ifndef _MSC_VER
const StringRef::size_type StringRef::npos;
#endif

static char ascii_tolower(char x)
{
  if (x >= 'A' && x <= 'Z')
    return x - 'A' + 'a';
  return x;
}

static char ascii_toupper(char x)
{
  if (x >= 'a' && x <= 'z')
    return x - 'a' + 'A';
  return x;
}

// Workaround memcmp issue with null pointers (undefined behavior)
// by providing a specialized version
static int compareMemory(const char* pLhs, const char* pRhs, size_t pLength)
{
  if (0 == pLength)
    return 0;
  return ::memcmp(pLhs, pRhs, pLength);
}

// Return the radix and remove the prefix.
static unsigned int GetAutoSenseRadix(StringRef& pStr)
{
  if (pStr.startswith("0x")) {
    pStr = pStr.substr(2);
    return 16;
  }

  if (pStr.startswith("0b")) {
    pStr = pStr.substr(2);
    return 2;
  }

  if (pStr.startswith("0o")) {
    pStr = pStr.substr(2);
    return 8;
  }

  if (pStr.startswith("0"))
    return 8;

  return 10;
}

/// StrToULongLong - Workhorse method that converts a integer character
/// sequence of radix up to 36 to an unsigned long long value.
/// @param [in]     pStr    The converted string
/// @param [in]     pRadix  The radix
/// @param [in,out] pResult The result unsigned long long value
/// @return false only if any error occurs.
bool onnc::StrToULongLong(StringRef pStr, unsigned int pRadix,
                              unsigned long long& pResult)
{
  // Autosense radix if not specified.
  if (0 == pRadix)
    pRadix = GetAutoSenseRadix(pStr);

  // Empty strings (after the radix autosense) are invalid.
  if (pStr.empty())
    return false;

  // Parse all the bytes of the string given this radix.  Watch for overflow.
  pResult = 0;
  while (!pStr.empty()) {
    unsigned int char_value;
    if (pStr[0] >= '0' && pStr[0] <= '9')
      char_value = pStr[0]-'0';
    else if (pStr[0] >= 'a' && pStr[0] <= 'z')
      char_value = pStr[0]-'a'+10;
    else if (pStr[0] >= 'A' && pStr[0] <= 'Z')
      char_value = pStr[0]-'A'+10;
    else
      return false;

    // If the parsed value is larger than the integer radix, the string is
    // invalid.
    if (char_value >= pRadix)
      return false;

    // Add in this character.
    unsigned long long prev_result = pResult;
    pResult = pResult * pRadix + char_value;

    // Check for overflow by shifting back and seeing if bits were lost.
    if (pResult/pRadix < prev_result)
      return false;

    pStr = pStr.substr(1);
  }

  return true;
}

/// StrToLongLong - Workhorse method that converts a integer character
/// sequence of radix up to 36 to an long long value.
/// @param [in]     pStr    The converted string
/// @param [in]     pRadix  The radix
/// @param [in,out] pResult The result long long value
/// @return false only if any error occurs.
bool onnc::StrToLongLong(StringRef pStr, unsigned int pRadix,
                             long long& pResult)
{
  unsigned long long value = 0;

  // Handle positive strings first.
  if (pStr.empty() || pStr.front() != '-') {
    if (!StrToULongLong(pStr, pRadix, value))
      return false;
    // Check for value so large it overflows a signed value.
    if ((long long)value < 0)
      return false;
    pResult = value;
    return true;
  }

  // Get the positive part of the value.
  if (!StrToULongLong(pStr.substr(1), pRadix, value))
    return false;
  // Reject values so large they'd overflow as negative signed, but allow
  // "-0".  This negates the unsigned so that the negative isn't undefined
  // on signed overflow.
  if ((long long)-value > 0)
    return false;
  pResult = -value;
  return true;
}

//===----------------------------------------------------------------------===//
// StringRef
//===----------------------------------------------------------------------===//
/// Construct an empty string ref.
StringRef::StringRef()
  : m_Data(NULL), m_Length(0) {
}

/// Construct a string ref from a cstring.
StringRef::StringRef(const char *pStr)
  : m_Data(pStr) {
  if (NULL == m_Data)
    m_Length = 0;
  else
    m_Length = ::strlen(pStr); // invoking strlen(NULL) is undefined behavior
}

/// Construct a string ref from a pointer and length.
StringRef::StringRef(const char *pData, size_t pLength)
  : m_Data(pData), m_Length(pLength) {
  assert((pData || pLength == 0) &&
  "StringRef cannot be built from a NULL argument with non-zero length");
}

/// Construct a string ref from an std::string.
StringRef::StringRef(const std::string &pStr)
  : m_Data(pStr.data()), m_Length(pStr.length()) {
}

/// Copy constructor
StringRef::StringRef(const StringRef& pCopy)
  : m_Data(pCopy.m_Data), m_Length(pCopy.m_Length) {
}

/// Assignment
StringRef& StringRef::operator=(const StringRef& pCopy)
{
  m_Data = pCopy.m_Data;
  m_Length = pCopy.m_Length;
  return *this;
}

/// Destruct the string reference.
StringRef::~StringRef()
{
  m_Data = NULL;
  m_Length = 0;
}

/// front - Get the first character in the string.
char StringRef::front() const
{
  assert(!empty());
  return m_Data[0];
}

/// back - Get the last character in the string.
char StringRef::back() const
{
  assert(!empty());
  return m_Data[m_Length-1];
}

/// equals - Check for string equality, this is more efficient than
/// compare() when the relative ordering of inequal strings isn't needed.
bool StringRef::equals(StringRef pRHS) const
{
  return (m_Length == pRHS.m_Length &&
          0 == compareMemory(m_Data, pRHS.m_Data, pRHS.m_Length));
}

/// equals_lower - Check for string equality, ignoring case.
bool StringRef::equals_lower(const StringRef& pRHS) const
{
  return (m_Length == pRHS.m_Length && compare_lower(pRHS) == 0);
}

/// compare - Compare two strings; the result is -1, 0, or 1 if this string
/// is lexicographically less than, equal to, or greater than the \p RHS.
int StringRef::compare(StringRef pRHS) const
{
  // Check the prefix for a mismatch.
  if (int res = compareMemory(m_Data, pRHS.m_Data,
                              min(m_Length, pRHS.m_Length)))
    return res < 0 ? -1 : 1;

  // Otherwise the prefixes match, so we only need to check the lengths.
  if (m_Length == pRHS.m_Length)
    return 0;
  return m_Length < pRHS.m_Length ? -1 : 1;
}

/// compare_lower - Compare strings, ignoring case.
int StringRef::compare_lower(const StringRef& pRHS) const
{
  for (size_type i = 0, e = min(m_Length, pRHS.m_Length); i != e; ++i) {
    unsigned char lhc = ascii_tolower(m_Data[i]);
    unsigned char rhc = ascii_tolower(pRHS.m_Data[i]);
    if (lhc != rhc)
      return lhc < rhc ? -1 : 1;
  }

  if (m_Length == pRHS.m_Length)
    return 0;
  return m_Length < pRHS.m_Length ? -1 : 1;
}

/// operator[](index)
char StringRef::operator[](size_t pIdx) const
{
  return m_Data[pIdx];
}

char StringRef::at(size_t pIdx) const
{
  assert(pIdx < m_Length && "Invalid index!");
  return m_Data[pIdx];
}

/// Check if this string starts with the given \p Prefix.
bool StringRef::startswith(StringRef pPrefix) const
{
  return (m_Length >= pPrefix.length() &&
         0 == compareMemory(m_Data, pPrefix.data(), pPrefix.length()));
}

/// Check if this string ends with the given \p Suffix.
bool StringRef::endswith(StringRef pSuffix) const
{
  return (m_Length >= pSuffix.length() &&
         0 == compareMemory(end() - pSuffix.length(),
                            pSuffix.data(),
                            pSuffix.length()));
}

//===----------------------------------------------------------------------===//
// StringRef find series
//===----------------------------------------------------------------------===//
/// Search for the first character \p pC in the string.
StringRef::size_type StringRef::find(char pC, size_type pFrom) const
{
  for (size_type i = min(pFrom, m_Length), e = m_Length; i != e; ++i)
    if (m_Data[i] == pC)
      return i;
  return npos;
}

/// Search for the first string \p Str in the string.
StringRef::size_type
StringRef::find(const StringRef& pStr, size_type pFrom) const
{
  size_type substr_len = pStr.size();
  if (substr_len > m_Length)
    return npos;

  // For short haystacks or unsupported needles fall back to the naive algorithm
  if (m_Length < 16 || substr_len > 255 || substr_len == 0) {
    for (size_type e = m_Length - substr_len + 1, i = min(pFrom, e); i != e; ++i)
      if (substr(i, substr_len).equals(pStr))
        return i;
    return npos;
  }

  if (pFrom >= m_Length)
    return npos;

  // Build the bad char heuristic table, with uint8_t to reduce cache thrashing.
  uint8_t bad_char_skip[256];
  std::memset(bad_char_skip, substr_len, 256);
  for (unsigned i = 0; i != substr_len-1; ++i)
    bad_char_skip[(uint8_t)pStr[i]] = substr_len-1-i;

  unsigned len = m_Length-pFrom, pos = pFrom;
  while (len >= substr_len) {
    if (substr(pos, substr_len).equals(pStr)) // See if this is the correct substring.
      return pos;

    // Otherwise skip the appropriate number of bytes.
    uint8_t skip = bad_char_skip[(uint8_t)(*this)[pos+substr_len-1]];
    len -= skip;
    pos += skip;
  }

  return npos;
}

/// Search for the last character \p pC in the string.
StringRef::size_type StringRef::rfind(char pC, size_type pFrom) const
{
  pFrom = min(pFrom, m_Length);
  size_t i = pFrom;
  while (0 != i) {
    --i;
    if (m_Data[i] == pC)
      return i;
  }
  return npos;
}

/// Search for the last string \p pStr in the string.
StringRef::size_type StringRef::rfind(const StringRef& pStr) const
{
  size_t str_size = pStr.size();
  if (str_size > m_Length)
    return npos;
  for (size_type i = m_Length - str_size + 1, e = 0; i != e;) {
    --i;
    if (substr(i, str_size).equals(pStr))
      return i;
  }
  return npos;
}

/// Find the first character in the string that is \p pC, or npos if not found.
/// Same as find.
StringRef::size_type StringRef::find_first_of(char pC, size_type pFrom) const
{
  return find(pC, pFrom);
}

/// Find the first character in the string that is in \p pSubstr, or npos if
/// not found.
StringRef::size_type
StringRef::find_first_of(const StringRef& pSubstr, size_type pFrom) const
{
  std::bitset<1 << CHAR_BIT> char_bits;
  for (size_type i = 0; i != pSubstr.size(); ++i)
    char_bits.set((unsigned char)pSubstr[i]);

  for (size_type i = min(pFrom, m_Length), e = m_Length; i != e; ++i)
    if (char_bits.test((unsigned char)m_Data[i]))
      return i;
  return npos;
}

/// Find the first character in the string that is not \p pC or npos if not
/// found.
StringRef::size_type
StringRef::find_first_not_of(char pC, size_type pFrom) const
{
  for (size_type i = min(pFrom, m_Length), e = m_Length; i != e; ++i)
    if (m_Data[i] != pC)
      return i;
  return npos;
}

/// Find the first character in the string that is not in the string
/// \p pSubstrs, or npos if not found.
StringRef::size_type
StringRef::find_first_not_of(const StringRef& pSubstrs, size_type pFrom) const
{
  std::bitset<1 << CHAR_BIT> char_bits;
  for (size_type i = 0; i != pSubstrs.size(); ++i)
    char_bits.set((unsigned char)pSubstrs[i]);

  for (size_type i = min(pFrom, m_Length), e = m_Length; i != e; ++i)
    if (!char_bits.test((unsigned char)m_Data[i]))
      return i;
  return npos;
}

/// Find the last character in the string that is \p pC, or npos if not found.
StringRef::size_type StringRef::find_last_of(char pC, size_type pFrom) const
{
  return rfind(pC, pFrom);
}

/// Find the last character in the string that is \p C, or npos if not found.
StringRef::size_type
StringRef::find_last_of(const StringRef& pSubstr, size_type pFrom) const
{
  std::bitset<1 << CHAR_BIT> char_bits;
  for (size_type i = 0; i != pSubstr.size(); ++i)
    char_bits.set((unsigned char)pSubstr[i]);

  for (size_type i = min(pFrom, m_Length) - 1, e = -1; i != e; --i)
    if (char_bits.test((unsigned char)m_Data[i]))
      return i;
  return npos;
}

/// Find the last character in the string that is not \p C, or npos if not
/// found.
StringRef::size_type
StringRef::find_last_not_of(char pC, size_type pFrom) const
{
  for (size_type i = min(pFrom, m_Length) - 1, e = -1; i != e; --i)
    if (m_Data[i] != pC)
      return i;
  return npos;
}

/// Find the last character in the string that is not in \p Chars, or
/// npos if not found.
StringRef::size_type
StringRef::find_last_not_of(const StringRef& pSubstr, size_type pFrom) const
{
  std::bitset<1 << CHAR_BIT> char_bits;
  for (size_type i = 0, e = pSubstr.size(); i != e; ++i)
    char_bits.set((unsigned char)pSubstr[i]);

  for (size_type i = min(pFrom, m_Length) - 1, e = -1; i != e; --i)
    if (!char_bits.test((unsigned char)m_Data[i]))
      return i;
  return npos;
}

/// Return the number of occurrences of \p pC in the string.
size_t StringRef::count(char pC) const
{
  size_t result = 0;
  for (size_t i = 0, e = m_Length; i != e; ++i)
    if (m_Data[i] == pC)
      ++result;
  return result;
}

/// Return the number of non-overlapped occurrences of \p pStr in
/// the string.
size_t StringRef::count(const StringRef& pStr) const
{
  size_t result = 0;
  size_t length = pStr.size();
  if (length > m_Length)
    return 0;
  for (size_t i = 0, e = m_Length - length + 1; i != e; ++i)
    if (substr(i, length).equals(pStr))
      ++result;
  return result;
}

/// Convert the given ASCII string to lowercase.
std::string StringRef::lower() const
{
  std::string result(size(), char());
  for (size_type i = 0, e = size(); i != e; ++i) {
    result[i] = ascii_tolower(m_Data[i]);
  }
  return result;
}

/// Convert the given ASCII string to uppercase.
std::string StringRef::upper() const
{
  std::string result(size(), char());
  for (size_type i = 0, e = size(); i != e; ++i) {
    result[i] = ascii_toupper(m_Data[i]);
  }
  return result;
}

void StringRef::toStr(std::ostream& pOS) const
{
  if (!isValid())
    pOS << "(null)";
  else {
    pOS << str();
  }
}

/// Return a reference to the substring from [Start, Start + N).
StringRef StringRef::substr(size_t pStart, size_t pN) const
{
  size_t start = min(pStart, m_Length);
  return StringRef(m_Data + start, min(pN, m_Length - start));
}

/// Return a StringRef equal to 'this' but with the first \p N elements dropped.
StringRef StringRef::drop_front(size_t pN) const
{
  assert(size() >= pN && "Dropping more elements than exist");
  return substr(pN);
}

/// Return a StringRef equal to 'this' but with the last \p N elements dropped.
StringRef StringRef::drop_back(size_t pN) const
{
  assert(size() >= pN && "Dropping more elements than exist");
  return substr(0, size() - pN);
}

StringRef StringRef::slice(size_type pStart, size_type pEnd) const
{
  size_t start = min(pStart, m_Length);
  size_t end = min(max(start, pEnd), m_Length);
  return StringRef(m_Data + start, end - start);
}

std::pair<StringRef, StringRef> StringRef::split(char pSeparator) const
{
  size_t idx = find(pSeparator);
  if (idx == npos)
    return std::make_pair(*this, StringRef());
  return std::make_pair(slice(0, idx), slice(idx+1, npos));
}

std::pair<StringRef, StringRef> StringRef::split(StringRef pSeparator) const
{ 
  size_t idx = find(pSeparator);
  if (idx == npos)
    return std::make_pair(*this, StringRef());
  return std::make_pair(slice(0, idx), slice(idx + pSeparator.size(), npos));
}

void StringRef::split(std::vector<StringRef> &pA, StringRef pSeparator,
                      int pMaxSplit, bool pKeepEmpty) const
{
  StringRef S = *this;

  // Count down from pMaxSplit. When pMaxSplit is -1, this will just split
  // "forever". This doesn't support splitting more than 2^31 times
  // intentionally; if we ever want that we can make pMaxSplit a 64-bit integer
  // but that seems unlikely to be useful.
  while (pMaxSplit-- != 0) {
    size_t Idx = S.find(pSeparator);
    if (Idx == npos)
      break;

    // Push this split.
    if (pKeepEmpty || Idx > 0)
      pA.push_back(S.slice(0, Idx));

    // Jump forward.
    S = S.slice(Idx + pSeparator.size(), npos);
  }

  // Push the tail.
  if (pKeepEmpty || !S.empty())
    pA.push_back(S);
}

void StringRef::split(std::vector<StringRef> &pA, char pSeparator,
                      int pMaxSplit, bool pKeepEmpty) const
{
  StringRef S = *this;

  // Count down from pMaxSplit. When pMaxSplit is -1, this will just split
  // "forever". This doesn't support splitting more than 2^31 times
  // intentionally; if we ever want that we can make pMaxSplit a 64-bit integer
  // but that seems unlikely to be useful.
  while (pMaxSplit-- != 0) {
    size_t Idx = S.find(pSeparator);
    if (Idx == npos)
      break;

    // Push this split.
    if (pKeepEmpty || Idx > 0)
      pA.push_back(S.slice(0, Idx));

    // Jump forward.
    S = S.slice(Idx + 1, npos);
  }

  // Push the tail.
  if (pKeepEmpty || !S.empty())
    pA.push_back(S);
}

template<> onnc::TypeTag onnc::type_tag<StringRef>()
{
  static bool instance;
  return &instance;
}
