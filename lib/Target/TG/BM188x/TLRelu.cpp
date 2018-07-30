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
#include "TLRelu.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

#define DEBUG_TYPE "tl_relu"

namespace onnc {
namespace BM188X {

TLRelu::TLRelu(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("TLRelu"))
{
  // ONNC extension attribute
  assert(pNode.hasAttribute(::onnx::Symbol("op_name")));
  m_SplitName = pNode.s(::onnx::Symbol("op_name"));

  assert(pNode.hasAttribute(::onnx::Symbol("input_dim")));
  assert(pNode.hasAttribute(::onnx::Symbol("output_dim")));
  auto &inDim = pNode.is(::onnx::Symbol("input_dim"));
  m_InN = inDim[0];
  m_InC = inDim[1];
  m_InH = inDim[2];
  m_InW = inDim[3];
  auto &outDim = pNode.is(::onnx::Symbol("output_dim"));
  m_OutC = outDim[1];
  m_OutH = outDim[2];
  m_OutW = outDim[3];

  assert(pNode.hasAttribute(::onnx::Symbol("ifmap_laddr")));
  assert(pNode.hasAttribute(::onnx::Symbol("ofmap_laddr")));
  m_IFmapAddr = pNode.i(::onnx::Symbol("ifmap_laddr"));
  m_OFmapAddr = pNode.i(::onnx::Symbol("ofmap_laddr"));
  // End extension
}

void TLRelu::emit() const
{
  bmnet::bmnet_asm::bmnet_tl_relu_forward_bmkernel(m_IFmapAddr, m_OFmapAddr,
                                                   m_InN, m_InC, m_InH, m_InW);
}

TLRelu *TLRelu::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

} // namespace BM188X
} // namespace onnc
