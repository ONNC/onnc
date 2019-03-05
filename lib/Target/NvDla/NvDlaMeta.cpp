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
NvDlaCubeInfo::NvDlaCubeInfo(nvdla_cube_type m, int n, int c, int h, int w)
  : mode(m), dim_n(n), dim_c(c), dim_h(h), dim_w(w),
    reduced(false) {

  switch(mode) {
    case NVDLA_CUBE_FEATURE:
      stride_channel = ELEMENT_SIZE;
      stride_line = dim_w * FEATURE_ATOM_CUBE_SIZE;
      stride_surface = dim_h * dim_w * FEATURE_ATOM_CUBE_SIZE;
      stride_plane = 0;

      {
        int atom_c = FEATURE_ATOM_CUBE_SIZE / ELEMENT_SIZE;
        int fixed_c = (dim_c + (atom_c-1)) & ~(atom_c-1);

        size = dim_n * fixed_c * dim_h * dim_w * ELEMENT_SIZE;

        int segment = CBUF_BANK_WIDTH * ELEMENT_SIZE / FEATURE_ATOM_CUBE_SIZE;
        int num_of_segment = dim_c / (atom_c * segment);
        int entry_per_slice =
            (dim_w * atom_c * segment * num_of_segment / CBUF_BANK_WIDTH) +
            (dim_w * atom_c * segment * ((dim_c % (atom_c * segment)) != 0) / CBUF_BANK_WIDTH)
        eps = entry_per_slice;
      }
      banks = ((eps * dim_h) + CBUF_BANK_DEPTH - 1)/ CBUF_BANK_DEPTH;
      break;

#if HAS_IMAGE_MODE
    case NVDLA_CUBE_IMAGE:
      // ITRI advice:
      // -- image mode needs 4-channel input;
      // -- with 3-channel RGB image, create a 4th channel whose data & weight are all zero.
      // FIXME: for now, pretend it's 4-channel, reardless of whether it's the first model layer.
      if (dim_c == 3) {
        dim_c = 4;
      }
      stride_channel = ELEMENT_SIZE;
      stride_line = (dim_w * dim_c * ELEMENT_SIZE + (FEATURE_ATOM_CUBE_SIZE-1)) / FEATURE_ATOM_CUBE_SIZE * FEATURE_ATOM_CUBE_SIZE;
      stride_surface = dim_h * stride_line;
      stride_plane = 0;

      {
        //int atom_c = FEATURE_ATOM_CUBE_SIZE/ELEMENT_SIZE;
        //int fixed_c = (dim_c + (atom_c-1)) & ~(atom_c-1);
        size = stride_surface; //dim_n * dim_c * dim_h * dim_w * ELEMENT_SIZE;
        //NVDLA_DBG("%d %d/%d %d %d %d\n", dim_n, dim_c, fixed_c, dim_h, dim_w, ELEMENT_SIZE);
      }
      // FIXME: for now, use image mode if 3-channel, reardless of whether it's the first model layer.
      if (dim_c == 3 || dim_c == 4) {
         // sample direct mode: eps = dim_w(224) * dim_c_inflated(8) / CBUF_BANK_WIDTH(8) = 224
         // sample image mode : eps = dim_w(224) * dim_c_actual(4)   / CBUF_BANK_WIDTH(8) = 112
         eps = dim_w * dim_c / CBUF_BANK_WIDTH;
      }
      banks = ((eps * dim_h) + CBUF_BANK_DEPTH - 1)/ CBUF_BANK_DEPTH;
      break;
#endif // HAS_IMAGE_MODE


  case NVDLA_CUBE_WEIGHT:
      size = (dim_n * dim_c * dim_h * dim_w * ELEMENT_SIZE);//FIXME: why not need padding?
      eps = 0;
      stride_channel = ELEMENT_SIZE;
      stride_line = dim_n * dim_w * WEIGHT_ATOM_CUBE_SIZE;
      stride_surface = dim_n * dim_h * dim_h * WEIGHT_ATOM_CUBE_SIZE;
      //FIXME:                                                 V-- Why needs to multiply this?
      banks = (dim_n * dim_c * dim_h * dim_w * ELEMENT_SIZE + WEIGHT_ATOM_CUBE_SIZE + (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE-1))/ (CBUF_BANK_DEPTH * WEIGHT_ATOM_CUBE_SIZE);
      // Must  at least one bank for data.
      if(banks > (CBUF_BANK_NUM - 1)) {
        banks = (MAC_ATOMIC_K * dim_c * dim_h * dim_w * ELEMENT_SIZE * 2  + (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE-1))/ (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE);
        if(banks > (CBUF_BANK_NUM - 1)) {
          banks = (MAC_ATOMIC_K * dim_c * dim_h * dim_w * ELEMENT_SIZE + (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE-1))/ (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE);
        }
        reduced = true;//FIXME: Not clear what `reduce` means? 
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
      int rbanks = (MAC_ATOMIC_K * dim_c * dim_h * dim_w * ELEMENT_SIZE * 2  + (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE-1))/ (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE);
      if(reduced) {
        rbanks = (MAC_ATOMIC_K * dim_c * dim_h * dim_w * ELEMENT_SIZE  + (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE-1))/ (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE);
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
      banks = (MAC_ATOMIC_K * dim_c * dim_h * dim_w * ELEMENT_SIZE * 2 + (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE-1))/ (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE);
      if(reduced) {
        banks = (MAC_ATOMIC_K * dim_c * dim_h * dim_w * ELEMENT_SIZE + (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE-1))/ (CBUF_BANK_DEPTH*WEIGHT_ATOM_CUBE_SIZE);
      }
      break;
  } // end of switch
  reduced = true;
}
