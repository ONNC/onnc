#ifndef ONNX_BM1880_TGSUM_H
#define ONNX_BM1880_TGSUM_H

#include "ComputeOperator.h"
#include "TGBackend.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

class TGSum : public ComputeOperator2
{
public:
  TGSum(const ::onnx::Node &pNode,
        const tg::bm1880::LayerCalibrationParameter &pLayerCtable);

  void emit() const override;
  void print(OStream &pOS) const override;
  TGSum *addMemOperands(std::vector<MemOperand *> pVInput, MemOperand *pOutput);

private:
  int m_InN, m_InC, m_InH, m_InW;
  int m_InSize;
  tg::bm1880::LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
