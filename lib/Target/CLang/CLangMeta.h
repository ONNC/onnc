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
  using TensorBlockPair            = std::pair<const Tensor*, CLangMemoryBlock>;
  using PackedWeightMemoryBlocks   = std::vector<TensorBlockPair>;
  using PackedInternalMemoryBlocks = std::vector<TensorBlockPair>;
  using UsedOperatorNames          = std::set<std::string>;
  using PackedInputMemoryBlocks    = std::vector<TensorBlockPair>;

  CLangMeta()                 = default;
  CLangMeta(const CLangMeta&) = delete;
  CLangMeta(CLangMeta&&)      = delete;
  ~CLangMeta()                = default;

  CLangMeta& operator=(const CLangMeta&) = delete;
  CLangMeta& operator=(CLangMeta&&) = delete;

  static const std::string weightFileExt;
  static const std::string serviceLibraryFileExt;

  PackedInputMemoryBlocks    packedInputMemoryBlocks;
  PackedWeightMemoryBlocks   packedWeightMemoryBlocks;
  PackedInternalMemoryBlocks packedInternalMemoryBlocks;
  UsedOperatorNames          usedOperatorNames;
};

} // namespace onnc

#endif
