//===- Random.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Random.h>
#include <onnc/Config/Config.h>

// Include the truly platform-specific parts. 
#if defined(ONNC_ON_UNIX)
#include "Unix/Random.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/Random.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/Random.inc"
#endif
