//===- CodeEmitVisitor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CodeEmitVisitor.h"

#include "genmemNvdla/genmemNvdla.hpp"
#include "NvDlaUtil.h"
#include "Compute/NvDlaShuffle.h"

#include <onnc/ADT/Color.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/LRN.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/IR/Compute/Split.h>
#include <onnc/IR/Compute/Sum.h>
#include <onnc/IR/Compute/Unsqueeze.h>
#include <onnc/Support/Algorithm.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Match.h>
#include <onnc/Support/Preprocessor.h>
#include <onnc/Support/Range.h>
#include <onnc/Support/String.h>
#include <onnc/Support/View.h>

#include <algorithm>
#include <iterator>
#include <sstream>
#include <type_traits>
#include <utility>
#include <vector>

#define TRACE_STMTS(stmts)                          \
  if (onnc::VerboseLevels::TRACE <= m_VerboseLevel) \
    do {                                            \
      stmts                                         \
  } while (false)
#define DEBUG_STMTS(stmts)                          \
  if (onnc::VerboseLevels::DEBUG <= m_VerboseLevel) \
    do {                                            \
      stmts                                         \
  } while (false)
#define INFO_STMTS(stmts)                          \
  if (onnc::VerboseLevels::INFO <= m_VerboseLevel) \
    do {                                           \
      stmts                                        \
  } while (false)
#define WARN_STMTS(stmts)                          \
  if (onnc::VerboseLevels::WARN <= m_VerboseLevel) \
    do {                                           \
      stmts                                        \
  } while (false)

#define PP_DEFINE_VISIT(type, op)                                                 \
  void CodeEmitVisitor::visit(const type& op)                                     \
  {                                                                               \
    DEBUG_STMTS(outs() << op << "\n";);                                           \
    visitImpl(op);                                                                \
  }                                                                               \
  void CodeEmitVisitor::visitImpl(const type& op)

#define FIXED_BATCH_NUM (1u)

