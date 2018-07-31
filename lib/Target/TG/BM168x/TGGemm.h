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
#ifndef ONNX_BM_TGGEMM_H
#define ONNX_BM_TGGEMM_H

#include "ComputeOperator.h"
#include <onnx/common/ir.h>
namespace onnc {

//  m_EmOperands: input, weight, bias, output
class TGGemm : public ComputeOperator2
{
public:
  TGGemm(const ::onnx::Node &pNode);
  void emit() const override;

private:
  int m_InRowNum;
  int m_InColNum;
  int m_OutColNum;
  int m_HaveBias;
  int m_UsingRelu;
  bool m_WeightTp;
};

} // namespace onnc

#endif
