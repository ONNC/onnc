#include "genmem2Nvdla.hpp"

#include <cstdint>
#include <limits>

using namespace genmem;

int genmem::issue_dlaAddressList(onnc::NvDlaBackendMeta& m_pMeta, int genmem_address_entry, const NvDlaMemory& memory)
{
  unsigned mid = memory.genmem_addrress_entry_to_mid(genmem_address_entry);

  return m_pMeta.acquireMemory(mid, memory.get_ale_offset(genmem_address_entry));
}

vector<onnc::NvDlaDlaOperation*> genmem::genmemRegisterToDalOp(onnc::NvDlaBackendMeta&      m_pMeta,
                                                               const std::vector<Register>& regs, NvDlaMemory& memory)
{
  // 1. Allocate memory MLE
  for (auto& mle_info : memory.get_total_mle()) {
    if (mle_info.should_alloc == false)
      continue;

    ILoadable::MemoryListEntry mle;
    assert(m_pMeta.m_MemoryListEntries.size() == mle_info.mid);
    mle.id             = mle_info.mid;
    mle.size           = mle_info.size;
    mle.alignment      = 4096;
    mle.flags          = ILoadable::MemoryFlags_ALLOC;
    mle.domain         = ILoadable::MemoryDomain_SYSMEM;
    mle.bind_id        = 0;
    mle.tensor_desc_id = 0;

    m_pMeta.m_MemoryListEntries.push_back(mle);
    mle_info.should_alloc = false;
  }

  // 2. turn to NvDlaDlaOperation
  vector<onnc::NvDlaDlaOperation*> ops;
  for (const auto& reg : regs) {
    if (reg.rubik_mode != BDMAMODE)
      ops.push_back(rubik_register_to_dla(m_pMeta, reg, memory));
    else
      ops.push_back(bdma_register_to_dla(m_pMeta, reg, memory));
  }
  return ops;
}

onnc::NvDlaDlaOperation* genmem::rubik_register_to_dla(onnc::NvDlaBackendMeta& m_pMeta, const Register& reg,
                                                       const NvDlaMemory& memory)
{
  onnc::NvDlaDlaOperation* op = new onnc::NvDlaDlaOperation();
  op->op_dep.op_type          = DLA_OP_RUBIK;

  struct dla_rubik_op_desc* op_desc = (struct dla_rubik_op_desc*)(&(op->op_desc));

  op_desc->mode      = reg.rubik_mode;
  op_desc->precision = reg.in_precison;
  op_desc->stride_x  = reg.deconv_x_stride;
  op_desc->stride_y  = reg.deconv_y_stride;

  struct dla_rubik_surface_desc* op_surf = (struct dla_rubik_surface_desc*)(&(op->op_surf));
  assert(reg.high_address[genmem::DATAIN] == 0);
  assert(reg.high_address[genmem::DATAOUT] == 0);

  // input cube
  // data type : external memory
  op_surf->src_data.type    = DLA_MEM_MC;
  op_surf->src_data.address = issue_dlaAddressList(m_pMeta, reg.low_address[genmem::DATAIN], memory);
  op_surf->src_data.size =
    m_pMeta.m_MemoryListEntries[memory.genmem_addrress_entry_to_mid(reg.low_address[genmem::DATAIN])].size;

  op_surf->src_data.width   = reg.W;
  op_surf->src_data.height  = reg.H;
  op_surf->src_data.channel = reg.C;

  // We don't use NvDlaCubeInfo to get stride because we may adjust the stride
  // to make RUBIK put data on the right place.
  op_surf->src_data.line_stride  = reg.line_stride[genmem::DATAIN] * genmem::memWidth;
  op_surf->src_data.surf_stride  = reg.surf_stride[genmem::DATAIN] * genmem::memWidth;
  op_surf->src_data.plane_stride = reg.planar_stride[genmem::DATAIN] * genmem::memWidth;

  // output cube
  op_surf->dst_data.type    = DLA_MEM_MC;
  op_surf->dst_data.address = issue_dlaAddressList(m_pMeta, reg.low_address[genmem::DATAOUT], memory);
  op_surf->dst_data.size =
    m_pMeta.m_MemoryListEntries[memory.genmem_addrress_entry_to_mid(reg.low_address[genmem::DATAOUT])].size;

  op_surf->dst_data.width   = reg.W;
  op_surf->dst_data.height  = reg.H;
  op_surf->dst_data.channel = reg.dataout_channel;

  op_surf->dst_data.line_stride  = reg.line_stride[genmem::DATAOUT] * genmem::memWidth;
  op_surf->dst_data.surf_stride  = reg.surf_stride[genmem::DATAOUT] * genmem::memWidth;
  op_surf->dst_data.plane_stride = reg.planar_stride[genmem::DATAOUT] * genmem::memWidth;

  return op;
}

onnc::NvDlaDlaOperation* genmem::bdma_register_to_dla(onnc::NvDlaBackendMeta& m_pMeta, const Register& reg,
                                                      const NvDlaMemory& memory)
{
  onnc::NvDlaDlaOperation* op = new onnc::NvDlaDlaOperation();
  op->op_dep.op_type          = DLA_OP_BDMA;

  struct dla_bdma_op_desc* op_desc = (struct dla_bdma_op_desc*)(&(op->op_desc));
  op_desc->num_transfers           = reg.bdma_transfer.size();

  struct dla_bdma_surface_desc* op_surf = (struct dla_bdma_surface_desc*)(&(op->op_surf));
  op_surf->source_type                  = DLA_MEM_MC;
  op_surf->destination_type             = DLA_MEM_MC;
  op_surf->num_transfers                = reg.bdma_transfer.size();

  for (int i = 0; i < reg.bdma_transfer.size(); i++) {
    BDMATransfer trans                        = reg.bdma_transfer[i];
    op_surf->transfers[i].source_address      = issue_dlaAddressList(m_pMeta, trans.source_entry, memory);
    op_surf->transfers[i].destination_address = issue_dlaAddressList(m_pMeta, trans.destination_entry, memory);
    op_surf->transfers[i].line_size           = trans.line_size;
    op_surf->transfers[i].line_repeat         = trans.line_repeat;

    op_surf->transfers[i].source_line         = trans.source_line;
    op_surf->transfers[i].destination_line    = trans.destination_line;
    op_surf->transfers[i].surface_repeat      = trans.surface_repeat;
    op_surf->transfers[i].source_surface      = trans.source_surface;
    op_surf->transfers[i].destination_surface = trans.destination_surface;
  }

  return op;
}
