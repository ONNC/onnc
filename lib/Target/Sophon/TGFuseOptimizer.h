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
#ifndef TG_FUSE_OPTIMIZER_H
#define TG_FUSE_OPTIMIZER_H
#include <onnc/Config/ONNX.h>

namespace onnc {

class Module;
class TGBackend;

class TGFuseOptimizer
{

public:
  TGFuseOptimizer(TGBackend *pBackend) : m_pBackend(pBackend) {}

  virtual ~TGFuseOptimizer() = default;

  virtual void PrepareFuseOptimizer(Module &pModule) { return; };

  bool FuseOptimization(xGraph *pGraph, const int64_t &pOpsetVersion);

  static xNode *Fuse(xNode *pA, xNode *pB);

protected:
  bool FuseOpset6Nodes(xGraph *pGraph, xNode *pNode);
  bool FuseOpset7Nodes(xGraph *pGraph, xNode *pNode);

  virtual xNode *FuseConvScale(xGraph *pGraph, xNode *pConvNode,
                               xNode *pScaleNode);

  virtual xNode *FuseRelu(xGraph *pGraph, xNode *pNode, xNode *pReluNode);

  virtual xNode *FuseBNMulV6(xGraph *pGraph, xNode *pBNNode, xNode *pMul);

  virtual xNode *FuseBNAddV6(xGraph *pGraph, xNode *pBNNode, xNode *pAddNode);

  virtual xNode *FuseBNMul(xGraph *pGraph, xNode *pBNNode, xNode *pMul);

  virtual xNode *FuseBNAdd(xGraph *pGraph, xNode *pBNNode, xNode *pAddNode);

  virtual xNode *FuseBNMulTensor(xGraph *pGraph, xNode *pBNNode, xNode *pMul);

  virtual xNode *
  FuseBNAddTensor(xGraph *pGraph, xNode *pBNNode, xNode *pAddNode);

  virtual xNode *FuseBN(xGraph *pGraph, xNode *pNode);

  virtual xNode *AliasSumOperator(xGraph *pGraph, xNode *pAddNode);

private:
  bool FuseNodes(xGraph *pGraph, const int64_t &pOpsetVersion);

protected:
  TGBackend *m_pBackend; // NOLINT
};

} // namespace onnc

#endif
