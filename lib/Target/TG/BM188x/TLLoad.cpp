#include "TLLoad.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <bmnet/targets/plat-bm188x/bmkernel/bmkernel_api.h>
#include <onnc/Support/Debug.h>

#define DEBUG_TYPE "tl_load"

namespace onnc {
namespace BM188X {
TLLoad::TLLoad(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("TLLoad"))
{
  // ONNC extension attribute
  assert(pNode.hasAttribute(::onnx::Symbol("src_goffset")));
  assert(pNode.hasAttribute(::onnx::Symbol("dst_laddr")));

  assert(pNode.hasAttribute(::onnx::Symbol("local_dim")));
  assert(pNode.hasAttribute(::onnx::Symbol("global_dim")));

  assert(pNode.hasAttribute(::onnx::Symbol("do_transpose")));

  m_SrcGOffset = pNode.i(::onnx::Symbol("src_goffset"));
  m_DstLAddr = pNode.i(::onnx::Symbol("dst_laddr"));

  m_DoTranspose = pNode.i(::onnx::Symbol("do_transpose"));

  auto &local_dim = pNode.is(::onnx::Symbol("local_dim"));
  assert(local_dim.size() == 4);
  m_LocalN = local_dim[0];
  m_LocalC = local_dim[1];
  m_LocalH = local_dim[2];
  m_LocalW = local_dim[3];

  auto &global_dim = pNode.is(::onnx::Symbol("global_dim"));
  assert(global_dim.size() == 4);
  m_GlobalC = global_dim[0];
  m_GlobalH = global_dim[1];
  m_GlobalW = global_dim[2];
  // End extension
  DEBUG(dbgs() << "tl_load: src:" << m_SrcGOffset << ", dst:" << m_DstLAddr
               << ",<" << m_LocalN << "," << m_LocalC << "," << m_LocalH << ","
               << m_LocalW << ">,<" << m_GlobalC << "," << m_GlobalH << ","
               << m_GlobalW << ">\n";);
}

void TLLoad::emit() const {}

TLLoad *TLLoad::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TLLoad::print(OStream &pOS) const {}
} // namespace BM188X
} // namespace onnc
