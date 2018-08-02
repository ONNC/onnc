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
#ifndef ONNX_BM1880_TGSUM_H
#define ONNX_BM1880_TGSUM_H

#include "BM188xComputeOperator.h"
#include "TGBackend.h"
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

class TGSum : public BM188xComputeOperator
{
public:
  TGSum(const ::onnx::Node &pNode);

  void emit() const override;
  TGSum *addMemOperands(std::vector<MemOperand *> pVInput, MemOperand *pOutput);
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;

private:
  int m_InN, m_InC, m_InH, m_InW;
  size_t m_InputNum;
  int m_RShiftWidth;
  int m_DoRelu;
  std::vector<int> m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
