#ifndef GENMEM_GENMEM_2_NVDLA_H
#define GENMEM_GENMEM_2_NVDLA_H

#include "../NvDlaMeta.h"
#include "NvdlaMemory.hpp"
#include "genmem/Register.hpp"

namespace genmem {

vector<onnc::NvDlaDlaOperation*> genmemRegisterToDalOp(onnc::NvDlaBackendMeta&      m_pMeta,
                                                       const std::vector<Register>& regs, NvDlaMemory& memory);

int issue_dlaAddressList(onnc::NvDlaBackendMeta& m_pMeta, int genmem_address_entry, const NvDlaMemory& memory);

onnc::NvDlaDlaOperation* rubik_register_to_dla(onnc::NvDlaBackendMeta& m_pMeta, const Register& reg,
                                               const NvDlaMemory& mem);

onnc::NvDlaDlaOperation* bdma_register_to_dla(onnc::NvDlaBackendMeta& m_pMeta, const Register& reg,
                                              const NvDlaMemory& mem);
} // namespace genmem

#endif
