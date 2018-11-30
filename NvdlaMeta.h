#ifndef _NVDLA_META_H_
#define _NVDLA_META_H_

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <onnc/IR/Compute/Tensor.h>

#include "priv/Loadable.h"
#include "priv/loadable_generated.h"
#include "nvdla/ILoadable.h"

#include "dla_interface.h"
#include "emu_interface.h"

#include "fp16.h"

#ifdef DEBUG
#define NVDLA_DBG printf
#else
#define NVDLA_DBG
#endif

using namespace nvdla;
using namespace nvdla::priv;

#define FEATURE_ATOM_CUBE_SIZE  32
#define WEIGHT_ATOM_CUBE_SIZE  128

namespace onnc {
  typedef struct concat_meta {
    const Tensor *t;
    int ofs;
  } concat_meta;

  //typedef std::unordered_map<const Value *, float *> WeightTable;
  typedef std::unordered_map<Value *, int> MemoryIdxTable;
  typedef std::unordered_map<const Tensor *, const Tensor *> RemapTable;
  typedef std::unordered_map<const Tensor *, concat_meta> ConcatTable;

  typedef struct NvdlaDlaOperation{
    NvdlaDlaOperation(void){
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
    struct dla_common_op_desc op_dep;
    union dla_operation_container op_desc;
    union dla_surface_container op_surf;
  } NvdlaDlaOperation;

  typedef struct NvdlaEmuOperation{
    NvdlaEmuOperation(void){
      memset(&op_desc, 0, sizeof(op_desc));
      memset(&op_buf, 0, sizeof(op_buf));
    }
    union emu_operation_container op_desc;
    union emu_operation_buffer_container op_buf;
  } NvdlaEmuOperation;

  typedef struct NvdlaBackendMeta{
    NvdlaBackendMeta(void){
      m_NumLUTs = 0;
      m_NumBlobs = 0;
      m_Loadable = priv::LoadableFactory::newLoadable();

      m_pPrevOp = NULL;
      for(int i = 0; i < DLA_OP_NUM; i++){
        m_pDepOp[i] = NULL;
        m_DlaNetworkDesc.op_head[i] = -1;
      }

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

    ~NvdlaBackendMeta(void){
      std::map<std::string, Loadable::Symbol>::iterator it;
      for (it = m_Symbols.begin(); it != m_Symbols.end(); it++) {
          Loadable::Symbol symbol = it->second;
          if (symbol.data != NULL)
              delete[] symbol.data;
      }

      std::vector<NvdlaDlaOperation*>::iterator op_dla;
      for (op_dla = m_DLAOperationList.begin(); op_dla != m_DLAOperationList.end(); op_dla++) {
          NvdlaDlaOperation *op = *op_dla;
          NVDLA_DBG("op DLA release - %p\n", op);
          delete op;
      }

      std::vector<NvdlaEmuOperation*>::iterator op_emu;
      for (op_emu = m_EMUOperationList.begin(); op_emu != m_EMUOperationList.end(); op_emu++) {
          NvdlaEmuOperation *op = *op_emu;
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
    // memory allocation information for runtime (firmwares, memory buffer)
    std::vector<ILoadable::MemoryListEntry> m_MemoryListEntries;
    // addresses used in firmware
    std::vector<ILoadable::AddressListEntry> m_AddressListEntries;
    // input, output specific descriptor
    std::vector<ILoadable::TensorDescListEntry> m_TensorDescListEntries;
    // relocation information of input/output
    std::vector<ILoadable::RelocEntry> m_RelocEntries;

    // blobs, firmware binary (operators, initializer data)
    std::map<std::string, Loadable::Symbol> m_Symbols;
    // DLA or EMU batch tasks
    std::vector<ILoadable::TaskListEntry> m_TaskListEntries;
    // batch task submit order
    std::vector<ILoadable::SubmitListEntry> m_SubmitListEntries;
    // events between submits
    std::vector<ILoadable::EventListEntry> m_EventListEntries;

    int m_DlaAddresses;
    struct dla_network_desc  m_DlaNetworkDesc;
    //struct dla_lut_param m_LrnDefaultLutParam;
    int m_NumLUTs;
    std::vector<NvdlaDlaOperation *> m_DLAOperationList;
    std::vector<dla_lut_param *> m_LUTList;
    NvdlaDlaOperation *m_pDepOp[DLA_OP_NUM];
    NvdlaDlaOperation *m_pPrevOp;

    emu_network_desc  m_EmuNetworkDesc;
    std::vector<NvdlaEmuOperation *> m_EMUOperationList;

    MemoryIdxTable m_MemIdxTable;
    RemapTable m_ReshapeTable;
    ConcatTable m_ConcatTable;

    int m_NumBlobs;
    priv::LoadableFactory::LoadablePrivPair m_Loadable;
  } NvdlaBackendMeta;

  typedef enum {
    NVDLA_CUBE_FEATURE,
    NVDLA_CUBE_WEIGHT
  } nvdla_cube_type;

  class NvdlaCubeInfo {
  public:
    NvdlaCubeInfo(nvdla_cube_type m, int n, int c, int h, int w, int es){
      element_size = es;
      dim_n = n;
      dim_c = c;
      dim_h = h;
      dim_w = w;

      mode = m;
      reduced = false;
      switch(mode){
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
            switch((atom_per_channel % 4)){
              case 3:
                entry_per_slice += dim_w;
                break;
              case 2:
                entry_per_slice += (dim_w + 1)/2;
                break;
              case 1:
                entry_per_slice += (dim_w + 3)/4;
                break;
            }
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
          if(banks > 16){
            banks = (16 * dim_c * dim_h * dim_w * element_size * 2  + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
            if(banks > 16)
              banks /= 2;
            reduced = true;
          }
          break;
      }
    }

    int getReducedBanks(void){
      switch(mode){
        case NVDLA_CUBE_FEATURE:
          return banks;
        case NVDLA_CUBE_WEIGHT:
          {
            int rbanks = (16 * dim_c * dim_h * dim_w * element_size * 2  + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
            if(reduced){
              rbanks /= 2;
            }
            return rbanks;
          }
      }
    }

    void reduceBanks(void){
      switch(mode){
        case NVDLA_CUBE_FEATURE:
          break;
        case NVDLA_CUBE_WEIGHT:
          banks = (16 * dim_c * dim_h * dim_w * element_size * 2 + (256*WEIGHT_ATOM_CUBE_SIZE-1))/ (256*WEIGHT_ATOM_CUBE_SIZE);
          if(reduced)
            banks /= 2;
          break;
      }
      reduced = true;
    }

    ~NvdlaCubeInfo() {};

    nvdla_cube_type mode;
    int element_size;
    int dim_n;
    int dim_c;
    int dim_h;
    int dim_w;
    int eps;
    int banks;
    int size;
    int stride_channel;
    int stride_line;
    int stride_surface;
    int stride_plane;
    bool reduced;
  };
}

#endif
