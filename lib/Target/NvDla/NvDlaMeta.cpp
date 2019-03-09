//===- NVDLAMeta.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaMeta.h"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>

using namespace onnc;

namespace onnc {
namespace detail {
template <typename T, std::size_t N>
constexpr std::size_t size(const T (&)[N]) noexcept
{
  return N;
}
} // namespace detail
} // namespace onnc

//===----------------------------------------------------------------------===//
// NvDlaDlaOperation
//===----------------------------------------------------------------------===//
NvDlaDlaOperation::NvDlaDlaOperation() noexcept
  : op_dep{}
  , op_desc{}
  , op_surf{}
{
  for (int i = 0; i < DLA_OP_NUM; i++) {
    op_dep.consumers[i].index = -1;
    op_dep.consumers[i].event = 1;
  }
  op_dep.fused_parent.index = -1;
  op_dep.fused_parent.event = 1;
}

//===----------------------------------------------------------------------===//
// NvDlaEmuOperation
//===----------------------------------------------------------------------===//
NvDlaEmuOperation::NvDlaEmuOperation() noexcept
  : op_desc{}
  , op_buf{}
{}

//===----------------------------------------------------------------------===//
// NvDlaBackendMeta
//===----------------------------------------------------------------------===//
NvDlaBackendMeta::NvDlaBackendMeta()
  : m_DlaNetworkDesc{}
  , m_NumLUTs{0}
  , m_pPrevOp{nullptr}
  , m_EmuNetworkDesc{}
  , m_NumBlobs{0}
  , m_Loadable{priv::LoadableFactory::newLoadable()}
{
  using namespace detail;
  using std::begin;
  using std::end;

  std::fill(begin(m_pDepOp), end(m_pDepOp), nullptr);

  for (std::size_t idx = 0; idx < size(m_DlaNetworkDesc.op_head); ++idx) {
    m_DlaNetworkDesc.op_head[idx] = -1;
  }

  {
    struct dla_lut_param* default_lut_param = new dla_lut_param();
    std::memset(default_lut_param, 0, sizeof(*default_lut_param));
    default_lut_param->linear_only_offset.frac_bits = -128;
    default_lut_param->linear_only_start            = 1;
    default_lut_param->linear_only_end              = 1;

    float alpha = 0.0001f;
    float beta  = 0.75f;
    float bias  = 1.0f;
    float size  = 5.0f;
    float x     = 1.0f;
    for (int i = 0; i < 65; i++) {
      default_lut_param->linear_exp_table[i] = __gnu_f2h_ieee(1.0f / pow((bias + (alpha * x / size)), beta));
      x *= 2.0f;
    }

    m_LUTList.push_back(default_lut_param);
  }
}

NvDlaBackendMeta::~NvDlaBackendMeta()
{
  std::map<std::string, Loadable::Symbol>::iterator it;
  for (it = m_Symbols.begin(); it != m_Symbols.end(); it++) {
    Loadable::Symbol symbol = it->second;
    if (symbol.data != NULL)
      delete[] symbol.data;
  }

  std::vector<NvDlaDlaOperation*>::iterator op_dla;
  for (op_dla = m_DLAOperationList.begin(); op_dla != m_DLAOperationList.end(); op_dla++) {
    NvDlaDlaOperation* op = *op_dla;
    NVDLA_DBG("op DLA release - %p\n", op);
    delete op;
  }

  std::vector<NvDlaEmuOperation*>::iterator op_emu;
  for (op_emu = m_EMUOperationList.begin(); op_emu != m_EMUOperationList.end(); op_emu++) {
    NvDlaEmuOperation* op = *op_emu;
    NVDLA_DBG("op EMU release - %p\n", op);
    delete op;
  }

  std::vector<struct dla_lut_param*>::iterator lut_param;
  for (lut_param = m_LUTList.begin(); lut_param != m_LUTList.end(); lut_param++) {
    struct dla_lut_param* lut = *lut_param;
    NVDLA_DBG("LUT release - %p\n", lut);
    delete lut;
  }
}

#define ELEMENT_SIZE 2
#define CBUF_BANK_WIDTH 64
#define CBUF_BANK_DEPTH 256
#define UNIT_ALIGNMENT(x, unit) (((x) + ((unit)-1)) & ~((unit)-1))
#define DIV_ROUNDUP(x, dividor) ((x) + (dividor)-1) / (dividor)

