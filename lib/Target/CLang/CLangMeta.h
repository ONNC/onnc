//===- CLangMeta.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_CLANG_META_H
#define TARGET_CLANG_META_H

#include <onnc/IR/Compute/Tensor.h>

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace onnc {

struct CLangMemoryBlock
{
  using address_type = std::uint64_t;
  using size_type    = std::uint64_t;

  address_type offset;
  size_type    length;
};

struct CLangMeta
{
  using PackedWeightMemoryBlocks   = std::vector<std::pair<const Tensor*, CLangMemoryBlock>>;
  using PackedInternalMemoryBlocks = std::vector<std::pair<const Tensor*, CLangMemoryBlock>>;
  using UsedOperatorNames          = std::set<std::string>;

  CLangMeta()                 = default;
  CLangMeta(const CLangMeta&) = delete;
  CLangMeta(CLangMeta&&)      = delete;
  ~CLangMeta()                = default;

  CLangMeta& operator=(const CLangMeta&) = delete;
  CLangMeta& operator=(CLangMeta&&) = delete;

  static const std::string weightFileExt;
  static const std::string serviceLibraryFileExt;

  PackedWeightMemoryBlocks   packedWeightMemoryBlocks;
  PackedInternalMemoryBlocks packedInternalMemoryBlocks;
  UsedOperatorNames          usedOperatorNames;
};

} // namespace onnc

#endif
