//===- CLangMeta.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_CLANG_META_H
#define TARGET_CLANG_META_H

#include <cassert>
#include <cstdint>
#include <cstdio>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <unordered_map>

#include <onnc/IR/Compute/Tensor.h>

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
  using WeightMemoryBlocks = std::unordered_map<
    const Tensor*, CLangMemoryBlock
  >;
  using PackedInternalMemoryBlocks = std::vector<
    std::pair<const Tensor*, CLangMemoryBlock>
  >;

  CLangMeta() = default;
  CLangMeta(const CLangMeta&) = delete;
  CLangMeta(CLangMeta&&) = delete;
  ~CLangMeta() = default;

  CLangMeta& operator=(const CLangMeta&) = delete;
  CLangMeta& operator=(CLangMeta&&) = delete;

  WeightMemoryBlocks         weightMemoryBlocks;
  PackedInternalMemoryBlocks packedInternalMemoryBlocks;
};

} // namespace onnc

#endif
