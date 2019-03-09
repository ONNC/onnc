//===- NvDlaTargetMemInfo.h --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_NVDLA_MEM_INFO_H
#define ONNC_TARGET_NVDLA_MEM_INFO_H
#include <onnc/Target/TargetMemInfo.h>

namespace onnc {
class NvDlaTargetMemInfo : public TargetMemInfo
{
public:
  MemSize getTensorMemorySize(const Tensor& pVal) override;
};

} // namespace onnc

#endif
