#define DEBUG_TYPE "bm188x_fuse_opt"
#include "BM188xFuseOptimizer.h"
#include "BM188xBackend.h"
#include <onnc/Support/Debug.h>

namespace onnc {

void BM188xFuseOptimizer::init(Module &pModule)
{
  auto &ctable = m_p1880backend->getCtable(pModule);
  m_p1880backend->setCtableProto(ctable);
}

void BM188xFuseOptimizer::FuseGemmRelu(::onnx::Graph *pGraph,
                                       ::onnx::Node *pGemmNode,
                                       ::onnx::Node *pReluNode)
{
  // TODO quantized and update ctable
  std::string layerName = pGemmNode->output()->uniqueName();
  auto &layerCtable = m_p1880backend->getCtableLayerParam(layerName);
  DEBUG(dbgs() << "get ctable:" << layerCtable.DebugString(););
  // do fuse
  TGFuseOptimizer::FuseGemmRelu(pGraph, pGemmNode, pReluNode);
}

} // namespace onnc