namespace onnc {

enum class VerboseLevels : unsigned
{
  TRACE = 1,
  DEBUG = 2,
  INFO  = 3,
  WARN  = 4,
};

inline bool operator<=(VerboseLevels lhs, unsigned rhs) noexcept
{
  using underlying_type = std::underlying_type<VerboseLevels>::type;
  static_assert(std::is_unsigned<underlying_type>::value, "underlying type of verbose should be unsigned");

  return static_cast<underlying_type>(lhs) <= rhs;
}

std::ostream& operator<<(std::ostream& stream, const ComputeOperator& op)
{
  op.print(stream);
  return stream;
}

namespace internal {

enum class BroadcastCategory : std::int8_t
{
  UNSUPPORT = -1,
  LAYER     = 0,
  CHANNEL   = 1,
  ELEMENT   = 2,
};

BroadcastCategory getBroadcastCategory(const Tensor& fromTensor, const Tensor& toTensor)
{
  const Tensor::Dimensions& fromDims = fromTensor.getDimensions();
  const Tensor::Dimensions& toDims   = toTensor.getDimensions();

  if (!isConstant(fromTensor) && !isConstant(toTensor)) {
    if (size(fromDims) == 4 && size(toDims) == 4) {
      return (equal(fromDims, begin(toDims)) ? BroadcastCategory::ELEMENT : BroadcastCategory::UNSUPPORT);
    } else {
      return BroadcastCategory::UNSUPPORT;
    }
    // only constant tensor can broadcast to non-constant tensor
  } else if (!isConstant(fromTensor) || isConstant(toTensor)) {
    return BroadcastCategory::UNSUPPORT;
  }

  assert(size(toDims) == 4);

  using Any     = MatchAny<Tensor::Dimension>;
  using AnyList = std::vector<Any>;

  using std::begin;
  switch (size(fromDims)) {
  case 4: {
    assert(toDims[0] == 1);

    const Tensor::Dimension c = toDims[1];
    const Tensor::Dimension h = toDims[2];
    const Tensor::Dimension w = toDims[3];

    if (equal({1, 1, 1, 1}, begin(fromDims))) {
      return BroadcastCategory::LAYER;
    } else if (equal(asRange<AnyList>(1, c, 1, 1), begin(fromDims))) {
      return BroadcastCategory::CHANNEL;
    } else if (equal(asRange<AnyList>(1, c, h, w), begin(fromDims))) {
      return BroadcastCategory::ELEMENT;
    } else {
      return BroadcastCategory::UNSUPPORT;
    }
  } break;
  case 3: {
    const Tensor::Dimension c = toDims[1];

    if (equal({1, 1, 1}, begin(fromDims))) {
      return BroadcastCategory::LAYER;
    } else if (equal(asRange<AnyList>(c, 1, 1), begin(fromDims))) {
      return BroadcastCategory::CHANNEL;
    } else {
      return BroadcastCategory::UNSUPPORT;
    }
  } break;
  case 2: {
    if (equal({1, 1}, begin(fromDims))) {
      return BroadcastCategory::LAYER;
    } else {
      return BroadcastCategory::UNSUPPORT;
    }
  } break;
  case 1:
    return (fromDims[0] == 1 ? BroadcastCategory::LAYER : BroadcastCategory::UNSUPPORT);
  default:
    return BroadcastCategory::UNSUPPORT;
  }
}

std::uint8_t getSdpOpMode(BroadcastCategory category)
{
  assert(category != BroadcastCategory::UNSUPPORT);

  return static_cast<std::uint8_t>(category);
}

nvdla_cube_type getSdpXSingleCubeType(const Tensor& tensor, std::uint8_t precision)
{
  if (!isConstant(tensor)) {
    return NVDLA_CUBE_FEATURE;
  }

  switch (precision) {
  case PRECISION_INT8:
    return NVDLA_CUBE_SDP_X_ALU_OR_MUL_ONE_BYTE;
  case PRECISION_INT16:
    [[fallthrough]];
  case PRECISION_FP16:
    return NVDLA_CUBE_SDP_X_ALU_OR_MUL_TWO_BYTE;
  default:
    break;
  }

  assert(false && "meet unsupported tensor");
  return NVLDA_CUBE_UNKNOWN;
}

enum class NvDlaOpType : std::uint8_t
{
  bdma  = DLA_OP_BDMA,
  conv  = DLA_OP_CONV,
  sdp   = DLA_OP_SDP,
  pdp   = DLA_OP_PDP,
  cdp   = DLA_OP_CDP,
  rubik = DLA_OP_RUBIK,
};

bool operator==(std::underlying_type<NvDlaOpType>::type lhs, NvDlaOpType rhs)
{
  return lhs == static_cast<std::underlying_type<NvDlaOpType>::type>(rhs);
}

std::unique_ptr<NvDlaDlaOperation> makeNvDlaOp(NvDlaOpType type)
{
  auto operation            = std::make_unique<NvDlaDlaOperation>();
  operation->op_dep.op_type = static_cast<std::underlying_type<NvDlaOpType>::type>(type);
  return operation;
}

template <NvDlaOpType type>
struct nvdla_op_desc;

template <>
struct nvdla_op_desc<NvDlaOpType::sdp>
{
  using type = dla_sdp_op_desc;
};

template <NvDlaOpType type>
typename nvdla_op_desc<type>::type& getDesc(NvDlaDlaOperation& operation);

template <>
nvdla_op_desc<NvDlaOpType::sdp>::type& getDesc<NvDlaOpType::sdp>(NvDlaDlaOperation& operation)
{
  assert(operation.op_dep.op_type == NvDlaOpType::sdp);
  return operation.op_desc.sdp_op;
}

template <NvDlaOpType type>
struct nvdla_op_surface;

template <>
struct nvdla_op_surface<NvDlaOpType::sdp>
{
  using type = dla_sdp_surface_desc;
};

template <NvDlaOpType type>
typename nvdla_op_surface<type>::type& getSurface(NvDlaDlaOperation& operation);

template <>
nvdla_op_surface<NvDlaOpType::sdp>::type& getSurface<NvDlaOpType::sdp>(NvDlaDlaOperation& operation)
{
  assert(operation.op_dep.op_type == NvDlaOpType::sdp);
  return operation.op_surf.sdp_surface;
}

NvDlaCubeInfo makeCubeInfo(const NvDlaConstants& constants, nvdla_cube_type type, Tensor::Dimension n,
                           Tensor::Dimension c, Tensor::Dimension h, Tensor::Dimension w)
{
  return NvDlaCubeInfo(constants, type, n, c, h, w);
}

NvDlaCubeInfo makeCubeInfo(const NvDlaConstants& constants, nvdla_cube_type type, const Tensor& tensor)
{
  const NvDlaDims dimensions(tensor);
  return makeCubeInfo(constants, type, dimensions.n, dimensions.c, dimensions.h, dimensions.w);
}

enum class NvDlaMemType : std::uint16_t
{
  mc = DLA_MEM_MC,
  cv = DLA_MEM_CV,
  hw = DLA_MEM_HW,
};

class NvDlaDataCubeModifier
{
public:
  NvDlaDataCubeModifier(dla_data_cube& cube, NvDlaMemType type)
    : cube_{cube}
  {
    cube_.type = static_cast<std::underlying_type<NvDlaMemType>::type>(type);
  }

  NvDlaDataCubeModifier& setAddress(std::int16_t address)
  {
    cube_.address = address;
    return *this;
  }

  NvDlaDataCubeModifier& setSize(std::uint32_t size)
  {
    cube_.size = size;
    return *this;
  }

