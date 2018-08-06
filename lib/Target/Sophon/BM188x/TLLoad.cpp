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
#include "TLLoad.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/BM188x/bmkernel_api.h>

#define DEBUG_TYPE "tl_load"

namespace onnc {
namespace BM188X {
TLLoad::TLLoad(const xNode &pNode)
    : BM188xComputeOperator(pNode, std::string("TLLoad"))
{
  // ONNC extension attribute
  assert(pNode.hasAttribute(xSymbol("src_goffset")));
  assert(pNode.hasAttribute(xSymbol("dst_laddr")));

  assert(pNode.hasAttribute(xSymbol("local_dim")));
  assert(pNode.hasAttribute(xSymbol("global_dim")));

  assert(pNode.hasAttribute(xSymbol("do_transpose")));
  assert(pNode.hasAttribute(xSymbol("is_aligned")));
  assert(pNode.hasAttribute(xSymbol("is_neuron")));
  assert(pNode.hasAttribute(xSymbol("op_name")));
  m_SplitName = pNode.s(xSymbol("op_name"));

  m_SrcGOffset = pNode.i(xSymbol("src_goffset"));
  m_DstLAddr = pNode.i(xSymbol("dst_laddr"));

  m_DoTranspose = pNode.i(xSymbol("do_transpose"));
  m_IsAligned = pNode.i(xSymbol("is_aligned"));
  m_IsNeuron = pNode.i(xSymbol("is_neuron"));

  auto &local_dim = pNode.is(xSymbol("local_dim"));
  assert(local_dim.size() == 4);
  m_LocalN = local_dim[0];
  m_LocalC = local_dim[1];
  m_LocalH = local_dim[2];
  m_LocalW = local_dim[3];

  auto &global_dim = pNode.is(xSymbol("global_dim"));
  assert(global_dim.size() == 4);
  m_GlobalC = global_dim[1];
  m_GlobalH = global_dim[2];
  m_GlobalW = global_dim[3];
  // End extension
}

void TLLoad::emit() const
{
  // Calculate the address after Global Memory Allocation Pass
  uint64_t gaddr = m_SrcGOffset + m_MemOperands[0]->m_Addr;

  DEBUG(dbgs()
    << "TLLoad::emit" << "\n"
    << "  " << gaddr << " " << m_DstLAddr << " "
    << m_LocalN << "  " << m_LocalC << "  " << m_LocalH << "  " << m_LocalW
    << "  " << m_GlobalC << "  " << m_GlobalH << "  " << m_GlobalW << "  "
    << m_DoTranspose << "  " << m_IsAligned << "  " << m_IsNeuron << "\n");

  bmnet::bmnet_asm::asm_context::get_context().name = m_SplitName;
  // TODO(arcbbb): only support 4d tensor for the moment
  bmnet::bmnet_asm::bmnet_tl_load_stride_bmkernel(
      gaddr,         // Src global addr
      m_DstLAddr,    // Dest local addr
      m_LocalN,      // Local N
      m_LocalC,      // Local C
      m_LocalH,      // Local H
      m_LocalW,      // Local W
      m_GlobalC,     // Global C
      m_GlobalH,     // Global H
      m_GlobalW,     // Global W
      m_DoTranspose, // Do Transpose
      m_IsAligned,   // Check alignment
      m_IsNeuron     // MemSpace: Neuron or Weight
  );
}

TLLoad *TLLoad::addMemOperands(MemOperand *pInput)
{
  m_MemOperands.push_back(pInput);
  return this;
}

} // namespace BM188X
} // namespace onnc
