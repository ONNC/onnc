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
#define DEBUG_TYPE "tg_upsample"
#include "TGUpsample.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGUpsample::TGUpsample(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Upsample"))
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;
  assert(pNode.is(::onnx::Symbol("size"))[0] == 1.0);
  assert(pNode.is(::onnx::Symbol("size"))[1] == 1.0);
  assert(pNode.is(::onnx::Symbol("size"))[2] ==
         pNode.is(::onnx::Symbol("size"))[3]);
  m_Scale = pNode.is(::onnx::Symbol("size"))[3];
}

TGUpsample *TGUpsample::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGUpsample::emit() const
{
  DEBUG(dbgs()
    << "TGUpsample::emit\n" << "  "
    << m_MemOperands[0]->m_Addr << " " << m_MemOperands[1]->m_Addr << " "
    << m_N << " " <<  m_C << " " <<  m_H << " " <<  m_W << " "
    << m_Scale << "\n");

  bmnet::bmnet_asm::bmnet_upsample_fixed_bmkernel(
      m_MemOperands[0]->m_Addr, // ifmap_gaddr
      m_MemOperands[1]->m_Addr, // ofmap_gaddr
      m_N, m_C, m_H, m_W, m_Scale);
}

} // namespace BM188X
} // namespace onnc
