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
#ifndef ONNX_BM1880_TGPRELU_H
#define ONNX_BM1880_TGPRELU_H
#include "BM188xComputeOperator.h"
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnc/Config/ONNX.h>

namespace onnc {
namespace BM188X {

class TGPRelu : public BM188xComputeOperator
{
public:
  TGPRelu(const xNode &pNode);

  void emit() const override;
  // m_MemOperands: input, slope, output
  TGPRelu *addMemOperands(MemOperand *pInput, MemOperand *pSlope,
                          MemOperand *pOutput);
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;

private:
  float m_Slope;
  int m_ChannelShared;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  int m_GTRShiftWidth;
  int m_LERShiftWidth;
  int m_GTScale;
};

} // namespace BM188X
} // namespace onnc

#endif