  NvDlaDataCubeModifier& setInfo(const NvDlaCubeInfo& info)
  {
    cube_.width        = info.dim_w;
    cube_.height       = info.dim_h;
    cube_.channel      = info.dim_c;
    cube_.line_stride  = info.stride_line;
    cube_.surf_stride  = info.stride_surface;
    cube_.plane_stride = info.stride_plane;
    return *this;
  }

private:
  dla_data_cube& cube_;
};

} // namespace internal

using namespace internal;

namespace nvdla {

#include "CodeEmitVisitor/Add.inc"
#include "CodeEmitVisitor/AveragePool.inc"
#include "CodeEmitVisitor/Concat.inc"
#include "CodeEmitVisitor/Conv.inc"
#include "CodeEmitVisitor/Initializer.inc"
#include "CodeEmitVisitor/InputOperator.inc"
#include "CodeEmitVisitor/LRN.inc"
#include "CodeEmitVisitor/MaxPool.inc"
#include "CodeEmitVisitor/Mul.inc"
#include "CodeEmitVisitor/NvDlaShuffle.inc"
#include "CodeEmitVisitor/OutputOperator.inc"
#include "CodeEmitVisitor/Relu.inc"
#include "CodeEmitVisitor/Reshape.inc"
#include "CodeEmitVisitor/Softmax.inc"
#include "CodeEmitVisitor/Split.inc"
#include "CodeEmitVisitor/Sum.inc"
#include "CodeEmitVisitor/Transpose.inc"

MemoryListEntryId CodeEmitVisitor::packWeight(const Tensor& weight, NvDlaDims destDims,
                                              Tensor::Dimension numFrontPaddingChannels,
                                              Tensor::Dimension outputChannelOffset)
{
  return packWeight(weight, NvDlaDims(weight), destDims, numFrontPaddingChannels, outputChannelOffset);
}

MemoryListEntryId CodeEmitVisitor::packWeight(const Tensor& weight, NvDlaDims srcDims, NvDlaDims destDims,
                                              Tensor::Dimension numFrontPaddingChannels,
                                              Tensor::Dimension outputChannelOffset)
{
  if (const auto* const floatTensor = dynamic_cast<const FloatTensor*>(&weight)) {
    return packWeight(floatTensor->getValues(), floatTensor, srcDims, destDims, numFrontPaddingChannels,
                      outputChannelOffset);
  }

  return MemoryListEntryId(-1);
}

MemoryListEntryId CodeEmitVisitor::packWeight(span<const float> weight, const Tensor* weightTensor, NvDlaDims srcDims,
                                              NvDlaDims destDims, Tensor::Dimension numFrontPaddingChannels,
                                              Tensor::Dimension outputChannelOffset)
{
  assert(size(weight) == srcDims.size());

  std::string blob_name = "tb-" + std::to_string(m_pMeta.m_NumBlobs++);

  const Tensor::Dimension numDestChannels = numFrontPaddingChannels + destDims.c;
  const NvDlaDims         destDimsWithFrontPadding(destDims.n, numDestChannels, destDims.h, destDims.w);

  ILoadable::Blob b;
  b.name              = blob_name;
  b.size              = UNIT_ALIGNMENT(destDimsWithFrontPadding.size() * ELEMENT_SIZE, WEIGHT_ATOM_CUBE_SIZE);
  b.version.major     = 0;
  b.version.minor     = 0;
  b.version.sub_minor = 0;
  b.interface         = ILoadable::Interface_NONE;
  b.subInterface      = 0;

  NvU8* blob_data = new NvU8[b.size];
  memset(blob_data, 0, b.size);

  const auto* srcData = weight.data();

  using weight_type           = nv_weight_t<nvdla::ConfigSet::nv_full>;
  weight_type* const destData = reinterpret_cast<weight_type*>(blob_data);

  packWeightImpl(destData, destDimsWithFrontPadding, weightTensor, srcData, srcDims, numFrontPaddingChannels,
                 outputChannelOffset);

  m_pMeta.m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);

  const MemoryListEntryId memoryId = m_pMeta.allocateMemory(
    ILoadable::MemoryDomain_SYSMEM, ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_SET, b.size);

  ILoadable::MemoryListEntry& memory = m_pMeta.getMemoryListEntry(memoryId);
  memory.contents.push_back(blob_name);
  memory.offsets.push_back(0);

  return memoryId;
}

MemoryListEntryId CodeEmitVisitor::packBias(const Tensor& bias, Tensor::Dimension numDestChannels,
                                            Tensor::Dimension srcChannelOffset)
{
  std::string   blob_name = "tb-" + std::to_string(m_pMeta.m_NumBlobs++);
  NvDlaCubeInfo finfo(*this, NVDLA_CUBE_FEATURE, 1, numDestChannels, 1, 1);

  ILoadable::Blob b;
  b.name              = blob_name;
  b.size              = ELEMENT_SIZE * UNIT_ALIGNMENT(numDestChannels, MAC_ATOMIC_K);
  b.version.major     = 0;
  b.version.minor     = 0;
  b.version.sub_minor = 0;
  b.interface         = ILoadable::Interface_NONE;
  b.subInterface      = 0;

  NvU8* blob_data = new NvU8[b.size];
  std::memset(blob_data, 0, b.size);

  if (const FloatTensor* floatTensor = dynamic_cast<const FloatTensor*>(&bias)) {
    const auto* srcData = reinterpret_cast<const float*>(floatTensor->getValues().data());

    using weight_type           = nv_weight_t<nvdla::ConfigSet::nv_full>;
    weight_type* const destData = reinterpret_cast<weight_type*>(blob_data);

    packBiasImpl(destData, numDestChannels, &bias, srcData, srcChannelOffset);
  }

  m_pMeta.m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);

  const MemoryListEntryId memoryId = m_pMeta.allocateMemory(
    ILoadable::MemoryDomain_SYSMEM, ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_SET, b.size);

  ILoadable::MemoryListEntry& memory = m_pMeta.getMemoryListEntry(memoryId);
  memory.contents.push_back(blob_name);
  memory.offsets.push_back(0);

  return memoryId;
}

