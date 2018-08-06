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
#ifndef ONNC_TARGET_BM188X_MEM_INFO_H
#define ONNC_TARGET_BM188X_MEM_INFO_H
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

  uint64_t getElemSize(xTensorProtoDataType pTy) const override;
  MemSize getValueMemorySize(xValue *pValue) override;

private:
  TGBackend *m_pTGBackend; // NOLINT
};

} // namespace onnc

#endif
