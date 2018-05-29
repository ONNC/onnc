//===- BM188xMemInfo.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_BM188x_MEM_INFO_H
#define ONNC_BM188x_MEM_INFO_H
#include <onnc/Target/TargetMemInfo.h>

namespace onnc {

class BM188xTargetMemInfo : public TargetMemInfo
{
public:
  size_t getGlobalMemSize() const override;

  size_t getLocalMemSize() const override;

  size_t getElemSize(onnx::TensorProto_DataType pTy) const override;
  MemSize getValueMemorySize(onnx::Value *pValue) override;
};

} // namespace onnc

#endif
