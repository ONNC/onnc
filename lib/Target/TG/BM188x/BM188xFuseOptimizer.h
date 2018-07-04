#ifndef BM188X_FUSE_OPTIMIZER_H
#define BM188X_FUSE_OPTIMIZER_H

#include "BM188xBackend.h"
#include "TGFuseOptimizer.h"

namespace onnc {

class BM188xFuseOptimizer : public TGFuseOptimizer
{
public:
  BM188xFuseOptimizer(TGBackend *pBackend) : TGFuseOptimizer(pBackend)
  {
    m_p1880backend = static_cast<BM1880Backend *>(pBackend);
  }

  ~BM188xFuseOptimizer() override = default;

  void FuseGemmRelu(::onnx::Graph *pGraph, ::onnx::Node *pGemmNode,
                    ::onnx::Node *pReluNode) override;

  onnx::Node *FuseConvScale(onnx::Graph *pGraph, onnx::Node *pConvNode,
                            onnx::Node *pScaleNode) override;

private:
  BM1880Backend *m_p1880backend; // NOLINT
};

} // namespace onnc

#endif
