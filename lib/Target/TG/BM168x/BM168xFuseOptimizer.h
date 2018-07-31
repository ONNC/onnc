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
#ifndef BM168X_FUSE_OPTIMIZER_H
#define BM168X_FUSE_OPTIMIZER_H

#include "BM168xBackend.h"
#include "TGFuseOptimizer.h"

namespace onnc {

class BM168xFuseOptimizer : public TGFuseOptimizer
{
public:
  BM168xFuseOptimizer(TGBackend *pBackend) : TGFuseOptimizer(pBackend) {}

  ~BM168xFuseOptimizer() override = default;
};

} // namespace onnc

#endif
