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
