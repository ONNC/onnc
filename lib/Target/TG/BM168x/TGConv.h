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
#ifndef ONNX_BM_TGCONV_H
#define ONNX_BM_TGCONV_H

#include "ComputeOperator.h"
#include <onnx/common/ir.h>

namespace onnc {

// m_MemOperands: ifmap, weight, ofmap, bias
class TGConv : public ComputeOperator2
{
public:
  TGConv(const ::onnx::Node &pNode);

  void emit() const override;

private:
  int m_InN, m_InC, m_InH, m_InW;
  int m_Groups;
  int m_OutC;
  uint16_t m_KH, m_KW;
  uint16_t m_DilationH, m_DilationW;
  uint8_t m_PadH, m_PadW;
  uint8_t m_StrideH, m_StrideW;
  int m_DoBias;
};

} // namespace onnc

#endif
