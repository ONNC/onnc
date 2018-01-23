//===- EngineFwd.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_ENGINE_FORWARD_H
#define ONNC_DIAGNOSTIC_ENGINE_FORWARD_H
#include <onnc/ADT/StringRef.h>
#include <string>

namespace onnc {
namespace diagnostic {

/** \enum Severity
 *  \brief The severity level of a diagnostic
 */
enum Severity {
  Unreachable,
  Fatal,
  Error,
  Warning,
  Debug,
  Note,
  Ignore,
  None
};

enum ArgumentKind {
  ak_std_string,  // std::string
  ak_c_string,    // const char *
  ak_sint32,      // int32_t
  ak_uint32,      // uint32_t
  ak_sint64,      // uint64_t
  ak_uint64,      // uint64_t
  ak_bool         // bool
};

class State
{
public:
  enum { MaxArguments = 10 };

public:
  State();

  void reset();

public:
  StringRef   Format;
  std::string ArgumentStrs[MaxArguments];
  intptr_t    ArgumentVals[MaxArguments];
  uint8_t     ArgumentKinds[MaxArguments];
  uint8_t     NumOfArgs;
  uint16_t    ID;
  Severity    CurrentSeverity;
};

} // namespace of diagnostic
} // namespace of onnc

#endif
