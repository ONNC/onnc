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
#ifndef ONNX_BM1880_TGCONV_H
#define ONNX_BM1880_TGCONV_H

#include "BM188xComputeOperator.h"
#include "TGBackend.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_emOperands: ifmap, weight, ofmap, bias
class TGConv : public BM188xComputeOperator
{
public:
  TGConv(const ::onnx::Node &pNode);

  void emit() const override;
  void prepareWeight(std::vector<int8_t> &pWeight);
  TGConv *addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                         MemOperand *pWeight, MemOperand *pBias,
                         MemOperand *pScale, MemOperand *pScaleBias);
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;
  bool isDoBias() { return (m_DoBias == 1) ? true : false; }
  bool isDoScale() { return m_DoScale ? true : false; }
  bool isDoScaleBias() { return m_DoScaleBias ? true : false; }
  int getBiasIdx() { return m_BiasIdx; }
  int getScaleIdx() { return m_ScaleIdx; }
  int getScaleBiasIdx() { return m_ScaleBiasIdx; }
  float getConvOuputThreshold() { return m_ConvOutputThreshold; }
  int getInN() const { return m_InN; }
  int getInC() const { return m_InC; }
  int getInH() const { return m_InH; }
  int getInW() const { return m_InW; }
  int getGroups() const { return m_Groups; }
  int getOutC() const { return m_OutC; }

  uint16_t getKH() const { return m_KH; }
  uint16_t getKW() const { return m_KW; }

  int getDoBias() const { return m_DoBias; }
  int getDoScale() const { return m_DoScale; }
  int getDoScaleBias() const { return m_DoScaleBias; }

  int getBiasIdx() const { return m_BiasIdx; }
  int getScaleIdx() const { return m_ScaleIdx; }
  int getScaleBiasIdx() const { return m_ScaleBiasIdx; }

private:
  int m_InN, m_InC, m_InH, m_InW;
  int m_Groups;
  int m_OutC;
  uint16_t m_KH, m_KW;
  uint16_t m_DilationH, m_DilationW;
  uint8_t m_PadH, m_PadW;
  uint8_t m_StrideH, m_StrideW;
  int m_DoBias;
  // int m_DoRelu;
  int m_DoScale;
  int m_DoScaleBias;
  int m_RShiftWidth;
  int m_ScaleRShiftWidth;
  int m_BiasIdx, m_ScaleIdx, m_ScaleBiasIdx;
  float m_ConvOutputThreshold;
  // TODO add prelu_param
};

} // namespace BM188X
} // namespace onnc

#endif
