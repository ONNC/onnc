#include "TLStore.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

#define DEBUG_TYPE "tl_store"

namespace onnc {
namespace BM188X {
TLStore::TLStore(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("TLStore"))
{
  // ONNC extension attribute
  assert(pNode.hasAttribute(::onnx::Symbol("dst_goffset")));
  assert(pNode.hasAttribute(::onnx::Symbol("src_laddr")));

  assert(pNode.hasAttribute(::onnx::Symbol("local_dim")));
  assert(pNode.hasAttribute(::onnx::Symbol("global_dim")));

  assert(pNode.hasAttribute(::onnx::Symbol("do_transpose")));
  assert(pNode.hasAttribute(::onnx::Symbol("is_aligned")));
  assert(pNode.hasAttribute(::onnx::Symbol("is_neuron")));
  assert(pNode.hasAttribute(::onnx::Symbol("op_name")));
  m_SplitName = pNode.s(::onnx::Symbol("op_name"));

  m_DstGOffset = pNode.i(::onnx::Symbol("dst_goffset"));
  m_SrcLAddr = pNode.i(::onnx::Symbol("src_laddr"));

  m_DoTranspose = pNode.i(::onnx::Symbol("do_transpose"));
  m_IsAligned = pNode.i(::onnx::Symbol("is_aligned"));
  m_IsNeuron = pNode.i(::onnx::Symbol("is_neuron"));

  auto &local_dim = pNode.is(::onnx::Symbol("local_dim"));
  assert(local_dim.size() == 4);
  m_LocalN = local_dim[0];
  m_LocalC = local_dim[1];
  m_LocalH = local_dim[2];
  m_LocalW = local_dim[3];

  auto &global_dim = pNode.is(::onnx::Symbol("global_dim"));
  assert(global_dim.size() == 4);
  m_GlobalC = global_dim[1];
  m_GlobalH = global_dim[2];
  m_GlobalW = global_dim[3];
  // End extension
}

void TLStore::emit() const
{
  // Calculate the address after Global Memory Allocation Pass
  uint64_t gaddr = m_DstGOffset + m_MemOperands[0]->m_Addr;

  DEBUG(print(dbgs()));
  bmnet::bmnet_asm::asm_context::get_context().name = m_SplitName;
  // TODO(arcbbb): only support 4d tensor for the moment
  bmnet::bmnet_asm::bmnet_tl_store_stride_bmkernel(
      gaddr,         // Dest global addr
      m_SrcLAddr,    // Src  local addr
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

TLStore *TLStore::addMemOperands(MemOperand *pOutput)
{
  m_MemOperands.push_back(pOutput);
  return this;
}

void TLStore::print(OStream &pOs) const
{
  pOs << m_DstGOffset << "<" << m_LocalN << "," << m_LocalC << "," << m_LocalH
      << "," << m_LocalW << "> = TLStore <src:" << m_SrcLAddr << "> (<"
      << m_GlobalC << "," << m_GlobalH << "," << m_GlobalW << ">)\n";
  ;
}
} // namespace BM188X
} // namespace onnc
