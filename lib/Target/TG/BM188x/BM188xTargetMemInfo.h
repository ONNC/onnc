//===- BM188xMemInfo.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_BM188x_MEM_INFO_H
#define ONNC_BM188x_MEM_INFO_H
#include "TGBackend.h"
#include <onnc/Target/TargetMemInfo.h>

namespace onnc {

class TGBackend;
class BM188xTargetMemInfo : public TargetMemInfo
{
public:
  BM188xTargetMemInfo(TGBackend *pTGBackend) : m_pTGBackend(pTGBackend){};
  uint64_t getGlobalMemSize() const override;

  uint64_t getLocalMemSize() const override;

  uint64_t getElemSize(::onnx::TensorProto_DataType pTy) const override;
  MemSize getValueMemorySize(::onnx::Value *pValue) override;

private:
  TGBackend *m_pTGBackend; // NOLINT
};

} // namespace onnc

#endif
