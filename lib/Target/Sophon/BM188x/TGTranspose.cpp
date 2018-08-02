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
#define DEBUG_TYPE "tg_transpose"
#include "TGTranspose.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGTranspose::TGTranspose(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Transpose")),
      m_NeedPermute(false)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;
  m_OutputShape.resize(inDim.size());

  auto &orders = pNode.is(::onnx::Symbol("perm"));
  assert((orders.size() == 4) || (orders.size() == 5));
  m_Order.resize(orders.size());
  assert(((orders[0] == 0) && (orders[1] == 2) && (orders[2] == 3) &&
          (orders[3] == 1)) ||
         ((orders[0] == 2) && (orders[1] == 3) && (orders[2] == 0) &&
          (orders[3] == 1)) ||
         ((orders[0] == 1) && (orders[1] == 0) && (orders[2] == 2) &&
          (orders[3] == 3)) ||
         ((orders[0] == 0) && (orders[1] == 2) && (orders[2] == 1) &&
          (orders[3] == 3) && (orders[4] == 4)));

  for (bmnet::bmnet_asm::u32 i = 0; i < orders.size(); ++i) {
    m_Order[i] = orders[i];
    m_OutputShape[i] = inDim[orders[i]].dim;
  }

  if (orders.size() == 5) { // shufflenet
    assert((orders[3] == 3) && (orders[4] == 4));
    m_W = inDim[3].dim * inDim[4].dim;
    m_OutputShape[3] = m_OutputShape[3] * m_OutputShape[4];
  }

  // Check if we need to reorder the data or keep it.
  for (bmnet::bmnet_asm::u32 i = 0; i < orders.size(); ++i) {
    if (orders[i] != i) {
      // As long as there is one order which is different from the natural order
      // of the data, we need to permute. Otherwise, we share the data and diff.
      m_NeedPermute = true;
      break;
    }
  }
}

TGTranspose *TGTranspose::addMemOperands(MemOperand *pInput,
                                         MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGTranspose::emit() const
{
  DEBUG(dbgs()
    << "TGTranspose::emit\n" << "  "
    << m_MemOperands[0]->m_Addr << " " << m_MemOperands[1]->m_Addr << " "
    << m_N << " " <<  m_C << " " <<  m_H << " " <<  m_W << " "
    << m_OutputShape[0] << " " << m_OutputShape[1] << " "
    << m_OutputShape[2] << " " << m_OutputShape[3] << " "
    << m_Order[0] << " " << m_Order[1] << " "
    << m_Order[2] << " " << m_Order[3] << " "
    << m_NeedPermute << "\n");

  bmnet::bmnet_asm::bmnet_permute_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, m_MemOperands[1]->m_Addr, m_N, m_C, m_H, m_W,
      m_OutputShape[0], m_OutputShape[1], m_OutputShape[2], m_OutputShape[3],
      m_Order[0], m_Order[1], m_Order[2], m_Order[3], m_NeedPermute);
}

} // namespace BM188X
} // namespace onnc
