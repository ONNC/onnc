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

  bool FuseOptimization(onnx::Graph *pGraph, const int64_t &pOpsetVersion);

  static onnx::Node *Fuse(::onnx::Node *pA, ::onnx::Node *pB);

protected:
  bool FuseOpset6Nodes(onnx::Graph *pGraph, onnx::Node *pNode);
  virtual onnx::Node *FuseChannelWiseMulAdd(onnx::Graph *pGraph,
                                            onnx::Node *pMulNode,
                                            onnx::Node *pAddNode);

  virtual onnx::Node *FuseBNScale(onnx::Graph *pGraph, onnx::Node *pBNNode,
                                  onnx::Node *pScaleNode);

  virtual onnx::Node *FuseConvScale(onnx::Graph *pGraph, onnx::Node *pConvNode,
                                    onnx::Node *pScaleNode);

  virtual onnx::Node *FuseRelu(onnx::Graph *pGraph, onnx::Node *pNode,
                               onnx::Node *pReluNode);

  virtual onnx::Node *FuseBN(onnx::Graph *pGraph, onnx::Node *pNode);

private:
  bool FuseNodes(::onnx::Graph *pGraph, const int64_t &pOpsetVersion);

protected:
  TGBackend *m_pBackend; // NOLINT
};

} // namespace onnc

#endif
