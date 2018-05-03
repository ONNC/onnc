//===- onnc/Support/Host.h - Host machine characteristics ---*- C++ -*-===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
// Methods for querying the nature of the host machine.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_HOST_H
#define ONNC_SUPPORT_HOST_H

#if defined(__linux__) || defined(__GNU__)
#include <endian.h>
#else
#if !defined(BYTE_ORDER) && !defined(ONNC_ON_WIN32)
#include <machine/endian.h>
#endif
#endif

#include <string>

namespace onnc {
namespace sys {

#if defined(BYTE_ORDER) && defined(BIG_ENDIAN) && BYTE_ORDER == BIG_ENDIAN
  static const bool IsBigEndianHost = true;
#else
  static const bool IsBigEndianHost = false;
#endif

  static const bool IsLittleEndianHost = !IsBigEndianHost;

  /// Get the host quadruple.
  std::string GetHostQuadruple();

} // namespace of sys
} // namespace of onnc

#endif
