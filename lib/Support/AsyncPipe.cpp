//===- AsyncPipe.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/AsyncPipe.h>
#include <onnc/Config/Config.h>

// Include the truly platform-specific parts. 
#if defined(ONNC_ON_UNIX)
#include "Unix/AsyncPipe.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/AsyncPipe.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/AsyncPipe.inc"
#endif