template <typename Type>
void CodeEmitVisitor::packWeightImpl(Type* destData, NvDlaDims destDimsWithFrontPadding, const Tensor* tensor,
                                     const float* srcData, NvDlaDims srcDims, Tensor::Dimension numFrontPaddingChannels,
                                     Tensor::Dimension outputChannelOffset)
{
  const NvDlaDims::value_type N = destDimsWithFrontPadding.n;
  const NvDlaDims::value_type C = destDimsWithFrontPadding.c;
  const NvDlaDims::value_type H = destDimsWithFrontPadding.h;
  const NvDlaDims::value_type W = destDimsWithFrontPadding.w;

  const int channel_per_cube = WEIGHT_ATOM_CUBE_SIZE / ELEMENT_SIZE;
  const int w_stride_kgrp    = MAC_ATOMIC_K * C * H * W;

  using weight_t = typename std::decay<decltype(*destData)>::type;

  for (Tensor::Dimension n = 0; n < (N / MAC_ATOMIC_K + 1); n++) {
    int n_size        = (N - n * MAC_ATOMIC_K >= MAC_ATOMIC_K) ? MAC_ATOMIC_K : N - n * MAC_ATOMIC_K;
    int w_stride_surf = W * H * n_size * channel_per_cube;
    for (Tensor::Dimension h = 0; h < H; h++) {
      for (Tensor::Dimension w = 0; w < W; w++) {
        for (int n_ofs = 0; n_ofs < n_size; n_ofs++) {
          for (Tensor::Dimension c = 0; c < C; c++) {
            int surf_ofs  = c / channel_per_cube;
            int ch_ofs    = c % channel_per_cube;
            int cube_size = (C - surf_ofs * channel_per_cube) >= channel_per_cube ? channel_per_cube
                                                                                  : (C - surf_ofs * channel_per_cube);
            int w_stride_line = W * n_size * cube_size;

            int dest_ofs = (n * w_stride_kgrp) + (surf_ofs * w_stride_surf) + (h * w_stride_line) +
                           w * n_size * cube_size + (n_ofs * cube_size) + ch_ofs;

            const Tensor::Dimension srcChannel = c - numFrontPaddingChannels;
            int src_ofs = ((n * MAC_ATOMIC_K + n_ofs + outputChannelOffset) * srcDims.c * srcDims.h * srcDims.w) +
                          (srcChannel * (srcDims.h * srcDims.w)) + (h * srcDims.w) + w;

            // fill zero at front if necessary
            if (c < numFrontPaddingChannels) {
              *(destData + dest_ofs) = 0;
              continue;
            }

            assert(srcChannel < srcDims.c);
            assert(src_ofs < srcDims.size());

            *(destData + dest_ofs) = f2float16_ieee(*(srcData + src_ofs));
          }
        }
      }
    }
  }
}

template <typename Type>
void CodeEmitVisitor::packBiasImpl(Type* destData, Tensor::Dimension numDestChannels, const Tensor* tensor,
                                   const float* srcData, Tensor::Dimension srcChannelOffset)
{
  assert(srcData != nullptr);
  assert(destData != nullptr);

  using weight_t = typename std::decay<decltype(*destData)>::type;

  for (Tensor::Dimension destChannel = 0; destChannel < numDestChannels; ++destChannel) {
    *(destData + destChannel) =
      f2float16_ieee(*(srcData + srcChannelOffset + destChannel)); // FIXME: bad hack method.
  }
}

MemoryListEntryId CodeEmitVisitor::packFeature(const Tensor& tensor, const NvDlaCubeInfo& cube)
{
  assert(false && "not implemented");
  return 0;
}

AddressListEntryId CodeEmitVisitor::issueEmuAddr(MemoryListEntryId mid)
{
  AddressListEntryId aid = m_pMeta.m_AddressListEntries.size();

  ILoadable::AddressListEntry ale;
  ILoadable::MemoryListEntry  mle = m_pMeta.getMemoryListEntry(mid);

  ale.size   = 0;
  ale.offset = 0;
  ale.mem_id = mid;
  ale.id     = aid;

  m_pMeta.m_AddressListEntries.push_back(ale);
  return aid;
}

void CodeEmitVisitor::issueEmuOp(NvDlaEmuOperation* op)
{
  m_pMeta.m_EMUOperationList.push_back(op);

  m_pMeta.appendOperationMeta(m_pMeta.m_EMUOperationList.size() - 1,
                              NvDlaBackendMeta::OperationMeta::Category::emu);
}

AddressListEntryId CodeEmitVisitor::issueDlaAddr(const Tensor& tensor, const NvDlaCubeInfo& cube,
                                                 Tensor::Dimension channelOffset, NvDlaBackendMeta::Offset hOffset)
{
  using offset_type = NvDlaBackendMeta::Offset;

  const offset_type h_offset     = hOffset * cube.stride_line;
  const offset_type memoryOffset = (channelOffset * (cube.dim_h * cube.dim_w * ELEMENT_SIZE)) + h_offset;

  return m_pMeta.acquireMemory(m_pMeta.getMemoryListEntryId(tensor), memoryOffset);
}

AddressListEntryId CodeEmitVisitor::issueDlaAddr(const Tensor& tensor, const NvDlaCubeInfo& cube)
{
  const MemoryListEntryId memoryId = m_pMeta.getMemoryListEntryId(tensor);

  return m_pMeta.acquireMemory(memoryId, 0);
}

AddressListEntryId CodeEmitVisitor::issueSDPOperand(const Tensor& tensor, const NvDlaCubeInfo& cube,
                                                    MemoryListEntryId& memoryId)
{
  if (isConstant(tensor)) {
    memoryId = packSDPOperand(&tensor, nullptr, cube);
    return issueDlaAddr(memoryId, cube);
  }

  memoryId = m_pMeta.getMemoryListEntryId(tensor);
  return issueDlaAddr(tensor, cube);
}

AddressListEntryId CodeEmitVisitor::issueDlaAddr(MemoryListEntryId memoryId, const NvDlaCubeInfo& cube)
{
  assert(m_pMeta.hasMemoryListEntry(memoryId));

  return m_pMeta.acquireMemory(memoryId, 0);
}

