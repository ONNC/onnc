#pragma once
#include <cassert>
#include <onnc/Target/TG/BM188x/asm.pb.h>

namespace onnc {
namespace bm_asm {

template <class PARAM_T> void setDim(PARAM_T *pP, uint32_t pH, uint32_t pW)
{
  pP->set_h(pH);
  pP->set_w(pW);
}
template <class PARAM_T>
void setDim(PARAM_T *pP, uint32_t pN, uint32_t pC, uint32_t pH, uint32_t pW)
{
  pP->set_n(pN);
  pP->set_c(pC);
  pP->set_h(pH);
  pP->set_w(pW);
}
template <class PARAM_T, class DATA_T>
void setMem(PARAM_T *pP, DATA_T *pD, tg::bm1880::Operand::Type pType)
{
  pP->set_addr(pD->m_Addr);
  size_t s = 0;
  switch (pType) {
  case tg::bm1880::Operand::Int8:
    s = 1;
    break;
  case tg::bm1880::Operand::Int16:
    s = 2;
    break;
  default:
    assert(0);
  }
  pP->set_type(pType);
  assert(pD->m_Size == pD->m_Count * s);
  pP->set_count(pD->m_Count);
}
} // namespace bm_asm
} // namespace onnc
