//===-- Debug.cpp - An easy way to add debug output to your code ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements a handy way of adding debugging information to your
// code, without it being enabled all of the time, and without having to add
// command line options to enable it.
//
// In particular, just wrap your code with the DEBUG() macro, and it will be
// enabled automatically if you specify '-debug' on the command-line.
// Alternatively, you can also use the SET_DEBUG_TYPE("foo") macro to specify
// that your debug code belongs to class "foo".  Then, on the command line, you
// can specify '-debug-only=foo' to enable JUST the debug information for the
// foo class.
//
// When compiling without assertions, the -debug-* options and all code in
// DEBUG() statements disappears, so it does not affect the runtime of the code.
//
//===----------------------------------------------------------------------===//

#include "onnc/Support/Debug.h"
#include "onnc/Option/CommandLine.h"
#include "onnc/Support/ManagedStatic.h"

#undef isCurrentDebugType
#undef setCurrentDebugType
#undef setCurrentDebugTypes

using namespace onnc;

// Even though LLVM might be built with NDEBUG, define symbols that the code
// built without NDEBUG can depend on via the onnc/Support/Debug.h header.
namespace onnc {
/// Exported boolean set by the -debug option.

/// Return true if the specified string is the debug type
/// specified on the command line, or if none was specified on the command line
/// with the -debug-only=X option.
bool isCurrentDebugType(const char *DebugType)
{
  if (DebugMsg::getCurrentDebugType()->empty())
    return true;
  // See if DebugType is in list. Note: do not use find() as that forces us to
  // unnecessarily create an std::string instance.
  for (auto &d : *DebugMsg::getCurrentDebugType()) {
    if (d == DebugType)
      return true;
  }
  return false;
}

/// Set the current debug type, as if the -debug-only=X
/// option were specified.  Note that DebugOpt also needs to be set to true for
/// debug output to be produced.
///
void setCurrentDebugTypes(const char **Types, unsigned Count);

void setCurrentDebugType(const char *Type) { setCurrentDebugTypes(&Type, 1); }

void setCurrentDebugTypes(const char **Types, unsigned Count)
{
  DebugMsg::getCurrentDebugType()->clear();
  for (size_t T = 0; T < Count; ++T)
    DebugMsg::getCurrentDebugType()->push_back(Types[T]);
}
} // namespace onnc

#ifndef NDEBUG

namespace {

struct DebugOption {
  void operator=(const bool &Val) const
  {
    if (Val == true)
      DebugMsg::getDebugFlag() = true;
  }
};

struct DebugOnlyOpt {
  void operator=(const std::string &Val) const
  {
    DebugMsg::setDebugOnlyOpt(Val);
  }
};

} // namespace

namespace onnc {
DebugMsg::DebugMsg()
{
  // -debug - Command line option to enable the DEBUG statements in the passes.
  // This flag may only be enabled in debug builds.
  static cl::opt<DebugOption, cl::OptParser<bool> > debug_option(
      "debug", cl::kShort, cl::kOptional, cl::kValueDisallowed, cl::init(false),
      cl::desc("Enable debug output"));
  static cl::opt<DebugOnlyOpt, cl::OptParser<std::string> > debug_only(
      "debug-only", cl::kShort, cl::kOptional, cl::kValueRequired,
      cl::desc("Enable a specific type of debug output (comma "
               "separated list of types)"));
};
std::vector<std::string> *DebugMsg::getCurrentDebugType()
{
  static std::vector<std::string> current_debug_type;
  return &current_debug_type;
}
bool &DebugMsg::getDebugFlag()
{
  static bool debug_flag = false;
  return debug_flag;
}
void DebugMsg::setDebugOnlyOpt(const std::string &pVal)
{
  if (pVal.empty())
    return;
  DebugMsg::getDebugFlag() = true;
  std::vector<StringRef> dbgTypes;
  StringRef(pVal).split(dbgTypes, ',', -1, false);
  for (auto dbgType : dbgTypes)
    DebugMsg::getCurrentDebugType()->push_back(dbgType);
}
} // namespace onnc

/// dbgs - Return a debug stream.
onnc::OStream &onnc::dbgs() { return onnc::outs(); }

#else
// Avoid "has no symbols" warning.
namespace onnc {
/// dbgs - Return errs().
onnc::OStream &dbgs() { return errs(); }
} // namespace onnc

#endif