void CodeEmitVisitor::issueDlaOp(NvDlaDlaOperation* op, NvDlaDlaOperation* op_fuse, NvDlaDlaOperation* op_prev)
{
  struct dla_common_op_desc* op_desc = &(op->op_dep);
  int                        op_type = op_desc->op_type;
  op_desc->index            = m_pMeta.m_DLAOperationList.size();
  op_desc->roi_index        = 0;
  op_desc->dependency_count = 0;

  if (op_prev != NULL && !m_pMeta.isLastDlaOperationInTaskEntry(*op_prev)) {
    struct dla_common_op_desc* prev_op_desc = &(op_prev->op_dep);
    prev_op_desc->consumers[op_type].index  = op_desc->index;
    prev_op_desc->consumers[op_type].event  = 1;
    op_desc->dependency_count++;
  }

  if (m_pMeta.m_pDepOp[op_type] != NULL && !m_pMeta.isLastDlaOperationInTaskEntry(*m_pMeta.m_pDepOp[op_type])) {
    struct dla_common_op_desc* dep_op_desc = &(m_pMeta.m_pDepOp[op_type]->op_dep);
    if (m_pMeta.m_pDepOp[op_type] != op_prev) {
      dep_op_desc->consumers[op_type].index = op_desc->index;
      if ((op_type == DLA_OP_CONV) && (op_prev == NULL)) { // splitted convolution layers other than the first one
        dep_op_desc->consumers[op_type].event = 3;
      } else { // first splitted layer or other normal layers
        dep_op_desc->consumers[op_type].event = 2;
      }
      op_desc->dependency_count++;
    }
  }

  m_pMeta.m_DlaNetworkDesc.op_head[op_type] = (m_pMeta.m_DlaNetworkDesc.op_head[op_type] < 0)
                                                ? m_pMeta.m_DLAOperationList.size()
                                                : m_pMeta.m_DlaNetworkDesc.op_head[op_type];

  m_pMeta.m_DLAOperationList.push_back(op);
  m_pMeta.m_pDepOp[op_type] = op;

  m_pMeta.appendOperationMeta(m_pMeta.m_DLAOperationList.size() - 1,
                              NvDlaBackendMeta::OperationMeta::Category::dla);

  if (op_fuse != NULL) {
    struct dla_common_op_desc* fuse_op_desc = &(op_fuse->op_dep);
    int                        op_fuse_type = fuse_op_desc->op_type;
    fuse_op_desc->index                     = m_pMeta.m_DLAOperationList.size();
    fuse_op_desc->roi_index                 = 0;
    fuse_op_desc->dependency_count          = 1;
    fuse_op_desc->fused_parent.index        = op_desc->index;
    fuse_op_desc->fused_parent.event        = 3;

    op_desc->consumers[op_fuse_type].index = fuse_op_desc->index;
    op_desc->consumers[op_fuse_type].event = 2;
    if (op_prev != NULL) {
      struct dla_common_op_desc* prev_op_desc = &(op_prev->op_dep);
      prev_op_desc->consumers[op_type].event  = 1;
    }
    op_desc->dependency_count++;

    if (m_pMeta.m_pDepOp[op_fuse_type] != NULL) {
      struct dla_common_op_desc* dep_op_desc     = &(m_pMeta.m_pDepOp[op_fuse_type]->op_dep);
      dep_op_desc->consumers[op_fuse_type].index = fuse_op_desc->index;
      dep_op_desc->consumers[op_fuse_type].event = 2;

      fuse_op_desc->dependency_count++;
    }
    m_pMeta.m_pDepOp[op_fuse_type] = op_fuse;

    m_pMeta.m_DlaNetworkDesc.op_head[op_fuse_type] = (m_pMeta.m_DlaNetworkDesc.op_head[op_fuse_type] < 0)
                                                       ? m_pMeta.m_DLAOperationList.size()
                                                       : m_pMeta.m_DlaNetworkDesc.op_head[op_fuse_type];

    m_pMeta.m_DLAOperationList.push_back(op_fuse);
    m_pMeta.m_pPrevOp = op_fuse;

    m_pMeta.appendOperationMeta(m_pMeta.m_DLAOperationList.size() - 1,
                                NvDlaBackendMeta::OperationMeta::Category::dla);
  } else {
    m_pMeta.m_pPrevOp = op;
  }
}

void CodeEmitVisitor::issueDlaOp(std::unique_ptr<NvDlaDlaOperation> operation)
{
  issueDlaOp(operation.release(), nullptr, m_pMeta.m_pPrevOp);
}

