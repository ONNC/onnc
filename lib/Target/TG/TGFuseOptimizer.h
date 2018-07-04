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

  virtual void PrepareFuseOptimizer(Module &pModule) { return; };

  bool FuseOptimization(onnx::Graph *pGraph);

  static onnx::Node *Fuse(::onnx::Node *pA, ::onnx::Node *pB);

protected:
  virtual void FuseGemmRelu(::onnx::Graph *pGraph, ::onnx::Node *pGemmNode,
                            ::onnx::Node *pReluNode);
  virtual void FuseChannelWiseMulAdd(onnx::Graph *pGraph, onnx::Node *pMulNode,
                                     onnx::Node *pAddNode);

  virtual void FuseBNScale(onnx::Graph *pGraph, onnx::Node *pBNNode,
                           onnx::Node *pScaleNode);

  virtual onnx::Node *FuseConvScale(onnx::Graph *pGraph, onnx::Node *pConvNode,
                                    onnx::Node *pScaleNode);

  virtual void FuseConvRelu(onnx::Graph *pGraph, onnx::Node *pConvNode,
                            onnx::Node *pReluNode);

  virtual void FuseSumRelu(onnx::Graph *pGraph, onnx::Node *pSumNode,
                           onnx::Node *pReluNode);

private:
  bool FuseNodes(::onnx::Graph *pGraph);

protected:
  TGBackend *m_pBackend; // NOLINT
};

} // namespace onnc

#endif
