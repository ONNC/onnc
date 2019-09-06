//===- NVDLAMeta.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaMeta.h"

#include <onnc/Diagnostic/MsgHandling.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>

using namespace onnc;

namespace onnc {
namespace internal {
template <typename T, std::size_t N>
constexpr std::size_t size(const T (&)[N]) noexcept
{
  return N;
}
} // namespace internal
} // namespace onnc

//===----------------------------------------------------------------------===//
// NvDlaDlaOperation
//===----------------------------------------------------------------------===//
NvDlaDlaOperation::NvDlaDlaOperation() noexcept
  : op_dep{}
  , op_desc{}
  , op_surf{}
{
  std::memset(&op_dep, 0, sizeof(op_dep));
  std::memset(&op_desc, 0, sizeof(op_desc));
  std::memset(&op_surf, 0, sizeof(op_surf));

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
{
  std::memset(&op_desc, 0, sizeof(op_desc));
  std::memset(&op_buf, 0, sizeof(op_buf));
}

//===----------------------------------------------------------------------===//
// NvDlaBackendMeta
//===----------------------------------------------------------------------===//
NvDlaBackendMeta::NvDlaBackendMeta(const NvDlaConstants& constants)
  : NvDlaConstants{constants}
  , m_DlaNetworkDesc{}
  , m_NumLUTs{0}
  , m_pPrevOp{nullptr}
  , m_EmuNetworkDesc{}
  , m_NumBlobs{0}
  , m_Loadable{priv::LoadableFactory::newLoadable()}
{
  using namespace internal;

  using std::begin;
  using std::end;

  std::fill(begin(m_pDepOp), end(m_pDepOp), nullptr);

  for (std::size_t idx = 0; idx < size(m_DlaNetworkDesc.op_head); ++idx) {
    m_DlaNetworkDesc.op_head[idx] = -1;
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
    delete op;
  }

  std::vector<NvDlaEmuOperation*>::iterator op_emu;
  for (op_emu = m_EMUOperationList.begin(); op_emu != m_EMUOperationList.end(); op_emu++) {
    NvDlaEmuOperation* op = *op_emu;
    delete op;
  }

  std::vector<struct dla_lut_param*>::iterator lut_param;
  for (lut_param = m_LUTList.begin(); lut_param != m_LUTList.end(); lut_param++) {
    struct dla_lut_param* lut = *lut_param;
    delete lut;
  }

  priv::LoadableFactory::deleteLoadable(m_Loadable.i());
}

bool NvDlaBackendMeta::hasMemoryListEntry(const Tensor& tensor) const noexcept
{
  using std::end;

  return m_MemIdxTable.find(&tensor) != end(m_MemIdxTable);
}

bool NvDlaBackendMeta::hasMemoryListEntry(MemoryListEntryId memoryId) const noexcept
{
  return memoryId < m_MemoryListEntries.size();
}

MemoryListEntryId NvDlaBackendMeta::getMemoryListEntryId(const Tensor& tensor) const noexcept
{
  static_assert(std::is_integral<MemoryListEntryId>::value, "MemoryListEntryId should be an integer");

  using std::end;

  {
    const auto found = m_MemIdxTable.find(&tensor);
    if (found != end(m_MemIdxTable)) {
      return found->second;
    }
  }

  // if the tensor is Reshape's output, we should use
  // Reshape's input as search target
  {
    const auto found = m_ReshapeTable.find(&tensor);
    if (found != end(m_ReshapeTable)) {
      return getMemoryListEntryId(*found->second);
    }
  }

  return getInvalidMemoryListEntryId();
}

const NvDlaBackendMeta::MemoryListEntry& NvDlaBackendMeta::getMemoryListEntry(const Tensor& tensor) const
{
  return getMemoryListEntry(getMemoryListEntryId(tensor));
}

NvDlaBackendMeta::MemoryListEntry& NvDlaBackendMeta::getMemoryListEntry(const Tensor& tensor)
{
  return getMemoryListEntry(getMemoryListEntryId(tensor));
}

const NvDlaBackendMeta::MemoryListEntry& NvDlaBackendMeta::getMemoryListEntry(MemoryListEntryId id) const
{
  assert(hasMemoryListEntry(id));

  return m_MemoryListEntries[id];
}

NvDlaBackendMeta::MemoryListEntry& NvDlaBackendMeta::getMemoryListEntry(MemoryListEntryId id)
{
  assert(hasMemoryListEntry(id));

  return m_MemoryListEntries[id];
}

NvDlaBackendMeta::MemoryListEntrySize NvDlaBackendMeta::getMemoryListEntrySize(const Tensor& tensor) const
{
  return getMemoryListEntry(tensor).size;
}

NvDlaBackendMeta::MemoryListEntrySize NvDlaBackendMeta::getMemoryListEntrySize(MemoryListEntryId id) const
{
  return getMemoryListEntry(id).size;
}

bool NvDlaBackendMeta::isReshaped(const Tensor& tensor) const noexcept
{
  using std::end;

  return m_ReshapeTable.find(&tensor) != end(m_ReshapeTable);
}

const Tensor& NvDlaBackendMeta::getReshapeSource(const Tensor& tensor) const
{
  assert(isReshaped(tensor));

  using std::end;

  // find canonical Reshape input tensor (it's not a Reshape's output)
  const Tensor* to = &tensor;
  for (;;) {
    const auto found = m_ReshapeTable.find(to);
    if (found == end(m_ReshapeTable)) {
      return *to;
    }

    to = found->second;
  }

  assert(false && "should not reach here");
  return tensor;
}

void NvDlaBackendMeta::markAsReshaped(const Tensor& input, const Tensor& output)
{
  const auto result = m_ReshapeTable.emplace(&output, &input);
  assert(result.second && "cannot bind Reshape output with different input");
}

bool NvDlaBackendMeta::shouldOwnMemory(const Tensor& tensor)
{
  return !isReshaped(tensor);
}

AddressListEntryId NvDlaBackendMeta::acquireMemory(MemoryListEntryId memoryId, Offset offset)
{
  return acquireMemory(memoryId, offset, getMemoryListEntrySize(memoryId));
}

AddressListEntryId NvDlaBackendMeta::acquireMemory(MemoryListEntryId memoryId, Offset offset, Size size)
{
  assert(hasMemoryListEntry(memoryId));

  if (hasAddressListEntry(memoryId, offset)) {
    return getAddressListEntryId(memoryId, offset);
  }

  AddressListEntry address;

  address.id     = static_cast<AddressListEntryId>(m_AddressListEntries.size());
  address.mem_id = memoryId;
  address.size   = size;
  address.offset = offset;

  m_AddressListEntries.emplace_back(address);
  assert(m_AddressListEntries.size() < std::numeric_limits<std::int16_t>::max());

  m_AddressListEntryIds[memoryId].emplace(offset, address.id);

  return address.id;
}

MemoryListEntryId NvDlaBackendMeta::allocateMemory(MemoryDomain domain, MemoryFlags flags, Size size, bool isOutput)
{
  MemoryListEntry memory;

  memory.id             = static_cast<MemoryListEntryId>(m_MemoryListEntries.size());
  memory.alignment      = 4096;
  memory.bind_id        = 0;
  memory.domain         = domain;
  memory.flags          = flags;
  memory.size           = size;
  memory.tensor_desc_id = static_cast<NvU16>(isOutput);

  m_MemoryListEntries.emplace_back(memory);

  return memory.id;
}

MemoryListEntryId NvDlaBackendMeta::allocateMemoryFor(const Tensor& tensor, MemoryDomain domain, MemoryFlags flags,
                                                      Size size, bool isOutput)
{
  const MemoryListEntryId memoryId = allocateMemory(domain, flags, size, isOutput);

  const auto result = m_MemIdxTable.emplace(&tensor, memoryId);
  assert(result.second && "already allocated memory for this tensor");

  return memoryId;
}

MemoryListEntryId NvDlaBackendMeta::tryAllocateMemoryFor(const Tensor& tensor, MemoryDomain domain, MemoryFlags flags,
                                                         Size size, bool isOutput)
{
  if (hasMemoryListEntry(tensor)) {
    return getMemoryListEntryId(tensor);
  }

  return allocateMemoryFor(tensor, domain, flags, size, isOutput);
}

bool NvDlaBackendMeta::hasLutId(const LutParams& params) const
{
  using std::end;

  return m_LutIds.find(params) != end(m_LutIds);
}

NvDlaBackendMeta::LutId NvDlaBackendMeta::getLutId(const LutParams& params) const
{
  assert(hasLutId(params));

  return m_LutIds.find(params)->second;
}

bool NvDlaBackendMeta::addLutId(const LutParams& params, LutId id) { return m_LutIds.emplace(params, id).second; }

void NvDlaBackendMeta::appendOperationMeta(OperationMeta::index_type index, OperationMeta::Category category)
{
  if (!m_OperationMetas.empty() && m_OperationMetas.back().category != category) {
    const auto& lastMeta = m_OperationMetas.back();
    switch (lastMeta.category) {
    // reset last dla operation's consumers
    case OperationMeta::Category::dla:
      {
        NvDlaDlaOperation* const lastOperation = m_DLAOperationList[lastMeta.index];

        for (std::size_t iConsumer = 0; iConsumer < DLA_OP_NUM; ++iConsumer) {
          dla_consumer& consumer = lastOperation->op_dep.consumers[iConsumer];
          consumer.index = -1;
        }
      }
      break;
    case OperationMeta::Category::emu:
      break;
    case OperationMeta::Category::none:
      [[fallthrough]];
    default:
      assert(false && "should not reach here");
    }
  }

  m_OperationMetas.emplace_back(index, category);
}

bool NvDlaBackendMeta::isLastDlaOperationInTaskEntry(const NvDlaDlaOperation& operation) const
{
  if (m_OperationMetas.empty()) {
    return false;
  }

  OperationMeta::Category afterCategory = m_OperationMetas.back().category;
  for (std::size_t idx = m_OperationMetas.size(); 0 < idx; --idx) {
    const OperationMeta& beforeMeta = m_OperationMetas[idx - 1];

    const OperationMeta::Category beforeCategory  = beforeMeta.category;
    const NvDlaDlaOperation*      beforeOperation = m_DLAOperationList[beforeMeta.index];

    // check if meet category boundary and previous category is dla
    if (beforeCategory != afterCategory
        && beforeCategory == OperationMeta::Category::dla
        && beforeOperation == &operation) {
      return true;
    }

    afterCategory = beforeCategory;
  }

  return false;
}

MemoryListEntryId NvDlaBackendMeta::getInvalidMemoryListEntryId() { return static_cast<MemoryListEntryId>(-1); }

bool NvDlaBackendMeta::hasAddressListEntry(MemoryListEntryId memoryId, Offset offset) const
{
  using std::end;

  const auto memoryOffsetListPair = m_AddressListEntryIds.find(memoryId);
  if (memoryOffsetListPair == end(m_AddressListEntryIds)) {
    return false;
  }

  const auto& offsetList = memoryOffsetListPair->second;
  return offsetList.find(offset) != end(offsetList);
}

AddressListEntryId NvDlaBackendMeta::getAddressListEntryId(MemoryListEntryId memoryId, Offset offset) const
{
  assert(hasAddressListEntry(memoryId, offset));

  using std::end;

  const auto memoryOffsetListPair = m_AddressListEntryIds.find(memoryId);
  if (memoryOffsetListPair == end(m_AddressListEntryIds)) {
    return static_cast<AddressListEntryId>(-1);
  }

  const auto& offsetList = memoryOffsetListPair->second;
  return offsetList.find(offset)->second;
}

//===----------------------------------------------------------------------===//
// NvDlaCubeInfo
//===----------------------------------------------------------------------===//
NvDlaCubeInfo::NvDlaCubeInfo(const NvDlaConstants& constants, nvdla_cube_type m, int n, int c, int h, int w,
                             int pad_left, int pad_right)
  : NvDlaConstants{constants}
  , mode(m)
  , dim_n(n)
  , dim_c(c)
  , dim_h(h)
  , dim_w(w)
{
  switch (mode) {
  case NVDLA_CUBE_FEATURE:
    stride_channel = ELEMENT_SIZE;
    stride_line    = UNIT_ALIGNMENT(dim_w * FEATURE_ATOM_CUBE_SIZE, 32);
    stride_surface = stride_line * dim_h;
    stride_plane   = 0;
    {
      int atom_c    = FEATURE_ATOM_CUBE_SIZE / ELEMENT_SIZE;
      int segment_c = UNIT_ALIGNMENT(dim_c, atom_c);
      int num_surf = ((dim_c % MAC_ATOMIC_K == 0) ? (dim_c / MAC_ATOMIC_K): (dim_c / MAC_ATOMIC_K + 1));
      size = stride_surface * num_surf;

      // copy how SystemC calculate entry per slice
      int atom_per_channel = DIV_ROUNDUP((dim_c * ELEMENT_SIZE), FEATURE_ATOM_CUBE_SIZE);

      int entry_per_slice;
      //  This calculation is for nv_full and not general for any hardware configuration.
      entry_per_slice = (atom_per_channel / 4) * dim_w;

      // Same check as in IP_TOT/tools/cc_sanity_checker.pl (line350~357)
      if ((atom_per_channel % 4) == 3)
        entry_per_slice += dim_w;
      else if ((atom_per_channel % 4) == 2)
        entry_per_slice += (dim_w + 1) / 2;
      else if ((atom_per_channel % 4) == 1)
        entry_per_slice += (dim_w + 3) / 4;

      eps = entry_per_slice;
    }
    banks = DIV_ROUNDUP((eps * dim_h), CBUF_BANK_DEPTH);
    break;

  case NVDLA_CUBE_WEIGHT:
    size           = UNIT_ALIGNMENT(dim_n * dim_c * dim_h * dim_w * ELEMENT_SIZE, WEIGHT_ATOM_CUBE_SIZE);
    eps            = 0;
    stride_channel = ELEMENT_SIZE;
    stride_line    = 0;
    stride_surface = 0;
    stride_plane   = 0;
    banks          = 0; // Will be assigned later.
    break;

  case NVDLA_CUBE_SDP_X_ALU_OR_MUL_ONE_BYTE:
  case NVDLA_CUBE_SDP_Y_ALU_OR_MUL_ONE_BYTE:
    assert(dim_n == 1);

    size           = UNIT_ALIGNMENT(dim_c, MAC_ATOMIC_K) * dim_h * dim_w;
    stride_line    = MAC_ATOMIC_K * dim_w;
    stride_surface = stride_line * dim_h;
    stride_plane   = 0;
    break;

  case NVDLA_CUBE_SDP_X_BOTH_ONE_BYTE:
  case NVDLA_CUBE_SDP_Y_BOTH_ONE_BYTE:
    assert(dim_n == 1);

    size           = UNIT_ALIGNMENT(dim_c * 2, 2 * MAC_ATOMIC_K) * dim_h * dim_w;
    stride_line    = MAC_ATOMIC_K * 2 * dim_w;
    stride_surface = stride_line * dim_h;
    stride_plane   = 0;
    break;

  case NVDLA_CUBE_SDP_X_ALU_OR_MUL_TWO_BYTE:
  case NVDLA_CUBE_SDP_Y_ALU_OR_MUL_TWO_BYTE:
    assert(dim_n == 1);

    size           = 2 * UNIT_ALIGNMENT(dim_c, MAC_ATOMIC_K) * dim_h * dim_w;
    stride_line    = MAC_ATOMIC_K * 2 * dim_w;
    stride_surface = stride_line * dim_h;
    stride_plane   = 0;
    break;

  case NVDLA_CUBE_SDP_X_BOTH_TWO_BYTE:
  case NVDLA_CUBE_SDP_Y_BOTH_TWO_BYTE:
    assert(dim_n == 1);

    size           = 2 * UNIT_ALIGNMENT(dim_c * 2, 2 * MAC_ATOMIC_K) * dim_h * dim_w;
    stride_line    = MAC_ATOMIC_K * 4 * dim_w;
    stride_surface = stride_line * dim_h;
    stride_plane   = 0;
    break;

  default:
    unreachable(nvdla_unsupported_mode) << mode;
  } // end of switch
}

// Buffer full weights in CBUF
int NvDlaCubeInfo::getBanksForFullWeights() const
{
  return DIV_ROUNDUP((dim_n * dim_c * dim_h * dim_w * ELEMENT_SIZE), (CBUF_BANK_DEPTH * CBUF_BANK_WIDTH));
}

void NvDlaCubeInfo::setBanksForFullWeights() { banks = getBanksForFullWeights(); }

// Buffer partial weights in CBUF
int NvDlaCubeInfo::getBanksForPartialWeights() const
{
  return DIV_ROUNDUP(2 * MAC_ATOMIC_K * dim_c * dim_h * dim_w * ELEMENT_SIZE +
                     MAX_MEM_TRANSACTION_NUM * FEATURE_ATOM_CUBE_SIZE,
                     CBUF_BANK_DEPTH * CBUF_BANK_WIDTH);
}

void NvDlaCubeInfo::setBanksForPartialWeights() { banks = getBanksForPartialWeights(); }

// Buffer minimum weights in CBUF
int NvDlaCubeInfo::getBanksForMinimumWeights() const
{
  return DIV_ROUNDUP(MAC_ATOMIC_K * dim_c * dim_h * dim_w * ELEMENT_SIZE +
                     MAX_MEM_TRANSACTION_NUM * FEATURE_ATOM_CUBE_SIZE,
                     CBUF_BANK_DEPTH * CBUF_BANK_WIDTH);
}

void NvDlaCubeInfo::setBanksForMinimumWeights() { banks = getBanksForMinimumWeights(); }
