//===- Diagnostic.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/Diagnostic.h>
#include <onnc/Diagnostic/DiagnosticInfoMap.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/OStrStream.h>
#include <ctype.h>
#include <algorithm>

using namespace onnc;

/// findMatch - Scans string forward, looking for the given charactor, skipping
/// nested clauses and escaped characters.
static const char* findMatch(char pVal, const char* pBegin, const char* pEnd )
{
  unsigned int depth = 0;
  for (; pBegin != pEnd; ++pBegin) {
    if (0 == depth && *pBegin == pVal)
      return pBegin;
    if (0 != depth && *pBegin == '}')
      --depth;

    if ('%' == *pBegin) {
      ++pBegin;
      if (pBegin == pEnd)
        break;

      if (!isdigit(*pBegin) && !ispunct(*pBegin)) {
        ++pBegin;
        while (pBegin != pEnd && !isdigit(*pBegin) && *pBegin != '{')
          ++pBegin;

        if (pBegin == pEnd)
          break;
        if ('{' == *pBegin)
          ++depth;
      }
    }
  } // end of for
  return pEnd;
}

//===----------------------------------------------------------------------===//
// Diagnostic
//===----------------------------------------------------------------------===//
diagnostic::Diagnostic::Diagnostic(State& pState, const InfoMap& pInfoMap)
  : m_State(pState), m_InfoMap(pInfoMap), m_Severity(pState.CurrentSeverity) {
}

diagnostic::Diagnostic::~Diagnostic()
{
  m_State.reset();
}

/// format - format this diagnostic into string.
void diagnostic::Diagnostic::format(std::string& pOutStr) const
{
  StringRef desc = m_State.Format;
  format(desc.begin(), desc.end(), pOutStr);
}

/// format - format this diagnostic into string.
void diagnostic::Diagnostic::format(const char* pBegin, const char* pEnd,
                                    std::string& pOutStr) const
{
  const char* cur_char = pBegin;
  while (cur_char != pEnd) {
    if ('%' != *cur_char) {
      const char* new_end = std::find(cur_char, pEnd, '%');
      pOutStr.append(cur_char, new_end);
      cur_char = new_end;
      continue;
    }
    else if (ispunct(cur_char[1])) {
      // use % as the prefix of escape charactor
      pOutStr.push_back(cur_char[1]); // %[punctuation] -> [punctuation]
      cur_char += 2;
      continue;
    }

    // skip the %
    ++cur_char;

    // This is a placeholder for a diagnostic argument. The format for a
    // placeholder is one of "%0, "%modifier0", or "%modifier{arguments}0".
    // The digit is a number from 0-9 indicating which argument this comes
    // from. The modifier is a string of digits from the set [-a-z]+. Arguments
    // are brace encolosed strings.
    const char* modifier = NULL, *argument = NULL;
    size_t modifier_len = 0, argument_len = 0;


    if (!isdigit(*cur_char)) { //< we get a modifier
      modifier = cur_char;
      while ('-' == *cur_char || (*cur_char >= 'a' && *cur_char <= 'z'))
        ++cur_char;
      modifier_len = cur_char - modifier;

      // If we have an argument, get it next.
      if ('{' == *cur_char) {
        ++cur_char; // Skip '{'.
        argument = cur_char;

        cur_char = findMatch('}', cur_char, pEnd);
        assert(cur_char != pEnd && "Mismatched {} in the diagnostic");
        argument_len = cur_char - argument;
        ++cur_char; // Skip '}'.
      }
    } // we get a modifier

    assert(isdigit(*cur_char) && "Invalid format for argument in diagnostic");

    unsigned int arg_no = *cur_char - '0';
    ++cur_char; // Skip argument number.

    ArgumentKind kind = getArgKind(arg_no);
    switch (kind) {
      case ak_std_string: {
        const std::string& str = getArgStdStr(arg_no);
        pOutStr.append(str.begin(), str.end());
        break;
      }
      case ak_c_string: {
        const char* str = getArgCStr(arg_no);
        if (NULL == str)
          str = "(null)";
        pOutStr.append(str);
        break;
      }
      case ak_sint32: {
        int32_t val = getArgSInt32(arg_no);
        OStrStream(pOutStr) << val;
        break;
      }
      case ak_uint32: {
        uint32_t val = getArgUInt32(arg_no);
        OStrStream(pOutStr) << val;
        break;
      }
      case ak_sint64: {
        int64_t val = getArgSInt64(arg_no);
        OStrStream(pOutStr) << val;
        break;
      }
      case ak_uint64: {
        uint64_t val = getArgUInt64(arg_no);
        OStrStream(pOutStr) << val;
        break;
      }
      case ak_bool: {
        if (getArgBool(arg_no))
          pOutStr.append("true");
        else
          pOutStr.append("false");
        break;
      }
    } // switch for kind
  } // end of while
}

/// getArgKind - get argument kind
diagnostic::ArgumentKind
diagnostic::Diagnostic::getArgKind(unsigned int pID) const
{
  assert(pID < getNumOfArgs() && "Argument index is out of range");
  return (ArgumentKind)m_State.ArgumentKinds[pID];
}

/// getArgStdStr - get argument as standard string
const std::string&
diagnostic::Diagnostic::getArgStdStr(unsigned int pID) const
{
  assert(ak_std_string == getArgKind(pID) &&
         "Invalid argument accessor!");
  return m_State.ArgumentStrs[pID];
}

/// getArgCStr - get argument as c-string
const char* diagnostic::Diagnostic::getArgCStr(unsigned int pID) const
{
  assert(ak_c_string == getArgKind(pID) &&
         "Invalid argument accessor!");
  return reinterpret_cast<const char*>(m_State.ArgumentVals[pID]);
}

/// getArgSInt32 - get argument as int32_t
int32_t diagnostic::Diagnostic::getArgSInt32(unsigned int pID) const
{
  assert(ak_sint32 == getArgKind(pID) &&
         "Invalid argument accessor!");
  return (int32_t)m_State.ArgumentVals[pID];
}

/// getArgUInt32 - get argument as uint32_t
uint32_t diagnostic::Diagnostic::getArgUInt32(unsigned int pID) const
{
  assert(ak_uint32 == getArgKind(pID) &&
         "Invalid argument accessor!");
  return (uint32_t)m_State.ArgumentVals[pID];
}

/// getArgSInt64 - get argument as int64_t
int64_t diagnostic::Diagnostic::getArgSInt64(unsigned int pID) const
{
  assert(ak_sint64 == getArgKind(pID) &&
         "Invalid argument accessor!");
  return (int64_t)m_State.ArgumentVals[pID];
}

/// getArgUInt64 - get argument as uint64_t
uint64_t diagnostic::Diagnostic::getArgUInt64(unsigned int pID) const
{
  assert(ak_uint64 == getArgKind(pID) &&
         "Invalid argument accessor!");
  return (uint64_t)m_State.ArgumentVals[pID];
}

/// getArgBool - get argument as boolean
bool diagnostic::Diagnostic::getArgBool(unsigned int pID) const
{
  assert(ak_bool == getArgKind(pID) &&
         "Invalid argument accessor!");
  return (bool)m_State.ArgumentVals[pID];
}

/// getRawValue - get the raw value of argument
intptr_t diagnostic::Diagnostic::getRawValue(unsigned int pID) const
{
  return m_State.ArgumentVals[pID];
}
