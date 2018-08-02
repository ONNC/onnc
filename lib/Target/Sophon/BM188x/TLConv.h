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
#ifndef ONNX_BM1880_TLCONV_H
#define ONNX_BM1880_TLCONV_H

#include "BM188xComputeOperator.h"
#include "TGBackend.h"
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_emOperands: ifmap, weight, ofmap, bias
class TLConv : public BM188xComputeOperator
{
public:
  TLConv(const ::onnx::Node &pNode);

  void emit() const override;
  TLConv *addMemOperands(MemOperand *pInput, MemOperand *pWeight,
                         MemOperand *pOutput, MemOperand *pBias);
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;
  bool isDoBias() { return (m_DoBias == 1) ? true : false; }
  int getBiasIdx() { return m_BiasIdx; }
  float getConvOuputThreshold() { return m_ConvOutputThreshold; }

  int getInN() const { return m_InN; }
  int getInC() const { return m_InC; }
  int getInH() const { return m_InH; }
  int getInW() const { return m_InW; }
  int getGroups() const { return m_Groups; }
  int getOutC() const { return m_OutC; }
  int getOutH() const { return m_OutH; }
  int getOutW() const { return m_OutW; }
  uint16_t getKH() const { return m_KH; }
  uint16_t getKW() const { return m_KW; }

  bool getDoBias() const { return m_DoBias; }
  int getBiasIdx() const { return m_BiasIdx; }

private:
  uint64_t m_IFmapAddr, m_OFmapAddr, m_WeightAddr, m_BiasAddr;

  int m_InN, m_InC, m_InH, m_InW;
  int m_Groups;
  int m_OutC, m_OutH, m_OutW;
  uint16_t m_KH, m_KW;
  uint16_t m_DilationH, m_DilationW;
  uint8_t m_PadHTop, m_PadHBot;
  uint8_t m_PadWLeft, m_PadWRight;
  uint8_t m_StrideH, m_StrideW;
  bool m_DoResultAdd;

  bool m_DoBias;
  bool m_UseWinograd;
  bool m_DoRelu;
  int m_RShiftWidth;
  float m_ConvOutputThreshold; // FIXME(arcbbb): is it required?

  int m_BiasIdx;

  std::string m_SplitName;
};

} // namespace BM188X
} // namespace onnc

#endif
