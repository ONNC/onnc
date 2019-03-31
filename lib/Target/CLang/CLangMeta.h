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
  CLangMeta();

  ~CLangMeta();

public:
  AddressTable m_ATable;
};


} // namespace onnc

#endif
