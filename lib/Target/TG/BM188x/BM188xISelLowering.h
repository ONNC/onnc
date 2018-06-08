#pragma once

#include "BM188xBackend.h"
#include "TargetLowering.h"
#include <memory>
#include <onnx/common/ir.h>

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
      const ::onnx::Node &node,
      std::vector<std::unique_ptr<ComputeOperator2> > &instList) override;

  void PrepareISelLowering(Module &pModule) override;

private:
  ComputeOperator2 *
  LowerConv(const ::onnx::Node &pNode, ComputeGraph &graph,
            const tg::bm1880::LayerCalibrationParameter &layerCtable);
  ComputeOperator2 *
  LowerRelu(const ::onnx::Node &pNode, ComputeGraph &graph,
            const tg::bm1880::LayerCalibrationParameter &layerCtable);
  ComputeOperator2 *
  LowerMaxPool(const ::onnx::Node &pNode, ComputeGraph &graph,
               const tg::bm1880::LayerCalibrationParameter &layerCtable);
  ComputeOperator2 *
  LowerGemm(const ::onnx::Node &pNode, ComputeGraph &graph,
            const tg::bm1880::LayerCalibrationParameter &layerCtable);
  ComputeOperator2 *LowerReshape(const ::onnx::Node &pNode);

private:
  BM1880Backend *m_p1880backend;
};

} // namespace onnc
