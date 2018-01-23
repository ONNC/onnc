//===- Timer.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Timer.h>
#include <onnc/Config/Config.h>

//===----------------------------------------------------------------------===//
// Timer Implementation
//===----------------------------------------------------------------------===//
// Include the truly platform-specific parts. 
#if defined(ONNC_ON_UNIX)
#include "Unix/Timer.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/Timer.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/Timer.inc"
#endif
