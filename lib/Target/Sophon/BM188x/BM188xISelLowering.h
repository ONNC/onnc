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
#ifndef ONNC_TARGET_SOPHON_BM188X_ISELLOWERING_H
#define ONNC_TARGET_SOPHON_BM188X_ISELLOWERING_H
#include "BM188xBackend.h"
#include "TargetLowering.h"
#include <memory>
#include <onnc/Config/ONNX.h>

namespace onnc {

class BM1880Backend;

class BM188xISelLowering : public TargetLowering
{
public:
  BM188xISelLowering(TGBackend *pBackend) : TargetLowering(pBackend)
  {
    m_p1880backend = static_cast<BM1880Backend *>(pBackend);
  }

  ComputeOperator2 *LowerOperation(
      const xNode &pNode,
      std::vector<std::unique_ptr<ComputeOperator2> > &pInstList) override;

private:
  ComputeOperator2 *LowerConv(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerTLConv(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerRelu(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerPRelu(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerLeakyRelu(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerTLPool(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerMaxPool(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerAveragePool(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerGlobalAveragePool(const xNode &pNode,
                                           ComputeGraph &pGraph);
  ComputeOperator2 *LowerGemm(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *Lower2NopInst(const xNode &pNode);
  ComputeOperator2 *LowerFlatten(const xNode &pNode);
  ComputeOperator2 *LowerSum(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerUpsample(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerLRN(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerConcat(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerTranspose(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerScale(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerTLLoad(const xNode &pNode, ComputeGraph &pGraph);
  ComputeOperator2 *LowerTLStore(const xNode &pNode, ComputeGraph &pGraph);

private:
  BM1880Backend *m_p1880backend; // NOLINT
};

} // namespace onnc

#endif
