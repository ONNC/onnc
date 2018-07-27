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
#ifndef ONNX_BM1880_TGLEAKYRELU_H
#define ONNX_BM1880_TGLEAKYRELU_H

#include "BM188xComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

class TGLeakyRelu : public BM188xComputeOperator
{
public:
  TGLeakyRelu(const ::onnx::Node &pNode);

  void emit() const override;
  TGLeakyRelu *addMemOperands(MemOperand *pInput, MemOperand *pOutput);
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;
  float getAlpha() { return m_Alpha; }

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  float m_Alpha;
  int m_GTRShiftWidth;
  int m_LERShiftWidth;
  int m_GTScale;
  int m_LEScale;
};

} // namespace BM188X
} // namespace onnc

#endif
