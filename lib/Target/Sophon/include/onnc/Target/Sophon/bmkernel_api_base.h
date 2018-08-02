//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#ifndef BMKERNEL_API_BASH_H
#define BMKERNEL_API_BASH_H

#include <ostream>
#include <string>

namespace bmnet {
namespace bmnet_asm {

using gaddr_t = uint64_t;
using laddr_t = uint64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

const gaddr_t GADDR_INVALID = 0x000000FFFFFFFFFFULL;

class asm_context
{
private:
  std::ostream *fp{ nullptr };
  asm_context() = default;

public:
  std::string name;
  bool on() { return fp != nullptr; }
  std::ostream &get_fp() { return *fp; }
  void set_fp(std::ostream &fp_in) { fp = &fp_in; }
  static asm_context &get_context()
  {
    static asm_context actx;
    return actx;
  };
};
} // namespace bmnet_asm
} // namespace bmnet

#endif /* BMKERNEL_API_BASH_H */
