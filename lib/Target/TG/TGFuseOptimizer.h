#ifndef TG_FUSE_OPTIMIZER_H
#define TG_FUSE_OPTIMIZER_H

#include <onnx/common/ir.h>

namespace onnc {

class Module;
class TGBackend;

class TGFuseOptimizer
{

public:
  TGFuseOptimizer(TGBackend *pBackend) : m_pBackend(pBackend) {}

  virtual ~TGFuseOptimizer() = default;

  virtual void init(Module &pModule) { return; };

  virtual void FuseGemmRelu(::onnx::Graph *pGraph, ::onnx::Node *pGemmNode,
                            ::onnx::Node *pReluNode);

protected:
  TGBackend *m_pBackend; // NOLINT
};

} // namespace onnc

#endif