void CodeEmitVisitor::emitSdp(std::uint8_t opType, const Tensor& first, const Tensor& second, const Tensor& output)
{
  assert(!(isConstant(first) && isConstant(second)) && "cannot support 2 constant tensors");
  assert(opType == SDP_OP_ADD || opType == SDP_OP_MUL);

  // make sure the 'first' tensor is always non-constant
  if (isConstant(first)) {
    emitSdp(opType, second, first, output);
    return;
  }

  assert(!isConstant(first));

  const BroadcastCategory category =
    (isConstant(second) ? getBroadcastCategory(second, first) : getBroadcastCategory(first, second));
  auto operation = makeNvDlaOp(NvDlaOpType::sdp);

  auto& desc             = getDesc<NvDlaOpType::sdp>(*operation);
  desc.src_precision     = DLA_PRECISION;
  desc.dst_precision     = DLA_PRECISION;
  desc.lut_index         = -1;
  desc.out_cvt.scale     = 1;
  desc.out_cvt.truncate  = 0;
  desc.out_cvt.enable    = 1;
  desc.out_cvt.offset    = 0;
  desc.conv_mode         = CONV_MODE_DIRECT;
  desc.batch_num         = 1;
  desc.batch_stride      = 0;
  desc.x1_op.enable      = 1;
  desc.x1_op.alu_type    = SDP_ALU_OP_SUM;
  desc.x1_op.type        = opType;
  desc.x1_op.mode        = getSdpOpMode(category);
  desc.x1_op.act         = ACTIVATION_NONE;
  desc.x1_op.shift_value = 0;
  desc.x1_op.truncate    = 0;
  desc.x1_op.precision   = DLA_PRECISION;
  if (category == BroadcastCategory::LAYER) {
    const auto operand = to_<std::vector<float>>(second);
    assert(size(operand) == 1);

    switch (opType) {
    case SDP_OP_ADD:
      desc.x1_op.mul_operand = 0;
      desc.x1_op.alu_operand = f2float16_ieee(*begin(operand));
      break;
    case SDP_OP_MUL:
      desc.x1_op.alu_operand = 0;
      desc.x1_op.mul_operand = f2float16_ieee(*begin(operand));
      break;
    default:
      assert(false && "should not reach here");
    }
  }

  auto& surface = getSurface<NvDlaOpType::sdp>(*operation);

  const NvDlaCubeInfo firstCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, first);
  NvDlaDataCubeModifier(surface.src_data, NvDlaMemType::mc)
    .setSize(m_pMeta.getMemoryListEntrySize(first))
    .setAddress(issueDlaAddr(first, firstCubeInfo))
    .setInfo(firstCubeInfo);

  if (category == BroadcastCategory::LAYER) {
    NvDlaDataCubeModifier(surface.x1_data, NvDlaMemType::hw).setAddress(-1);
  } else {
    MemoryListEntryId   memoryId;
    const NvDlaCubeInfo secondCubeInfo = makeCubeInfo(*this, getSdpXSingleCubeType(second, DLA_PRECISION), second);
    NvDlaDataCubeModifier(surface.x1_data, NvDlaMemType::mc)
      .setAddress(issueSDPOperand(second, secondCubeInfo, memoryId))
      .setSize(m_pMeta.getMemoryListEntrySize(memoryId))
      .setInfo(secondCubeInfo);
  }

  const NvDlaCubeInfo outputCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, output);
  NvDlaDataCubeModifier(surface.dst_data, NvDlaMemType::mc)
    .setSize(m_pMeta.getMemoryListEntrySize(output))
    .setAddress(issueDlaAddr(output, outputCubeInfo))
    .setInfo(outputCubeInfo);

  issueDlaOp(std::move(operation));
}

void CodeEmitVisitor::packSDPOperandImpl(NvU8* blob, const Tensor* aluTensor, const float* aluData,
                                         const Tensor* mulTensor, const float* mulData, const NvDlaCubeInfo& cubeInfo)
{
  int64_t tmpdims[4];
  tmpdims[0] = cubeInfo.dim_n;
  tmpdims[1] = cubeInfo.dim_c;
  tmpdims[2] = cubeInfo.dim_h;
  tmpdims[3] = cubeInfo.dim_w;
  NvDlaDims srcDims(tmpdims);

  const float*  srcData = nullptr;
  const Tensor* srcTensor;
  if (cubeInfo.mode == NVDLA_CUBE_SDP_X_ALU_OR_MUL_ONE_BYTE || cubeInfo.mode == NVDLA_CUBE_SDP_Y_ALU_OR_MUL_ONE_BYTE ||
      cubeInfo.mode == NVDLA_CUBE_SDP_X_ALU_OR_MUL_TWO_BYTE || cubeInfo.mode == NVDLA_CUBE_SDP_Y_ALU_OR_MUL_TWO_BYTE) {
    if (aluData != nullptr) {
      srcData   = aluData;
      srcTensor = aluTensor;
    } else if (mulData != nullptr) {
      srcData   = mulData;
      srcTensor = mulTensor;
    }

    assert(srcData != nullptr);
  }

  assert(cubeInfo.dim_n == 1);
  for (int c = 0; c < cubeInfo.dim_c; c++) {     // kernel channel
    for (int h = 0; h < cubeInfo.dim_h; h++) {   // kernel height
      for (int w = 0; w < cubeInfo.dim_w; w++) { // kernel width

        int blob_ofs = getBlobOffsetForSDPOperand(c, h, w, cubeInfo);
        // printf("(c,h,w) = (%.2d,%.2d,%.2d)  blob_ofs=%.5d   (KC,KH,KW)=(%d,%d,%d)\n", c, h, w, blob_ofs,
        // cubeInfo.dim_n,
        //        cubeInfo.dim_c, cubeInfo.dim_h, cubeInfo.dim_w);

        int src_ofs = getONNXInitializerOffset(0, c, h, w, srcDims);

        switch (cubeInfo.mode) {
        case NVDLA_CUBE_SDP_X_ALU_OR_MUL_ONE_BYTE:
        case NVDLA_CUBE_SDP_Y_ALU_OR_MUL_ONE_BYTE:
          break;

        case NVDLA_CUBE_SDP_X_BOTH_ONE_BYTE:
          break;

        case NVDLA_CUBE_SDP_Y_BOTH_ONE_BYTE:
          break;

        case NVDLA_CUBE_SDP_X_ALU_OR_MUL_TWO_BYTE:
        case NVDLA_CUBE_SDP_Y_ALU_OR_MUL_TWO_BYTE:
          uint16_t data;
          data = (uint16_t)f2float16_ieee(*(srcData + src_ofs));
          // NVDLA uses little endian to interpret data in cube.
          *(blob + 2 * blob_ofs)     = (NvU8)(data & 0xFF);
          *(blob + 2 * blob_ofs + 1) = (NvU8)((data >> 8) & 0xFF);
          break;

        case NVDLA_CUBE_SDP_X_BOTH_TWO_BYTE:
        case NVDLA_CUBE_SDP_Y_BOTH_TWO_BYTE:
          uint16_t alu;
          uint16_t mul;
          alu = (uint16_t)f2float16_ieee(*(aluData + src_ofs));
          mul = (uint16_t)f2float16_ieee(*(mulData + src_ofs));

          if (cubeInfo.mode == NVDLA_CUBE_SDP_X_BOTH_TWO_BYTE) {
            *(blob + 4 * blob_ofs)     = (NvU8)(alu & 0xFF);
            *(blob + 4 * blob_ofs + 1) = (NvU8)((alu >> 8) & 0xFF);
            *(blob + 4 * blob_ofs + 2) = (NvU8)(mul & 0xFF);
            *(blob + 4 * blob_ofs + 3) = (NvU8)((mul >> 8) & 0xFF);
          } else {
            assert(cubeInfo.mode == NVDLA_CUBE_SDP_Y_BOTH_TWO_BYTE);

            *(blob + 4 * blob_ofs)     = (NvU8)(mul & 0xFF);
            *(blob + 4 * blob_ofs + 1) = (NvU8)((mul >> 8) & 0xFF);
            *(blob + 4 * blob_ofs + 2) = (NvU8)(alu & 0xFF);
            *(blob + 4 * blob_ofs + 3) = (NvU8)((alu >> 8) & 0xFF);
          }
          break;

        default:
          assert(0 && "Unsupported SDP cube mode.");
          break;
        } // switch (cubeInfo.mode)

      } // for (int w
    }   // for (int h
  }     // for (int c
}

