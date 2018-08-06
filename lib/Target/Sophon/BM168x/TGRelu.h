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
#ifndef ONNC_TARGET_BM168X_TGRELU_H
#define ONNC_TARGET_BM168X_TGRELU_H
#include "ComputeOperator.h"
#include <onnc/Config/ONNX.h>

namespace onnc {

// m_MemOperands: input, output
class TGRelu : public ComputeOperator2
{
public:
  TGRelu(const xNode &pNode);

  void emit() const override;

private:
  float m_NegativeSlope;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
};

} // namespace onnc

#endif
