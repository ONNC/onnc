#define DEBUG_TYPE "tg_concat"
#include "TGConcat.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGConcat::TGConcat(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Concat"))
{
  m_ConcatAxis = pNode.i(::onnx::Symbol("axis"));
  for (size_t i = 0; i < pNode.inputs().size(); i++) {
    const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[i]->sizes();
    m_InputDims.push_back(inDim[m_ConcatAxis].dim);
  }

  for (size_t i = 0; i < pNode.outputs()[0]->sizes().size(); i++) {
    const std::vector< ::onnx::Dimension> outDim = pNode.outputs()[0]->sizes();
    m_OutputDim.push_back(outDim[i].dim);
  }
}

TGConcat *TGConcat::addMemOperands(std::vector<MemOperand *> &pInput,
                                   MemOperand *pOutput)
{
  for (size_t i = 0; i < pInput.size(); i++) {
    m_MemOperands.push_back(pInput[i]);
    m_InputAddr.push_back(pInput[i]->m_Addr);
  }
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGConcat::print(OStream &pOS) const
{
  pOS << *(m_MemOperands.back()) << " = Concat <axis:" << m_ConcatAxis << "> (";
  size_t i = 0;
  for (; i < m_MemOperands.size() - 2; i++) {
    pOS << *m_MemOperands[i] << ", ";
  }
  pOS << *m_MemOperands[i] << ")\n";
}

void TGConcat::emit() const
{
  DEBUG(print(dbgs()));
  // Need to modify the api to use const.
  std::vector<uint64_t> input_addr;
  for (size_t i = 0; i < m_InputDims.size(); i++)
    input_addr.push_back(m_MemOperands[i]->m_Addr);

  bmnet::bmnet_asm::bmnet_concat_fixed_forward_bmkernel(
      input_addr.data(), m_MemOperands[1]->m_Addr,
      const_cast<int *>(m_InputDims.data()), m_InputDims.size(), m_ConcatAxis,
      m_OutputDim.size(), const_cast<int *>(m_OutputDim.data()));
}

} // namespace BM188X
} // namespace onnc