// NvDlaCubeInfo BN_OPERAND
MemoryListEntryId CodeEmitVisitor::packSDPOperand(const Tensor* aluTensor, const Tensor* mulTensor,
                                                  const NvDlaCubeInfo& cubeInfo)
{
  if (cubeInfo.mode == NVDLA_CUBE_SDP_X_BOTH_ONE_BYTE || cubeInfo.mode == NVDLA_CUBE_SDP_X_BOTH_TWO_BYTE) {
    assert(aluTensor != nullptr && mulTensor != nullptr);
  }

  if (cubeInfo.mode == NVDLA_CUBE_SDP_X_ALU_OR_MUL_ONE_BYTE || cubeInfo.mode == NVDLA_CUBE_SDP_X_ALU_OR_MUL_TWO_BYTE) {
    assert((aluTensor != nullptr && mulTensor == nullptr) || (aluTensor == nullptr && mulTensor != nullptr));
  }

  assert((aluTensor == nullptr || mulTensor == nullptr) || (NvDlaDims(*aluTensor) == NvDlaDims(*mulTensor)));

  std::string blob_name = "tb-" + std::to_string(m_pMeta.m_NumBlobs++);

  ILoadable::Blob b;
  b.name              = blob_name;
  b.size              = cubeInfo.size;
  b.version.major     = 0;
  b.version.minor     = 0;
  b.version.sub_minor = 0;
  b.interface         = ILoadable::Interface_NONE;
  b.subInterface      = 0;

  NvU8* blob_data = new NvU8[b.size];
  memset(blob_data, 0, b.size);

  int64_t tmpdims[4];
  tmpdims[0] = cubeInfo.dim_n;
  tmpdims[1] = cubeInfo.dim_c;
  tmpdims[2] = cubeInfo.dim_h;
  tmpdims[3] = cubeInfo.dim_w;
  NvDlaDims srcDims(tmpdims);

  // Get data of ALU and/or MUL.
  const float* aluData;
  const float* mulData;
  if (aluTensor != nullptr) {
    if (const FloatTensor* floatTensor = dynamic_cast<const FloatTensor*>(aluTensor)) {
      aluData = reinterpret_cast<const float*>(floatTensor->getValues().data());
    }
  }
  if (mulTensor != nullptr) {
    if (const FloatTensor* floatTensor = dynamic_cast<const FloatTensor*>(mulTensor)) {
      mulData = reinterpret_cast<const float*>(floatTensor->getValues().data());
    }
  }

  packSDPOperandImpl(blob_data, aluTensor, aluData, mulTensor, mulData, cubeInfo);

  m_pMeta.m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);

  const MemoryListEntryId memoryId = m_pMeta.allocateMemory(
    ILoadable::MemoryDomain_SYSMEM, ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_SET, b.size);

  ILoadable::MemoryListEntry& memory = m_pMeta.getMemoryListEntry(memoryId);
  memory.contents.push_back(blob_name);
  memory.offsets.push_back(0);

  return memoryId;
}

template <typename Type>
void CodeEmitVisitor::packImageWeightImpl(Type* blob, NvDlaDims blobDims, const Tensor* tensor, const float* srcData,
                                          NvDlaDims srcDims, Tensor::Dimension outputChannelOffset)
{
  assert((blobDims.c == 4) && "Kernel channel must be 4 in image mode.");

  using weight_t = typename std::decay<decltype(*blob)>::type;

  for (int k = 0; k < blobDims.n; k++) {       // kernel number
    for (int c = 0; c < blobDims.c; c++) {     // kernel channel
      for (int h = 0; h < blobDims.h; h++) {   // kernel height
        for (int w = 0; w < blobDims.w; w++) { // kernel width

          if (c >= srcDims.c)
            continue;

          int src_ofs  = getONNXInitializerOffset(outputChannelOffset + k, c, h, w, srcDims);
          int blob_ofs = getBlobOffsetForImageWeight(k, c, h, w, blobDims);

          *(blob + blob_ofs) = (weight_t)f2float16_ieee(*(srcData + src_ofs));
        }
      }
    }
  }
}

