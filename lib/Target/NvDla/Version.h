//===- Version.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_VERSION_H_INCLUDED
#define TARGET_NVDLA_VERSION_H_INCLUDED

#include <cstdint>

namespace onnc {

struct Version
{
  using uint_type = std::uint8_t;

  Version() = delete;
  constexpr Version(uint_type major, uint_type minor, uint_type subMinor) noexcept
    : major(major)
    , minor(minor)
    , subMinor(subMinor)
  {}
  constexpr Version(const Version&) = default;
  constexpr Version(Version&&)      = default;

  const uint_type major;
  const uint_type minor;
  const uint_type subMinor;
};

} // namespace onnc

#endif
