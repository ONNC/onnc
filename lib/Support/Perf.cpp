//===- Perf.cpp ----------------------------------------------------------===//
//
//                     The ONNC Team
//
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Perf.h>
#include <onnc/Config/Config.h>

//===----------------------------------------------------------------------===//
// Perf Implementation
//===----------------------------------------------------------------------===//
#if defined(ONNC_ON_WIN32)
#include "Windows/Perf.inc"
#endif

#if defined(ONNC_ON_UNIX)
#include "Unix/Perf.inc"
#endif

#if defined(ONNC_ON_DRAGON)
#include "Dragon/Perf.inc"
#endif