MemoryListEntryId CodeEmitVisitor::packImageWeight(const Tensor& weight, NvDlaDims destDims,
                                                   Tensor::Dimension outputChannelOffset)
{
  const std::string blob_name = "tb-" + std::to_string(m_pMeta.m_NumBlobs++);

  ILoadable::Blob b;
  b.name              = blob_name;
  b.size              = UNIT_ALIGNMENT(destDims.size() * ELEMENT_SIZE, WEIGHT_ATOM_CUBE_SIZE);
  b.version.major     = 0;
  b.version.minor     = 0;
  b.version.sub_minor = 0;
  b.interface         = ILoadable::Interface_NONE;
  b.subInterface      = 0;

  NvU8* blob_data = new NvU8[b.size];
  memset(blob_data, 0, b.size);

  // Pack weights here.
  if (const FloatTensor* floatTensor = dynamic_cast<const FloatTensor*>(&weight)) {
    const auto* srcData = reinterpret_cast<const float*>(floatTensor->getValues().data());

    packImageWeightImpl(reinterpret_cast<nv_weight_t<nvdla::ConfigSet::nv_full>*>(blob_data), destDims, &weight,
                        srcData, NvDlaDims(weight), outputChannelOffset);
  }

  m_pMeta.m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);

  const MemoryListEntryId memoryId = m_pMeta.allocateMemory(
    ILoadable::MemoryDomain_SYSMEM, ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_SET, b.size);

  ILoadable::MemoryListEntry& memory = m_pMeta.getMemoryListEntry(memoryId);
  memory.contents.push_back(blob_name);
  memory.offsets.push_back(0);

  return memoryId;
}

// lut_param->linear_exp_offset.exp_offset = 0
// lut_param->linear_exp_offset.frac_bits = 0
// lut_param->linear_exp_start = 0
// lut_param->linear_exp_end = 0
// lut_param->linear_only_offset.frac_bits = 0
// lut_param->linear_only_start = 1
// lut_param->linear_only_end = 1
// lut_param->method=LUT_METHOD_EXPONENTIAL
// REVIEW:
//   1. fp32 to fp16 conversion
//   2. pack LRN into function, pass function pointer and parameters?
//   3. General function todos?
//   4. How to verify?
//   5. Documentation or Description on how to setup 8
void CodeEmitVisitor::SetLUTParam(dla_lut_param* lut_param, float alpha, float beta, float bias, int fsize, float outdata_scale, float outdata_offset)
{
  // LUT X contents
  float indata;
  if (lut_param->linear_exp_offset.exp_offset > 0)
    indata = (float)(1 << lut_param->linear_exp_offset.exp_offset);
  else
    indata = 1.0 / (1 << (-lut_param->linear_exp_offset.exp_offset));

  for (int i = 0; i < 65; i++) {
    float outdata = 1.0f / std::pow((bias + (alpha * indata / fsize)), beta);

    if (lut_param->method == LUT_METHOD_EXPONENTIAL)
      indata *= 2.0f;
    else
      indata += (1 << lut_param->linear_exp_offset.frac_bits);

    lut_param->linear_exp_table[i] = f2float16_ieee(outdata);
  }

  lut_param->hybrid_priority    = 0;
  lut_param->underflow_priority = 0;
  lut_param->overflow_priority  = 0;

  // calculate slope
  // for LRN and sigmoid, overflow and under flow are all zero
  // two sets are union, only one can be set at a time
  lut_param->linear_exp_underflow_slope.data_f = 0.000000;
  lut_param->linear_exp_overflow_slope.data_f  = 0.000000;

  lut_param->linear_only_underflow_slope.data_f = 0.000000;
  lut_param->linear_only_overflow_slope.data_f  = 0.000000;
}

std::pair<unsigned, bool> CodeEmitVisitor::tryAllocateDataAndWeightsIntoCBuf(const NvDlaCubeInfo& data,
                                                                             NvDlaCubeInfo&       weight,
                                                                             Tensor::Dimension    yDilation) const
{
  unsigned minNumNeededDataBanks = 0;
  bool     shouldReuseWeight     = false;

  const CbufAllocType cbufAllocType = m_CbufAllocTypeGetter(data, weight, yDilation, minNumNeededDataBanks);
  switch (cbufAllocType) {
  case CbufAllocType::kFullDataFullWeight:
    // Full data and full weights
    weight.setBanksForFullWeights();
    shouldReuseWeight = true;
    break;
  case CbufAllocType::kFullDataPartialWeight:
    // Full data and partial weights
    weight.setBanksForPartialWeights();
    break;
  case CbufAllocType::kFullDataMinimumWeight:
    // Full data and minimum weights
    weight.setBanksForMinimumWeights();
    break;
  case CbufAllocType::kSplitDataFullWeight:
    // Split data and full weights
    weight.setBanksForFullWeights();
    shouldReuseWeight = true;
    break;
  case CbufAllocType::kSplitDataPartialWeight:
    // Split data and partial weights
    weight.setBanksForPartialWeights();
    break;
  case CbufAllocType::kSplitDataMinimumWeight:
    // Split data and minimum weights
    weight.setBanksForMinimumWeights();
    break;
  case CbufAllocType::kUnfeasible:
    weight.setBanksForMinimumWeights();
    std::ostringstream os;
    os << "data_banks(" << minNumNeededDataBanks << ") + weight_banks(" << weight.banks << ") > " << CBUF_BANK_NUM;
    fatal(nvdla_exceed_hardware_limit) << os.str();
    break;
  }

  return std::make_pair(minNumNeededDataBanks, shouldReuseWeight);
}

} // namespace nvdla
} // namespace onnc
