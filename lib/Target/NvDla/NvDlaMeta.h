//===- NvDlaMeta.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_META_H
#define TARGET_NVDLA_NVDLA_META_H

#include "NvDlaDefine.h"
#include "dla_interface.h"
#include "emu_interface.h"
#include "nvdla/ILoadable.h"
#include "priv/Loadable.h"
#include "priv/loadable_generated.h"

#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/Transpose.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iomanip>
#include <map>
#include <sstream>
#include <unordered_map>
#include <utility>

#ifndef UNIT_ALIGNMENT
#  define UNIT_ALIGNMENT(x, unit) (((x) + ((unit)-1)) & ~((unit)-1))
#else
#  error "UNIT_ALIGNMENT macro has been already defined"
#endif

#ifndef DIV_ROUNDUP
#  define DIV_ROUNDUP(x, dividor) ((x) + (dividor)-1) / (dividor)
#else
#  error "DIV_ROUNDUP macro has been already defined"
#endif

using namespace nvdla;
using namespace nvdla::priv;

namespace onnc {

using MemoryListEntryId  = decltype(std::declval<ILoadable::MemoryListEntry>().id);
using AddressListEntryId = decltype(std::declval<ILoadable::AddressListEntry>().id);

typedef std::unordered_map<const Tensor*, MemoryListEntryId> MemoryIdxTable;
typedef std::unordered_map<const Tensor*, const Tensor*>     RemapTable;

enum class CbufAllocType : unsigned
{
  kUnfeasible = 0,
  kSplitDataMinimumWeight,
  kFullDataMinimumWeight,
  kSplitDataPartialWeight,
  kSplitDataFullWeight,
  kFullDataPartialWeight,
  kFullDataFullWeight
};

/** \class NvDlaDlaOperation
 *  \brief
 */
class NvDlaDlaOperation
{
public:
  NvDlaDlaOperation() noexcept;

public:
  struct dla_common_op_desc     op_dep;
  union dla_operation_container op_desc;
  union dla_surface_container   op_surf;
};

/** \class NvDlaEmuOperation
 *  \brief
 */
class NvDlaEmuOperation
{
public:
  NvDlaEmuOperation() noexcept;

public:
  union emu_operation_container        op_desc;
  union emu_operation_buffer_container op_buf;
};

class NvDlaBackendMeta : private NvDlaConstants
{
public:
  friend class NvDlaMemInfoPass;
  friend class NvDlaTaskSubmitPass;

  using MemoryListEntry     = ILoadable::MemoryListEntry;
  using MemoryListEntrySize = decltype(std::declval<MemoryListEntry>().size);
  using AddressListEntry    = ILoadable::AddressListEntry;
  using Offset              = decltype(std::declval<AddressListEntry>().offset);
  using Size                = decltype(std::declval<AddressListEntry>().size);
  using MemoryDomain        = ILoadable::MemoryDomain;
  using MemoryFlags         = NvU8;
  using LutParams =
    std::tuple<float, float, float, std::int32_t, std::int8_t>; // alpha, beta, bias, size, lrn_exp_shift
  using LutId = std::int16_t;

  struct OperationMeta
  {
    using index_type = std::size_t;

    enum class Category : unsigned
    {
      none, dla, emu
    };

    OperationMeta() noexcept
      : OperationMeta(std::numeric_limits<index_type>::max(), Category::none)
    {}

    OperationMeta(index_type index, Category category) noexcept
      : index(index)
      , category(category)
    {}

    OperationMeta(const OperationMeta&) = default;
    OperationMeta(OperationMeta&&) = default;

    OperationMeta& operator=(const OperationMeta&) = default;
    OperationMeta& operator=(OperationMeta&&) = default;

    index_type index;
    Category category;
  };

public:
  explicit NvDlaBackendMeta(const NvDlaConstants& constants);

  ~NvDlaBackendMeta();