//===----------------------------------------------------------------------===//
// NvDlaCubeInfo
//===----------------------------------------------------------------------===//
NvDlaCubeInfo::NvDlaCubeInfo(nvdla_cube_type m, int n, int c, int h, int w, int es)
  : mode(m)
  , element_size(es)
  , dim_n(n)
  , dim_c(c)
  , dim_h(h)
  , dim_w(w)
  , reduced(false)
{
  switch (mode) {
  case NVDLA_CUBE_FEATURE:
    stride_channel = element_size;
    stride_line    = dim_w * FEATURE_ATOM_CUBE_SIZE;
    stride_surface = dim_h * dim_w * FEATURE_ATOM_CUBE_SIZE;
    stride_plane   = 0;
    {
      int atom_c    = FEATURE_ATOM_CUBE_SIZE / ELEMENT_SIZE;
      int segment_c = UNIT_ALIGNMENT(dim_c, atom_c);
      size          = dim_n * segment_c * dim_h * dim_w * ELEMENT_SIZE;

      // For unknown reason, the following case needs to be handled as an exception:
      // For dim_c = 64*a +  b, where a is integer and b is between 33-48 for the nv_full case,
      // a full CBUF entry needs to be allocated to this unaligned feature cube.
      // Although this works for nv_full and nv_small, this logic needs to be verified for other spec file.

      int entry_per_slice = (((segment_c / atom_c) % (CBUF_BANK_WIDTH / atom_c)) == ((CBUF_BANK_WIDTH / atom_c) - 1))
                              ? DIV_ROUNDUP(dim_w * UNIT_ALIGNMENT(segment_c, CBUF_BANK_WIDTH), CBUF_BANK_WIDTH)
                              : DIV_ROUNDUP((dim_w * segment_c), CBUF_BANK_WIDTH);
      eps = entry_per_slice;
      NVDLA_DBG("Cube_Info %d %d/%d %d %d %d %d\n", dim_n, dim_c, segment_c, dim_h, dim_w, ELEMENT_SIZE, eps);
    }
    banks = DIV_ROUNDUP((eps * dim_h), CBUF_BANK_DEPTH);
    break;
  case NVDLA_CUBE_WEIGHT:
    size           = (dim_n * dim_c * dim_h * dim_w * element_size);
    eps            = 0;
    stride_channel = element_size;
    stride_line    = dim_n * dim_w * WEIGHT_ATOM_CUBE_SIZE;
    stride_surface = dim_n * dim_h * dim_h * WEIGHT_ATOM_CUBE_SIZE;
    banks = (dim_n * dim_c * dim_h * dim_w * element_size + 128 + (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE - 1)) /
            (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE);
    if (banks > 16) {
      banks = (16 * dim_c * dim_h * dim_w * element_size * 2 + (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE - 1)) /
              (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE);
      if (banks > 16)
        banks = (16 * dim_c * dim_h * dim_w * element_size + (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE - 1)) /
                (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE);
      reduced = true;
    }
    break;
  } // end of switch
}

int NvDlaCubeInfo::getReducedBanks() const
{
  switch (mode) {
  case NVDLA_CUBE_FEATURE:
    return banks;
  case NVDLA_CUBE_WEIGHT: {
    int rbanks = (16 * dim_c * dim_h * dim_w * element_size * 2 + (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE - 1)) /
                 (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE);
    if (reduced) {
      rbanks = (16 * dim_c * dim_h * dim_w * element_size + (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE - 1)) /
               (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE);
    }
    return rbanks;
  }
  }         // end of switch
  return 0; //< is the default value correct?
}

void NvDlaCubeInfo::reduceBanks()
{
  switch (mode) {
  case NVDLA_CUBE_FEATURE:
    break;
  case NVDLA_CUBE_WEIGHT:
    banks = (16 * dim_c * dim_h * dim_w * element_size * 2 + (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE - 1)) /
            (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE);
    if (reduced)
      banks = (16 * dim_c * dim_h * dim_w * element_size + (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE - 1)) /
              (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE);
    break;
  } // end of switch
  reduced = true;
}
