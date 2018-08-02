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
#ifndef ONNX_BM1880_TGUPSAMPLE_H
#define ONNX_BM1880_TGUPSAMPLE_H

#include "BM188xComputeOperator.h"
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGUpsample : public BM188xComputeOperator
{
public:
  TGUpsample(const ::onnx::Node &pNode);

  void emit() const override;
  TGUpsample *addMemOperands(MemOperand *pInput, MemOperand *pOutput);
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override
  {
    return;
  }

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  int m_Scale;
};

} // namespace BM188X
} // namespace onnc

#endif
