//===- MsgHandler.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_MSGHANDLER_H
#define ONNC_DIAGNOSTIC_MSGHANDLER_H
#include <cstdio>
#include <cstring>
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/DataTypes.h>
#include <onnc/Support/Path.h>
#include <onnc/Support/ErrorCode.h>
#include <onnc/Diagnostic/EngineFwd.h>

namespace onnc {
namespace diagnostic {

class Engine;

/** \class onnc::MsgHandler
 *  \brief handles the timing of reporting a diagnostic result.
 *
 *  MsgHandler controls the process to display the message. The message is
 *  emitted when calling MsgHandler's destructor.
 */
class MsgHandler
{
public:
  /// Constructor. Use named diagnostic message 
  MsgHandler(Engine& pEngine);

  /// Destructor. Emission of message happens.
  ~MsgHandler();

  /// emit the message
  bool emit();

  /// add an argument as a std::string.
  void addString(const std::string& pStr) const;

  /// add an argument with tagged value.
  void addTaggedValue(intptr_t pValue, ArgumentKind pKind) const;

private:
  void flushCounts();

private:
  Engine& m_Engine;
  mutable unsigned int m_NumOfArgs;
};

} // namespace of diagnostic

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, StringRef pStr)
{
  pHandler.addString(pStr.str());
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, const std::string& pStr)
{
  pHandler.addString(pStr);
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, const Path& pStr)
{
  pHandler.addString(pStr.native());
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, SystemError pError)
{
  if (SystemError::kNotStartedYet == pError.code())
    pHandler.addString("not started yet");
  else if (SystemError::kSuccess == pError.code())
    pHandler.addString("success");
  else
    pHandler.addString(::strerror(pError.code()));
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, const char* pStr)
{
  pHandler.addString(pStr);
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, int32_t pValue)
{
  pHandler.addTaggedValue(pValue, diagnostic::ak_sint32);
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, uint32_t pValue)
{
  pHandler.addTaggedValue(pValue, diagnostic::ak_uint32);
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, int64_t pValue)
{
  pHandler.addTaggedValue(pValue, diagnostic::ak_sint64);
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, uint64_t pValue)
{
  pHandler.addTaggedValue(pValue, diagnostic::ak_uint64);
  return pHandler;
}

inline const diagnostic::MsgHandler&
operator<<(const diagnostic::MsgHandler& pHandler, bool pValue)
{
  pHandler.addTaggedValue(pValue, diagnostic::ak_bool);
  return pHandler;
}

} // namespace of onnc

#endif
