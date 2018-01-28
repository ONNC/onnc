//===- TargetSelect.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_SELECT_H
#define ONNC_TARGET_TARGET_SELECT_H

extern "C" {
// Declare all of the platform-specific information
#define ONNC_PLATFORM(TargetName) void Initialize##TargetName##ONNCPlatform();
#include "onnc/Config/Platforms.def"  // NOLINT [build/include] [4]

// Declare all of the available backends
#define ONNC_BACKEND(TargetName) void Initialize##TargetName##ONNCBackend();
#include "onnc/Config/Backends.def"  // NOLINT [build/include] [4]

}  // extern "C"

namespace onnc {

/// The main program should call this function if it wants access to all
/// available target information that ONNC is configured to support, to make
/// them available via the TargetRegistry.
///
/// It is legal for a client to make multiple calls to this function.
inline void InitializeAllPlatforms() {
#define ONNC_PLATFORM(TargetName) Initialize##TargetName##ONNCPlatform();
#include "onnc/Config/Platforms.def"  // NOLINT [build/include] [4]
}

/// InitializeAllTargets - The main program should call this function if it
/// wants access to all available target machines that ONNC is configured to
/// support, to make them available via the TargetRegistry.
///
/// It is legal for a client to make multiple calls to this function.
inline void InitializeAllBackends() {
#define ONNC_BACKEND(TargetName) Initialize##TargetName##ONNCBackend();
#include "onnc/Config/Backends.def"  // NOLINT [build/include] [4]
}

} // namespace of onnc

#endif
