//===- OptionPool.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_OPTION_POOL_H
#define ONNC_OPTION_OPTION_POOL_H
#include <onnc/ADT/StringMap.h>
#include <onnc/Option/OptDefs.h>

namespace onnc {
namespace cl {

/** \class OptionPool
 *  \brief OptionPool is a string map used to look up OptDefs.
 */
class OptionPool
{
public:
  typedef StringMap<OptDefs*> MapType;

public:
  /// lookup - Look up the option specified by the specified option on
  /// the command line. This assumes the leading dashes are stripped.
  ///
  /// Normal options, such as kLong and kShort will be found first, the
  /// prefix options.
  ///
  /// @param [in] pArg     The argument on the command line
  /// @param [out] pName   The name of the argument
  /// @param [out] pValue  The corresponding value
  /// @return OptDefs* The corresponding OptDefs
  OptDefs* lookup(StringRef pArg, StringRef& pName, StringRef& pValue);

  OptDefs* guess(StringRef& pName, StringRef& pValue);

  void record(OptDefs& pInfo);

private:
  /// LookupSimple - Look up simple options
  OptDefs* LookupSimple(StringRef& pArg, StringRef& pValue);

  /// LookupPrefix - Look up options with specified prefix
  OptDefs* LookupPrefix(StringRef& pArg, StringRef& pValue);

  /// LookupNearest - Look up options who is nearest by the requests.
  OptDefs* LookupNearest(StringRef& pArg, StringRef& pValue);

private:
  MapType m_Map;
};

} // namespace of cl
} // namespace of onnc

#endif
