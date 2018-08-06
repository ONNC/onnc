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
#ifndef ONNX_BM1880_TLPOOL_H
#define ONNX_BM1880_TLPOOL_H
#include "BM188xComputeOperator.h"
#include "TGBackend.h"
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnc/Config/ONNX.h>

namespace onnc {
namespace BM188X {

// m_emOperands: ifmap, weight, ofmap, bias
class TLPool : public BM188xComputeOperator
{
public:
  TLPool(const xNode &pNode);

  void emit() const override;
  TLPool *addMemOperands(MemOperand *pInput, MemOperand *pOutput);
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;

private:
  uint64_t m_IFmapAddr, m_OFmapAddr;

  int m_InN, m_InC, m_InH, m_InW;
  int m_OutN, m_OutC, m_OutH, m_OutW;
  uint16_t m_KH, m_KW;
  uint8_t m_PadHTop, m_PadHBot;
  uint8_t m_PadWLeft, m_PadWRight;
  uint8_t m_StrideH, m_StrideW;
  bool m_IsAvgPooling;

  int m_RShiftWidth;
  int m_ThresholdXQuantized;

  std::string m_SplitName;
};

} // namespace BM188X
} // namespace onnc

#endif
