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

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <unordered_map>

namespace onnc {

typedef std::unordered_map<Value *, void *> AddressTable;

class CLangMeta
{
public:
  CLangMeta() = default;
  CLangMeta(const CLangMeta&) = delete;
  CLangMeta(CLangMeta&&) = delete;
  ~CLangMeta() = default;

  CLangMeta& operator=(const CLangMeta&) = delete;
  CLangMeta& operator=(CLangMeta&&) = delete;

public:
  AddressTable m_ATable;
};

} // namespace onnc

#endif
