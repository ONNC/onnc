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

  m_DstGOffset = pNode.i(::onnx::Symbol("dst_goffset"));
  m_SrcLAddr = pNode.i(::onnx::Symbol("src_laddr"));

  m_DoTranspose = pNode.i(::onnx::Symbol("do_transpose"));

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
  DEBUG(dbgs() << m_DstGOffset << "<" << m_LocalN << "," << m_LocalC << ","
               << m_LocalH << "," << m_LocalW
               << "> = TLStore <src:" << m_SrcLAddr << "> (<" << m_GlobalC
               << "," << m_GlobalH << "," << m_GlobalW << ">)\n";);
}

void TLStore::emit() const {}

TLStore *TLStore::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
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
