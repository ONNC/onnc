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
  constexpr std::size_t size(const T(&)[N]) noexcept {
    return N;
  }

  template <typename T>
  void clear_bytes(T& object) {
    static_assert(std::is_trivial<T>::value);
    std::memset(std::addressof(object), 0, sizeof(object));
  }
} // namespace detail
} // namespace onnc

//===----------------------------------------------------------------------===//
// NvDlaDlaOperation
//===----------------------------------------------------------------------===//
NvDlaDlaOperation::NvDlaDlaOperation()
{
    // XXX: C++ is zero-initialized, so we need no memset(0).
    // TODO: use initialized member list.
    memset(&op_dep, 0, sizeof(op_dep));
    memset(&op_desc, 0, sizeof(op_desc));
    memset(&op_surf, 0, sizeof(op_surf));
    for(int i = 0; i < DLA_OP_NUM; i++){
      op_dep.consumers[i].index = -1;
      op_dep.consumers[i].event = 1;
    }
    op_dep.fused_parent.index = -1;
    op_dep.fused_parent.event = 1;
}

//===----------------------------------------------------------------------===//
// NvDlaEmuOperation
//===----------------------------------------------------------------------===//
NvDlaEmuOperation::NvDlaEmuOperation()
{
  // TODO: use initialized member list.
  memset(&op_desc, 0, sizeof(op_desc));
  memset(&op_buf, 0, sizeof(op_buf));
}

//===----------------------------------------------------------------------===//
// NvDlaBackendMeta
//===----------------------------------------------------------------------===//
NvDlaBackendMeta::NvDlaBackendMeta()
  : m_NumLUTs(0), m_NumBlobs(0), m_pPrevOp(nullptr) {

  using namespace detail;
  using std::begin;
  using std::end;

  m_Loadable = priv::LoadableFactory::newLoadable();

  std::fill(begin(m_pDepOp), end(m_pDepOp), nullptr);

  clear_bytes(m_DlaNetworkDesc);
  for (std::size_t idx = 0; idx < size(m_DlaNetworkDesc.op_head); ++idx) {
    m_DlaNetworkDesc.op_head[idx] = -1;
  }
  clear_bytes(m_EmuNetworkDesc);

  {
    struct dla_lut_param *default_lut_param = new dla_lut_param();
    memset(default_lut_param, 0, sizeof(struct dla_lut_param));
    default_lut_param->linear_only_offset.frac_bits = -128;
    default_lut_param->linear_only_start = 1;
    default_lut_param->linear_only_end = 1;

    float alpha = 0.0001f;
    float beta = 0.75f;
    float bias = 1.0f;
    float size = 5.0f;
    float x = 1.0f;
    for(int i = 0; i < 65; i++){
      default_lut_param->linear_exp_table[i] = __gnu_f2h_ieee(1.0f/pow((bias + (alpha*x/size)),beta));
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
    NvDlaDlaOperation *op = *op_dla;
    NVDLA_DBG("op DLA release - %p\n", op);
    delete op;
  }

  std::vector<NvDlaEmuOperation*>::iterator op_emu;
  for (op_emu = m_EMUOperationList.begin(); op_emu != m_EMUOperationList.end(); op_emu++) {
    NvDlaEmuOperation *op = *op_emu;
    NVDLA_DBG("op EMU release - %p\n", op);
    delete op;
  }

  std::vector<struct dla_lut_param*>::iterator lut_param;
  for (lut_param = m_LUTList.begin(); lut_param != m_LUTList.end(); lut_param++) {
    struct dla_lut_param *lut = *lut_param;
    NVDLA_DBG("LUT release - %p\n", lut);
    delete lut;
  }

  m_MemoryListEntries.clear();
  m_TaskListEntries.clear();
  m_SubmitListEntries.clear();
  m_EventListEntries.clear();
  m_AddressListEntries.clear();
  m_TensorDescListEntries.clear();
  m_RelocEntries.clear();
  m_NumBlobs = 0;

  //TODO: clear m_DLAOperationList;
}

//===----------------------------------------------------------------------===//
// NvDlaCubeInfo
//===----------------------------------------------------------------------===//
NvDlaCubeInfo::NvDlaCubeInfo(nvdla_cube_type m, int n, int c, int h, int w, int es)
  : mode(m), element_size(es), dim_n(n), dim_c(c), dim_h(h), dim_w(w),
    reduced(false) {
  switch(mode) {
    case NVDLA_CUBE_FEATURE:
      stride_channel = element_size;
      stride_line = dim_w*FEATURE_ATOM_CUBE_SIZE;
      stride_surface = dim_h*dim_w*FEATURE_ATOM_CUBE_SIZE;
      stride_plane = 0;

      {
        int atom_c = FEATURE_ATOM_CUBE_SIZE/element_size;
        int fixed_c = (dim_c + (atom_c-1)) & ~(atom_c-1);
        size = dim_n * fixed_c * dim_h * dim_w * element_size;
        NVDLA_DBG("%d %d/%d %d %d %d\n", dim_n, dim_c, fixed_c, dim_h, dim_w, element_size);
      }

      {
        int atom_per_channel = (dim_c * element_size + FEATURE_ATOM_CUBE_SIZE - 1) / FEATURE_ATOM_CUBE_SIZE;
        int entry_per_slice = (atom_per_channel / 4) * dim_w;
        switch((atom_per_channel % 4)) {
          case 3:
            entry_per_slice += dim_w;
            break;
          case 2:
            entry_per_slice += (dim_w + 1)/2;
            break;
          case 1:
            entry_per_slice += (dim_w + 3)/4;
            break;
        } // end of siwtch
        eps = entry_per_slice;
      }
      banks = ((eps * dim_h) + 255)/ 256;
      break;
    case NVDLA_CUBE_WEIGHT:
      size = (dim_n * dim_c * dim_h * dim_w * element_size);
      eps = 0;
      stride_channel = element_size;
      stride_line = dim_n * dim_w * WEIGHT_ATOM_CUBE_SIZE;
      stride_surface = dim_n * dim_h * dim_h * WEIGHT_ATOM_CUBE_SIZE;
      banks = (dim_n * dim_c * dim_h * dim_w * element_size + 128 + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
      if(banks > 16) {
        banks = (16 * dim_c * dim_h * dim_w * element_size * 2  + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
        if(banks > 16)
          banks = (16 * dim_c * dim_h * dim_w * element_size  + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
        reduced = true;
      }
      break;
  } // end of switch
}

int NvDlaCubeInfo::getReducedBanks() const
{
  switch(mode) {
    case NVDLA_CUBE_FEATURE:
      return banks;
    case NVDLA_CUBE_WEIGHT: {
      int rbanks = (16 * dim_c * dim_h * dim_w * element_size * 2  + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
      if(reduced) {
        rbanks = (16 * dim_c * dim_h * dim_w * element_size  + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
      }
      return rbanks;
    }
  } // end of switch
  return 0; //< is the default value correct?
}

void NvDlaCubeInfo::reduceBanks()
{
  switch(mode) {
    case NVDLA_CUBE_FEATURE:
      break;
    case NVDLA_CUBE_WEIGHT:
      banks = (16 * dim_c * dim_h * dim_w * element_size * 2 + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
      if(reduced)
        banks = (16 * dim_c * dim_h * dim_w * element_size + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
      break;
  } // end of switch
  reduced = true;
}
