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
// legacy code
#ifndef ONNX_BM1880_TLRELU_H
#define ONNX_BM1880_TLRELU_H

#include "BM188xComputeOperator.h"
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnc/Config/ONNX.h>

namespace onnc {
namespace BM188X {

class TLRelu : public BM188xComputeOperator
{
public:
  TLRelu(const xNode &pNode);

  void emit() const override;
  TLRelu *addMemOperands(MemOperand *pInput, MemOperand *pOutput);

private:
  int m_InN, m_InC, m_InH, m_InW;
  int m_OutC, m_OutH, m_OutW;
  std::string m_SplitName;
  uint64_t m_IFmapAddr, m_OFmapAddr;
};

} // namespace BM188X
} // namespace onnc

#endif
