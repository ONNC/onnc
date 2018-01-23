//===-- OptionPool.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Option/OptionPool.h>
#include <onnc/ADT/EditDistance.h>

using namespace onnc;
using namespace onnc::cl;

//===----------------------------------------------------------------------===//
// OptionPool
//===----------------------------------------------------------------------===//

/// lookup - Look up the option specified by the specified option on
/// the command line. This assumes the leading dashes are stripped.
/// @param [in] pArg       The argument on the command line
/// @param [out] pName  The name of the argument
/// @param [out] pValue The corresponding value
/// @return OptDefs*   The corresponding OptDefs
OptDefs*
OptionPool::lookup(StringRef pArg, StringRef& pName, StringRef& pValue)
{
  OptDefs* result = LookupSimple(pArg, pValue);
  if (NULL == result)
    result = LookupPrefix(pArg, pValue);

  pName = pArg;
  return result;
}

OptDefs* OptionPool::guess(StringRef& pArg, StringRef& pValue)
{
  // Reject all dash
  if (pArg.empty())
    return NULL;

  return LookupNearest(pArg, pValue);
}

void OptionPool::record(OptDefs& pInfo)
{
  bool exist = false;
  MapType::entry_type* entry = m_Map.insert(pInfo.getArgStr(), exist);
  entry->setValue(&pInfo);
  if (exist) {
    fatal(pInfo, "The option has been registered!");
  }
}

/// LookupSimple - Look up simple options
OptDefs* OptionPool::LookupSimple(StringRef& pArg, StringRef& pValue)
{
  StringRef::size_type equal_pos = pArg.find('=');

  if (StringRef::npos == equal_pos) {
    MapType::iterator entry = m_Map.find(pArg);
    if (m_Map.end() != entry)
      return entry->value();
    return NULL;
  }

  // If we have an equal sign, remember the value;
  pValue = pArg.substr(equal_pos+1);
  pArg   = pArg.substr(0, equal_pos);
  MapType::iterator entry = m_Map.find(pArg);
  if (m_Map.end() == entry) {
    return NULL;
  }
  return entry->value();
}

/// LookupPrefix - Look up options with specified prefix
OptDefs* OptionPool::LookupPrefix(StringRef& pArg, StringRef& pValue)
{
  StringRef name(pArg);
  MapType::iterator entry = m_Map.find(name);

  // start matching from longer sub-string
  while (m_Map.end() == entry && name.size() > 1) {
    // Chip off the last charactor. StringRef has fast O(1) substr.
    name = name.substr(0, name.size()-1);
    entry = m_Map.find(name);
  }

  // found
  if (m_Map.end() != entry &&
      cl::kNoneSeparated == entry->value()->getDelimiter()) {
    size_t length = name.size();
    pValue = pArg.substr(length);
    pArg = pArg.substr(0, length);
    return entry->value();
  }
  return NULL;
}

/// LookupNearest - Look up options who is nearest by the requests.
OptDefs* OptionPool::LookupNearest(StringRef& pArg, StringRef& pValue)
{
  StringRef::size_type equal_pos = pArg.find('=');

  StringRef lhs = pArg.substr(0, equal_pos);
  StringRef rhs = pArg.substr(equal_pos+1);

  if (lhs.empty())
    return NULL;

  OptDefs* best = NULL;
  unsigned int best_dist = 0;
  MapType::iterator it, iEnd = m_Map.end();
  for (it = m_Map.begin(); it != iEnd; ++it) {
    OptDefs* option = it->value();
    if (!option->hasArgStr())
      continue;

    StringRef option_name(option->getArgStr());
    unsigned int distance = EditDistance(lhs, option_name);
    if (NULL == best || distance < best_dist) {
      best_dist = distance;
      best = option;
    }
  }

  return best;
}
