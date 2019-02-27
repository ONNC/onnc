//===- NvDlaMeta.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_META_H
#define TARGET_NVDLA_NVDLA_META_H

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

//#ifndef NDEBUG
#define NVDLA_DBG printf
//#else
//#define NVDLA_DBG
//#endif

using namespace nvdla;
using namespace nvdla::priv;

// ------------------------------

#define DLA_TYPE_INT8
//#define DLA_TYPE_INT16
//#define DLA_TYPE_FP16

#ifdef DLA_TYPE_INT8
    #define DLA_PRECISION PRECISION_INT8
#endif
#ifdef DLA_TYPE_INT16
    #define DLA_PRECISION PRECISION_INT16
#endif
#ifdef DLA_TYPE_FP16
    #define DLA_PRECISION PRECISION_FP16
#endif

#define HAS_IMAGE_MODE 1

// FIXME: use macro or variable?
// FIXME: currently duplicated declaration at fp16.c & NvDlaMeta.h
#if 1
    // nv_small
    #define FEATURE_ATOM_CUBE_SIZE  8
    #define WEIGHT_ATOM_CUBE_SIZE   8

    #define ELEMENT_SIZE       1
    #define MAC_ATOMIC_C       8
    #define MAC_ATOMIC_K       8
    #define CBUF_BANK_NUM     32
    #define CBUF_BANK_WIDTH    8
    #define CBUF_BANK_DEPTH  512

    typedef char nv_weight_t;
#else
    // nv_full
    #define FEATURE_ATOM_CUBE_SIZE   32
    #define WEIGHT_ATOM_CUBE_SIZE   128

    #define ELEMENT_SIZE       2
    #define MAC_ATOMIC_C      64
    #define MAC_ATOMIC_K      16
    #define CBUF_BANK_NUM     16
    #define CBUF_BANK_WIDTH   64
    #define CBUF_BANK_DEPTH  256

    typedef short nv_weight_t;
#endif

// ------------------------------

namespace onnc {

struct concat_meta {
  const Tensor *t;
  int ofs;
};

//typedef std::unordered_map<const Value *, float *> WeightTable;
typedef std::unordered_map<Value *, int> MemoryIdxTable;
typedef std::unordered_map<const Tensor *, const Tensor *> RemapTable;
typedef std::unordered_map<const Tensor *, concat_meta> ConcatTable;

/** \class NvDlaDlaOperation
 *  \brief
 */
class NvDlaDlaOperation
{
public:
  struct dla_common_op_desc op_dep;

public:
  NvDlaDlaOperation();

public:
  union dla_operation_container op_desc;
  union dla_surface_container op_surf;
};

/** \class NvDlaEmuOperation
 *  \brief
 */
class NvDlaEmuOperation
{
public:
  NvDlaEmuOperation();

public:
  union emu_operation_container op_desc;
  union emu_operation_buffer_container op_buf;
};

class NvDlaBackendMeta
{
public:
  NvDlaBackendMeta();

  ~NvDlaBackendMeta();

public:
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
  std::vector<NvDlaDlaOperation *> m_DLAOperationList;
  std::vector<dla_lut_param *> m_LUTList;
  NvDlaDlaOperation *m_pDepOp[DLA_OP_NUM];
  NvDlaDlaOperation *m_pPrevOp;

  emu_network_desc  m_EmuNetworkDesc;
  std::vector<NvDlaEmuOperation *> m_EMUOperationList;

  MemoryIdxTable m_MemIdxTable;
  RemapTable m_ReshapeTable;
  ConcatTable m_ConcatTable;

  int m_NumBlobs;
  priv::LoadableFactory::LoadablePrivPair m_Loadable;
};

enum nvdla_cube_type {
  NVDLA_CUBE_FEATURE,
  NVDLA_CUBE_WEIGHT
};

class NvDlaCubeInfo
{
public:
  NvDlaCubeInfo(nvdla_cube_type m, int n, int c, int h, int w, int es);

  int getReducedBanks() const;

  void reduceBanks();

  ~NvDlaCubeInfo() { /* do nothing*/ }

public:
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

} // namespace of onnc

#endif