  bool                   hasMemoryListEntry(const Tensor& tensor) const noexcept;
  bool                   hasMemoryListEntry(MemoryListEntryId memoryId) const noexcept;
  MemoryListEntryId      getMemoryListEntryId(const Tensor& tensor) const noexcept;
  const MemoryListEntry& getMemoryListEntry(const Tensor& tensor) const;
  MemoryListEntry&       getMemoryListEntry(const Tensor& tensor);
  const MemoryListEntry& getMemoryListEntry(MemoryListEntryId id) const;
  MemoryListEntry&       getMemoryListEntry(MemoryListEntryId id);
  MemoryListEntrySize    getMemoryListEntrySize(const Tensor& tensor) const;
  MemoryListEntrySize    getMemoryListEntrySize(MemoryListEntryId id) const;
  bool                   isReshaped(const Tensor& tensor) const noexcept;
  const Tensor&          getReshapeSource(const Tensor& tensor) const;
  void                   markAsReshaped(const Tensor& input, const Tensor& output);
  bool                   shouldOwnMemory(const Tensor& tensor);
  AddressListEntryId     acquireMemory(MemoryListEntryId memoryId, Offset offset);
  AddressListEntryId     acquireMemory(MemoryListEntryId memoryId, Offset offset, Size size);
  MemoryListEntryId      allocateMemory(MemoryDomain domain, MemoryFlags flags, Size size, bool isOutput = false);
  MemoryListEntryId      allocateMemoryFor(const Tensor& tensor, MemoryDomain domain, MemoryFlags flags, Size size,
                                           bool isOutput = false);
  MemoryListEntryId      tryAllocateMemoryFor(const Tensor& tensor, MemoryDomain domain, MemoryFlags flags, Size size,
                                              bool isOutput = false);
  bool                   hasLutId(const LutParams& params) const;
  LutId                  getLutId(const LutParams& params) const;
  bool                   addLutId(const LutParams& params, LutId id);
  void                   appendOperationMeta(OperationMeta::index_type index, OperationMeta::Category category);
  bool                   isLastDlaOperationInTaskEntry(const NvDlaDlaOperation& operation) const;
  static MemoryListEntryId getInvalidMemoryListEntryId();

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

  int                             m_DlaAddresses;
  struct dla_network_desc         m_DlaNetworkDesc;
  int                             m_NumLUTs;
  std::vector<NvDlaDlaOperation*> m_DLAOperationList;
  std::vector<dla_lut_param*>     m_LUTList;
  NvDlaDlaOperation*              m_pDepOp[DLA_OP_NUM];
  NvDlaDlaOperation*              m_pPrevOp;

  emu_network_desc                m_EmuNetworkDesc;
  std::vector<NvDlaEmuOperation*> m_EMUOperationList;

private:
  bool hasAddressListEntry(MemoryListEntryId memoryId, Offset offset) const;
  AddressListEntryId getAddressListEntryId(MemoryListEntryId memoryId, Offset offset) const;

private:
  MemoryIdxTable                                   m_MemIdxTable;
  RemapTable                                       m_ReshapeTable;
  std::map<LutParams, LutId>                       m_LutIds;
  std::map<
    MemoryListEntryId,
    std::unordered_map<Offset, AddressListEntryId>
  >                                                m_AddressListEntryIds;
  std::vector<OperationMeta>                       m_OperationMetas;

public:
  int                                     m_NumBlobs;
  priv::LoadableFactory::LoadablePrivPair m_Loadable;
};

enum nvdla_cube_type
{
  NVLDA_CUBE_UNKNOWN,
  NVDLA_CUBE_FEATURE,
  NVDLA_CUBE_WEIGHT,
  NVDLA_CUBE_SDP_X_ALU_OR_MUL_ONE_BYTE,
  NVDLA_CUBE_SDP_X_BOTH_ONE_BYTE,
  NVDLA_CUBE_SDP_X_ALU_OR_MUL_TWO_BYTE,
  NVDLA_CUBE_SDP_X_BOTH_TWO_BYTE,
  NVDLA_CUBE_SDP_Y_ALU_OR_MUL_ONE_BYTE,
  NVDLA_CUBE_SDP_Y_BOTH_ONE_BYTE,
  NVDLA_CUBE_SDP_Y_ALU_OR_MUL_TWO_BYTE,
  NVDLA_CUBE_SDP_Y_BOTH_TWO_BYTE
};

class NvDlaCubeInfo : private NvDlaConstants
{
public:
  NvDlaCubeInfo(const NvDlaConstants& constants, nvdla_cube_type m, int n, int c, int h, int w,
                int pad_left = -1, int pad_right = -1);

  int  getBanksForFullWeights() const;
  void setBanksForFullWeights();

  int  getBanksForPartialWeights() const;
  void setBanksForPartialWeights();

  int  getBanksForMinimumWeights() const;
  void setBanksForMinimumWeights();

  ~NvDlaCubeInfo() = default;

public:
  nvdla_cube_type mode;
  unsigned        dim_n;
  unsigned        dim_c;
  unsigned        dim_h;
  unsigned        dim_w;
  unsigned        eps;
  unsigned        banks;
  unsigned        size;
  unsigned        stride_channel;
  unsigned        stride_line;
  unsigned        stride_surface;
  unsigned        stride_plane;
};

} // namespace onnc

#endif
