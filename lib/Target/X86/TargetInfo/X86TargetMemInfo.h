//===- X86TargetMemInfo.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_X86_MEM_INFO_H
#define ONNC_TARGET_X86_MEM_INFO_H
#include <onnc/Target/TargetMemInfo.h>

namespace onnc {

class X86TargetMemInfo : public TargetMemInfo
{
public:
  MemSize getTensorMemorySize(const Tensor& pVal) override;
};

} // namespace onnc

#endif