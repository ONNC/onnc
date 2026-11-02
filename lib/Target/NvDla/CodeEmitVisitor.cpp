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
#include <onnc/IR/Compute/Max.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/Min.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Flatten.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/IR/Compute/Split.h>
#include <onnc/IR/Compute/Squeeze.h>
#include <onnc/IR/Compute/Transpose.h>
#include <onnc/IR/Compute/Sum.h>
#include <onnc/IR/Compute/Unsqueeze.h>
#include <onnc/JSON/Object.h>
#include <onnc/JSON/Reader.h>
#include <onnc/JSON/Value.h>
#include <onnc/Support/Algorithm.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Match.h>
#include <onnc/Support/Preprocessor.h>
#include <onnc/Support/Range.h>
#include <onnc/Support/String.h>
#include <onnc/Support/View.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>
#include <type_traits>
#include <unordered_map>
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

#define PP_DEFINE_VISIT_CPU(type, op)                                             \
  void CodeEmitVisitor::visitImplCPU(const type& op)

#define PP_CALL_VISIT_CPU(op)                                                     \
  visitImplCPU(op)

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
  case 0:
    return BroadcastCategory::LAYER;
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

//===----------------------------------------------------------------------===//
// INT8 requantization primitives
//
// NVDLA is integer-only: a conv accumulates int8 x int8 -> int32, then an SDP
// "converter" (dla_cvt_param { int16_t scale; uint8_t truncate; ... }) rescales
// back to int8 with a fixed-point multiply:
//
//     out_i8 = saturate_round( (acc - offset) * scale >> truncate )
//
// For a conv layer the real output rescale is
//
//     R = SF_in * SF_wt / SF_out         (per the NVDLA precision spec)
//
// where SF_in/SF_out are per-tensor activation scales (calib.json) and SF_wt is
// the per-output-filter weight scale (per-channel / per-kernel, computed in
// packWeight). These helpers decompose a real ratio into the exact hardware
// field types: int16 scale, uint8 truncate (shift).
//===----------------------------------------------------------------------===//

struct CvtParams
{
  std::int16_t scale;
  std::uint8_t truncate;
};

constexpr std::int16_t kCvtScaleMax = 32767;   // INT16_MAX
constexpr std::int16_t kCvtScaleMin = -32768;  // INT16_MIN
// The dla_cvt_param truncate field is stored in a uint8, but the NVDLA hardware
// converter shifter is a 5-bit unsigned integer (UINT5, range [0,31]) -- see the
// NVDLA precision spec (nvdla.org/hw/v1/ias/precision.html: "shifter is a 5 bits
// unsigned integer") and the nvdla_compiler decomposition
// (calculateScaleAndShiftFromScalar warns for shifts > 31). Cap the search at 31
// so every shift we emit is valid for the hardware.
constexpr int kCvtTruncateMax = 31;

// Decompose a real rescale ratio R >= 0 into R ~= scale / 2^truncate with scale
// in int16 and truncate in uint8. Picks the LARGEST truncate that keeps |scale|
// within int16 for maximum precision (the standard NVDLA decomposition). A
// degenerate ratio maps to the identity converter (scale 1, truncate 0).
inline CvtParams realToCvt(double ratio)
{
  if (!(ratio > 0.0) || !std::isfinite(ratio)) {
    return CvtParams{1, 0};
  }
  for (int shift = kCvtTruncateMax; shift >= 0; --shift) {
    const double scaled = std::round(std::ldexp(ratio, shift)); // ratio * 2^shift
    if (std::fabs(scaled) <= static_cast<double>(kCvtScaleMax)) {
      return CvtParams{static_cast<std::int16_t>(scaled), static_cast<std::uint8_t>(shift)};
    }
  }
  // Ratio too large even at shift 0 -> saturate the scale.
  return CvtParams{kCvtScaleMax, 0};
}

// Per-kernel converter: NVDLA's out_cvt holds a single per-tensor shift, so a
// per-channel rescale is expressed as one SHARED truncate plus a per-channel
// int16 multiplier (the SDP per-kernel operand). Pick the largest shared shift
// that keeps every channel's multiplier within int16, then quantize each ratio.
struct PerChannelCvt
{
  std::uint8_t              truncate;
  std::vector<std::int16_t> mult; // one int16 multiplier per output channel
};

inline PerChannelCvt realToPerChannelCvt(const std::vector<double>& ratios)
{
  PerChannelCvt out;
  out.truncate = 0;
  out.mult.assign(ratios.size(), 0);
  if (ratios.empty()) {
    return out;
  }

  double maxRatio = 0.0;
  for (const double r : ratios) {
    if (std::isfinite(r) && r > maxRatio) {
      maxRatio = r;
    }
  }
  if (!(maxRatio > 0.0)) {
    return out; // all-zero ratios -> zero multipliers
  }

  int shared = 0;
  for (int shift = kCvtTruncateMax; shift >= 0; --shift) {
    if (std::round(std::ldexp(maxRatio, shift)) <= static_cast<double>(kCvtScaleMax)) {
      shared = shift;
      break;
    }
  }
  out.truncate = static_cast<std::uint8_t>(shared);

  for (std::size_t k = 0; k < ratios.size(); ++k) {
    double m = std::round(std::ldexp(ratios[k], shared));
    m        = std::max(static_cast<double>(kCvtScaleMin), std::min(static_cast<double>(kCvtScaleMax), m));
    out.mult[k] = static_cast<std::int16_t>(m);
  }
  return out;
}

// Fit a PDP (pooling) op's input and right/bottom padding to what its windows actually use, exactly as
// nvdla_compiler does (sw/umd/core/src/compiler/engine-ast/PDPNode.cpp: adjustBRPadding, and emitOp
// lines 584-591). The padding programmed is the amount the windows need,
//   pad_right = (out_w - 1) * stride_x + kernel_w - (in_w + pad_left)   (same for bottom),
// and when that is negative -- ONNX floor mode leaving trailing input columns/rows that no window
// covers, e.g. 8x8 -> 3x3 with a 3x3/2 window uses only 7x7 -- the input is shrunk by that amount and
// the padding set to 0. The line/surface strides stay those of the full tensor, so the PDP reads the
// top-left sub-cube. Without this the PDP reads input it never uses; the RTL reports the op done when
// its last output is written, before that input has drained, and the leftover hangs the next PDP op
// (SqueezeNet op 16 -> op 53). Logs every change against the ONNX values.
inline void fitPdpInputToWindows(dla_pdp_op_desc& desc, dla_pdp_surface_desc& surf, int out_w, int out_h)
{
  const int in_w = surf.src_data.width;
  const int in_h = surf.src_data.height;
  const int pad_r = (out_w - 1) * desc.stride_x + (desc.pool_width + 1) - (in_w + desc.pad_left);
  const int pad_b = (out_h - 1) * desc.stride_y + (desc.pool_height + 1) - (in_h + desc.pad_top);

  const int new_w  = (pad_r < 0) ? in_w + pad_r : in_w;
  const int new_h  = (pad_b < 0) ? in_h + pad_b : in_h;
  const int new_pr = std::max(pad_r, 0);
  const int new_pb = std::max(pad_b, 0);

  if (new_w != in_w || new_h != in_h || new_pr != desc.pad_right || new_pb != desc.pad_bottom) {
    errs() << "[NvDla][pdp] fit input to pooling windows: in " << in_h << "x" << in_w << " -> " << new_h
           << "x" << new_w << ", pad_bottom " << int(desc.pad_bottom) << " -> " << new_pb << ", pad_right "
           << int(desc.pad_right) << " -> " << new_pr << " (out " << out_h << "x" << out_w << ")\n";
  }
  surf.src_data.width  = static_cast<std::uint16_t>(new_w);
  surf.src_data.height = static_cast<std::uint16_t>(new_h);
  desc.pad_right       = static_cast<std::uint8_t>(new_pr);
  desc.pad_bottom      = static_cast<std::uint8_t>(new_pb);
}

} // namespace internal

using namespace internal;

namespace nvdla {

#include "CodeEmitVisitor/Add.inc"
#include "CodeEmitVisitor/AddEMU.inc"
#include "CodeEmitVisitor/AveragePool.inc"
#include "CodeEmitVisitor/AveragePoolEMU.inc"
#include "CodeEmitVisitor/Concat.inc"
#include "CodeEmitVisitor/ConcatEMU.inc"
#include "CodeEmitVisitor/Conv.inc"
#include "CodeEmitVisitor/ConvEMU.inc"
#include "CodeEmitVisitor/Initializer.inc"
#include "CodeEmitVisitor/InputOperator.inc"
#include "CodeEmitVisitor/LRN.inc"
#include "CodeEmitVisitor/MaxPool.inc"
#include "CodeEmitVisitor/MaxPoolEMU.inc"
#include "CodeEmitVisitor/Mul.inc"
#include "CodeEmitVisitor/MulEMU.inc"
#include "CodeEmitVisitor/NvDlaShuffle.inc"
#include "CodeEmitVisitor/OutputOperator.inc"
#include "CodeEmitVisitor/Relu.inc"
#include "CodeEmitVisitor/ReluEMU.inc"
#include "CodeEmitVisitor/Reshape.inc"
#include "CodeEmitVisitor/Softmax.inc"
#include "CodeEmitVisitor/Split.inc"
#include "CodeEmitVisitor/Sum.inc"
#include "CodeEmitVisitor/Transpose.inc"

const std::vector<float>* CodeEmitVisitor::getWeightScales(const Tensor* weight) const
{
  const auto it = m_WeightScales.find(weight);
  return (it == m_WeightScales.end()) ? nullptr : &it->second;
}

float CodeEmitVisitor::getActivationScale(const std::string& tensorName) const
{
  const auto it = m_ActivationScales.find(tensorName);
  return (it == m_ActivationScales.end()) ? 0.0f : it->second;
}

float CodeEmitVisitor::getActivationScaleResolved(const Tensor* tensor) const
{
  if (tensor == nullptr) {
    return 0.0f;
  }
  const float direct = getActivationScale(tensor->getName());
  if (direct > 0.0f) {
    return direct;
  }
  // Shaping ops are scale-preserving (Part E). A tensor produced just before one
  // may be absent from calib.json under its own name (e.g. ReplaceGemmByConv's
  // `replace_gemm_by_conv_N` conv output, reshaped to the calibrated graph
  // output) OR a Relu (intermediate gemm->conv+relu fuses to the relu-OUTPUT scale).
  auto& uses = const_cast<Tensor*>(tensor)->getUses();
  if (uses.size() == 1) {
    ComputeOperator* user = uses[0].getUser();
    if (isa<Reshape>(user) || isa<Squeeze>(user) || isa<Unsqueeze>(user) || isa<Flatten>(user) ||
        isa<Transpose>(user) || isa<Relu>(user)) {
      return getActivationScaleResolved(static_cast<const Tensor*>(user->getOutput(0)));
    }
  }
  return 0.0f;
}

void CodeEmitVisitor::loadCalibrationTable()
{
  // Only INT8 needs activation scales; FP16 ignores the calibration table.
  if (DLA_PRECISION != PRECISION_INT8) {
    return;
  }

  const char* const path = std::getenv("ONNC_NVDLA_CALIB");
  if (path == nullptr || *path == '\0') {
    return;
  }

  // Called from a noexcept constructor: never let an exception escape. A missing
  // or malformed table just yields an empty map (the converters then fall back).
  try {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
      errs() << "[NvDla][int8] cannot open calibration table: " << path << "\n";
      return;
    }
    const std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    json::Reader reader;
    json::Value  root;
    if (!reader.read(StringRef(content.data(), content.size()), root) || !root.isObject()) {
      errs() << "[NvDla][int8] failed to parse calibration table: " << path << "\n";
      return;
    }

    // calib.json schema: accepts NVDLA's native FLAT table (resnet50.json:
    // { "<tensor>": {"scale":...} }) AND this repo's wrapped { "data": {...} }.
    // Disambiguate on top["data"]: the wrapper lacks a "scale" key; a flat tensor
    // named "data" (ResNet-50 input) HAS "scale". Only the symmetric "scale" used.
    const json::Object& top = root.toObject();
    const json::Object* tablePtr = &top;
    if (top.hasValue("data") && top.get("data").isObject() &&
        !top.get("data").toObject().hasValue("scale"))
      tablePtr = &top.get("data").toObject();
    const json::Object& data = *tablePtr;
    for (json::Object::const_iterator it = data.begin(), end = data.end(); it != end; ++it) {
      const json::Value& entry = it->value();
      if (!entry.isObject()) {
        continue;
      }
      const json::Object& tensorObj = entry.toObject();
      if (!tensorObj.hasValue("scale")) {
        continue;
      }
      const float scale = static_cast<float>(tensorObj.get("scale").toFloating());
      m_ActivationScales.emplace(it->key().str(), scale);
    }

    outs() << "[NvDla][int8] loaded " << m_ActivationScales.size() << " activation scale(s) from " << path << "\n";
  } catch (const std::exception& e) {
    errs() << "[NvDla][int8] error reading calibration table: " << e.what() << "\n";
  } catch (...) {
    errs() << "[NvDla][int8] unknown error reading calibration table\n";
  }
}

const std::vector<float>& CodeEmitVisitor::computeWeightScales(const Tensor* weightTensor, span<const float> weight,
                                                               NvDlaDims srcDims)
{
  // Symmetric, per-output-filter weight quantization (the per-channel scheme):
  // SF_wt[k] = max_abs(filter_k) / 127, so that round(w / SF_wt[k]) lands in
  // [-127, 127]. The int8 weights and the SF_wt are what NVDLA's converters use
  // later to realize out_rescale = SF_in * SF_wt / SF_out per layer.
  const auto cached = m_WeightScales.find(weightTensor);
  if (cached != m_WeightScales.end()) {
    return cached->second;
  }

  const int K           = srcDims.n;                          // output filters
  const int elemsPerFilt = srcDims.c * srcDims.h * srcDims.w; // C*H*W per filter

  std::vector<float> scales(K, 1.0f);
  for (int k = 0; k < K; ++k) {
    float maxAbs = 0.0f;
    for (int i = 0; i < elemsPerFilt; ++i) {
      const float v = std::fabs(weight[k * elemsPerFilt + i]);
      if (v > maxAbs) {
        maxAbs = v;
      }
    }
    // All-zero filter -> keep scale 1 (the quantized weights are all 0 anyway)
    // and avoid a divide-by-zero in the packing loop.
    scales[k] = (maxAbs > 0.0f) ? (maxAbs / 127.0f) : 1.0f;
  }

  return m_WeightScales.emplace(weightTensor, std::move(scales)).first->second;
}

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

  // INT8 lays weights out as 1-byte int8 (ELEMENT_SIZE = 1) and quantizes them
  // per output filter; FP16 keeps the 2-byte half path byte-for-byte unchanged.
  if (DLA_PRECISION == PRECISION_INT8) {
    computeWeightScales(weightTensor, weight, srcDims); // memoize SF_wt for packWeightImpl + converters
    using weight_type = nv_weight_t<nvdla::ConfigSet::nv_full_int8>;
    packWeightImpl(reinterpret_cast<weight_type*>(blob_data), destDimsWithFrontPadding, weightTensor, srcData, srcDims,
                   numFrontPaddingChannels, outputChannelOffset);
  } else {
    using weight_type = nv_weight_t<nvdla::ConfigSet::nv_full>;
    packWeightImpl(reinterpret_cast<weight_type*>(blob_data), destDimsWithFrontPadding, weightTensor, srcData, srcDims,
                   numFrontPaddingChannels, outputChannelOffset);
  }

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

  // INT8 bias must be quantized into the int32 accumulator domain,
  // round(bias / (SF_in * SF_wt)), which needs the activation scale SF_in from
  // the calibration table. That is wired in a later patch (calib ingest + bias);
  // here we leave the INT8 bias blob zero-initialized rather than write fp16
  // bytes into it. FP16 keeps the existing half-precision bias path.
  if (DLA_PRECISION != PRECISION_INT8) {
    if (const FloatTensor* floatTensor = dynamic_cast<const FloatTensor*>(&bias)) {
      const auto* srcData = reinterpret_cast<const float*>(floatTensor->getValues().data());

      using weight_type           = nv_weight_t<nvdla::ConfigSet::nv_full>;
      weight_type* const destData = reinterpret_cast<weight_type*>(blob_data);

      packBiasImpl(destData, numDestChannels, &bias, srcData, srcChannelOffset);
    }
  }

  m_pMeta.m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);

  const MemoryListEntryId memoryId = m_pMeta.allocateMemory(
    ILoadable::MemoryDomain_SYSMEM, ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_SET, b.size);

  ILoadable::MemoryListEntry& memory = m_pMeta.getMemoryListEntry(memoryId);
  memory.contents.push_back(blob_name);
  memory.offsets.push_back(0);

  return memoryId;
}

MemoryListEntryId CodeEmitVisitor::packRequantMul(const std::vector<std::int16_t>& mult,
                                                  Tensor::Dimension numDestChannels, Tensor::Dimension srcChannelOffset)
{
  const std::string blob_name = "tb-" + std::to_string(m_pMeta.m_NumBlobs++);

  ILoadable::Blob b;
  b.name              = blob_name;
  // One int16 multiplier per output channel (the SDP per-kernel MUL operand is a
  // 2-byte cube regardless of the int8 feature/weight datapath).
  b.size              = sizeof(std::int16_t) * UNIT_ALIGNMENT(numDestChannels, MAC_ATOMIC_K);
  b.version.major     = 0;
  b.version.minor     = 0;
  b.version.sub_minor = 0;
  b.interface         = ILoadable::Interface_NONE;
  b.subInterface      = 0;

  NvU8* blob_data = new NvU8[b.size];
  std::memset(blob_data, 0, b.size);

  // NVDLA reads cube data little-endian; reinterpret as int16 on the (LE) host.
  std::int16_t* const destData = reinterpret_cast<std::int16_t*>(blob_data);
  for (Tensor::Dimension c = 0; c < numDestChannels; ++c) {
    const std::size_t k = static_cast<std::size_t>(srcChannelOffset) + c;
    destData[c]         = (k < mult.size()) ? mult[k] : static_cast<std::int16_t>(0);
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

  // Input-channel cube width = the HW MAC accumulation atom (MAC_ATOMIC_C = 64), NOT
  // WEIGHT_ATOM_CUBE_SIZE/ELEMENT_SIZE. The two happen to coincide for FP16 (128/2 = 64)
  // but diverge for INT8 (128/1 = 128 != 64). The CSC weight fetch walks the blob in
  // 64-channel blocks (ELEMENT_PER_INPUT_ATOM=64, block_operation_iter over
  // ceil(C/64) blocks -- vendor/nvdla-hw/cmod/csc/NV_NVDLA_csc.cpp:1062,1145-1182), and
  // the reference compiler tiles channels by atomicCSize=64 with the kernel loop nested
  // inside each block (vendor/nvdla-sw .../WeightTranslationUnit.h prep/execWtTrnsOpsForDC).
  // Using 128 for INT8 packs all 128 channels as one contiguous cube per kernel, so the
  // second 64-channel accumulation block reads the wrong weights -> scrambled output for
  // convs with > 64 input channels (docs/08 s8b). MAC_ATOMIC_C is correct for both.
  const int channel_per_cube = MAC_ATOMIC_C;
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

            if (DLA_PRECISION == PRECISION_INT8) {
              // Absolute output filter index, used to pick this filter's scale.
              const int                 kAbs   = n * MAC_ATOMIC_K + n_ofs + outputChannelOffset;
              const std::vector<float>& scales = m_WeightScales.at(tensor);
              *(destData + dest_ofs)           = static_cast<weight_t>(f2int8_ieee(*(srcData + src_ofs) / scales[kAbs]));
            } else {
              *(destData + dest_ofs) = f2float16_ieee(*(srcData + src_ofs));
            }
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
  if (m_pMeta.isNewTaskBoundary(NvDlaBackendMeta::OperationMeta::Category::dla)) {
    // Each DLA task block should start with a fresh dependency context and a task-local op index space.
    // Task-to-task ordering is handled by the loadable submit/event graph,
    // not by op-level consumer links across task boundaries.
    m_pMeta.resetCurrentTaskDlaState();
    m_pMeta.m_pPrevOp = nullptr;
    std::fill(std::begin(m_pMeta.m_pDepOp), std::end(m_pMeta.m_pDepOp), nullptr);
  }

  struct dla_common_op_desc* op_desc = &(op->op_dep);
  int                        op_type = op_desc->op_type;
  // op_desc->index            = m_pMeta.m_DLAOperationList.size();
  const std::size_t          op_index = m_pMeta.nextDlaTaskLocalIndex();
  op_desc->index            = op_index;
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
                                                ? static_cast<int16_t>(op_index)
                                                : m_pMeta.m_DlaNetworkDesc.op_head[op_type];

  m_pMeta.m_DLAOperationList.push_back(op);
  m_pMeta.m_pDepOp[op_type] = op;

  m_pMeta.appendOperationMeta(m_pMeta.m_DLAOperationList.size() - 1,
                              NvDlaBackendMeta::OperationMeta::Category::dla);

  if (op_fuse != NULL) {
    struct dla_common_op_desc* fuse_op_desc = &(op_fuse->op_dep);
    int                        op_fuse_type = fuse_op_desc->op_type;
    // fuse_op_desc->index                     = m_pMeta.m_DLAOperationList.size();
    const std::size_t          fuse_index = m_pMeta.nextDlaTaskLocalIndex();
    fuse_op_desc->index                     = fuse_index;
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
                                                       ? static_cast<int16_t>(fuse_index)
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

void CodeEmitVisitor::emitSdp(const ComputeOperator& op, const Tensor& first, const Tensor& second, const Tensor& output)
{
  assert(!(isConstant(first) && isConstant(second)) && "cannot support 2 constant tensors");
  assert(isa<Add>(&op) || isa<Mul>(&op) || isa<Max>(&op) || isa<Min>(&op));

  // make sure the 'first' tensor is always non-constant
  if (isConstant(first)) {
    emitSdp(op, second, first, output);
    return;
  }

  assert(!isConstant(first));

  const BroadcastCategory category =
    (isConstant(second) ? getBroadcastCategory(second, first) : getBroadcastCategory(first, second));
  auto operation = makeNvDlaOp(NvDlaOpType::sdp);

  std::uint8_t opType = isa<Mul>(&op) ? SDP_OP_MUL : SDP_OP_ALU;

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
  desc.x1_op.alu_type    = (isa<Max>(&op) ? SDP_ALU_OP_MAX : (isa<Min>(&op) ? SDP_ALU_OP_MIN : SDP_ALU_OP_SUM));
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
    case SDP_OP_ALU:
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

  // INT8 requant for the eltwise SDP ops (Add/Mul/Max/Min). The fp16 path above
  // left out_cvt=1 (and fp16 operands); for int8 we recompute the converters from
  // the calibrated scales so the op lands in the output scale SF_out. This matches
  // nvdla_compiler's SDPEltWiseOp::performPerTensorRescaling, which programs ONLY a
  // single out_cvt and has NO per-operand converter -- it requires the two inputs
  // to already share one scale (it asserts equal input scales). We make that hold
  // by enforcing equal scales on additive-eltwise inputs in 05b (like Concat), so:
  //   Add/Max/Min, two features : inputs share SF_in -> out_cvt = realToCvt(SF_in/SF_out)
  //       (no operand cvt -- exactly nvdla's form; resolves the old X1-cvt assumption).
  //   Mul, two features         : a true multiply lets the scales multiply, so no
  //       enforced equality -> out_cvt = realToCvt(SF_a*SF_b/SF_out), no operand cvt.
  //   const operand (LAYER)     : the scalar c is quantized to the input scale --
  //       Add/Max/Min -> alu_operand = round(c/SF_a), out_cvt = realToCvt(SF_a/SF_out)
  //                      (e.g. Clip->Max(0)/Min(6) for ReLU6);
  //       Mul         -> mul_operand = 1,             out_cvt = realToCvt(c*SF_a/SF_out).
  // See docs/ONNC_INT8_PLAN.md Part D (nvdla source) + Part E.
  if (DLA_PRECISION == PRECISION_INT8) {
    const float sfA   = getActivationScale(first.getName());
    const float sfOut = getActivationScale(output.getName());
    if (sfA > 0.0f && sfOut > 0.0f) {
      internal::CvtParams outCvt{1, 0};
      bool                handled = true;
      if (category == BroadcastCategory::LAYER) {
        const float c = to_<std::vector<float>>(second).front();
        if (isa<Mul>(&op)) {
          desc.x1_op.mul_operand = 1;
          outCvt = internal::realToCvt(static_cast<double>(c) * sfA / sfOut);
        } else {
          desc.x1_op.alu_operand = static_cast<int32_t>(std::lround(static_cast<double>(c) / sfA));
          outCvt                 = internal::realToCvt(static_cast<double>(sfA) / sfOut);
        }
      } else {
        const float sfB = getActivationScale(second.getName());
        handled         = (sfB > 0.0f);
        if (handled && isa<Mul>(&op)) {
          outCvt = internal::realToCvt(static_cast<double>(sfA) * sfB / sfOut);
        } else if (handled) {
          // Add/Max/Min: inputs are enforced to one scale upstream (05b), so a
          // single out_cvt suffices -- no per-operand X1 converter (matches nvdla).
          outCvt = internal::realToCvt(static_cast<double>(sfA) / sfOut);
        }
      }
      if (handled) {
        desc.out_cvt.scale    = outCvt.scale;
        desc.out_cvt.truncate = outCvt.truncate;
        const char* kind = isa<Mul>(&op) ? "mul" : (isa<Max>(&op) ? "max" : (isa<Min>(&op) ? "min" : "add"));
        outs() << "[NvDla][int8] eltwise-" << kind << " " << output.getName() << " : SF_a=" << sfA
               << " SF_out=" << sfOut << (category == BroadcastCategory::LAYER ? " const" : "")
               << " out_cvt=" << outCvt.scale << "/" << static_cast<int>(outCvt.truncate) << "\n";
      }
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

void CodeEmitVisitor::emitEmuSdp(const ComputeOperator& op, const Tensor& first, const Tensor& second, const Tensor& output)
{
  assert(!(isConstant(first) && isConstant(second)) && "cannot support 2 constant tensors");
  assert(isa<Add>(&op) || isa<Mul>(&op) || isa<Max>(&op) || isa<Min>(&op));

  // make sure the 'first' tensor is always non-constant
  if (isConstant(first)) {
    emitEmuSdp(op, second, first, output);
    return;
  }

  assert(!isConstant(first));

  const BroadcastCategory category =
    (isConstant(second) ? getBroadcastCategory(second, first) : getBroadcastCategory(first, second));
  // auto operation = makeNvDlaOp(NvDlaOpType::sdp);
  NvDlaEmuOperation* const operation = new NvDlaEmuOperation();

  std::uint8_t opType = isa<Mul>(&op) ? SDP_OP_MUL : SDP_OP_ALU;

  // auto& desc             = getDesc<NvDlaOpType::sdp>(*operation);
  struct emu_sdp_op_desc* desc = (struct emu_sdp_op_desc*)(&(operation->op_desc));
  struct emu_common_op_desc          common;
  std::memset(&common, 0, sizeof(common));
  common.op_type          = NVDLA_EMU_OP_SDP;
  desc->common            = common;

  // desc->src_precision     = 3; // DLA_PRECISION = 2 but EMU_FORMAT_FF16 = 3
  /**
   * Temporary solution.
   * Attempt to map dla_interface precision consts
   * to emu_interface ones.
   */
  desc->src_precision     = DLA_PRECISION + 1;
  desc->dst_precision     = 3; // DLA_PRECISION = 2 but EMU_FORMAT_FF16 = 3
  desc->lut_index         = -1;
  desc->out_cvt.scale     = 1;
  desc->out_cvt.truncate  = 0;
  desc->out_cvt.enable    = 1;
  desc->out_cvt.offset    = 0;
  desc->conv_mode         = CONV_MODE_DIRECT;
  desc->batch_num         = 1;
  desc->batch_stride      = 0;
  desc->x1_op.enable      = 1;
  desc->x1_op.alu_type    = (isa<Max>(&op) ? SDP_ALU_OP_MAX : (isa<Min>(&op) ? SDP_ALU_OP_MIN : SDP_ALU_OP_SUM));
  desc->x1_op.type        = opType;
  desc->x1_op.mode        = getSdpOpMode(category);
  desc->x1_op.act         = ACTIVATION_NONE;
  desc->x1_op.shift_value = 0;
  desc->x1_op.truncate    = 0;
  desc->x1_op.precision   = 3; // DLA_PRECISION = 2 but EMU_FORMAT_FF16 = 3;
  if (category == BroadcastCategory::LAYER) {
    const auto operand = to_<std::vector<float>>(second);
    assert(size(operand) == 1);

    switch (opType) {
    case SDP_OP_ALU:
      desc->x1_op.mul_operand = 0;
      desc->x1_op.alu_operand = f2float16_ieee(*begin(operand));
      break;
    case SDP_OP_MUL:
      desc->x1_op.alu_operand = 0;
      desc->x1_op.mul_operand = f2float16_ieee(*begin(operand));
      break;
    default:
      assert(false && "should not reach here");
    }
  }

  // INT8 requant for the eltwise SDP ops (Add/Mul/Max/Min). The fp16 path above
  // left out_cvt=1 (and fp16 operands); for int8 we recompute the converters from
  // the calibrated scales so the op lands in the output scale SF_out. This matches
  // nvdla_compiler's SDPEltWiseOp::performPerTensorRescaling, which programs ONLY a
  // single out_cvt and has NO per-operand converter -- it requires the two inputs
  // to already share one scale (it asserts equal input scales). We make that hold
  // by enforcing equal scales on additive-eltwise inputs in 05b (like Concat), so:
  //   Add/Max/Min, two features : inputs share SF_in -> out_cvt = realToCvt(SF_in/SF_out)
  //       (no operand cvt -- exactly nvdla's form; resolves the old X1-cvt assumption).
  //   Mul, two features         : a true multiply lets the scales multiply, so no
  //       enforced equality -> out_cvt = realToCvt(SF_a*SF_b/SF_out), no operand cvt.
  //   const operand (LAYER)     : the scalar c is quantized to the input scale --
  //       Add/Max/Min -> alu_operand = round(c/SF_a), out_cvt = realToCvt(SF_a/SF_out)
  //                      (e.g. Clip->Max(0)/Min(6) for ReLU6);
  //       Mul         -> mul_operand = 1,             out_cvt = realToCvt(c*SF_a/SF_out).
  // See docs/ONNC_INT8_PLAN.md Part D (nvdla source) + Part E.
  if (DLA_PRECISION == PRECISION_INT8) {
    outs() << "[DEBUG] sfA from tensor " << first.getName() << "\n";
    outs() << "[DEBUG] sfOut from tensor " << output.getName() << "\n";
    const float sfA   = getActivationScale(first.getName());
    const float sfOut = getActivationScale(output.getName());
    if (sfA > 0.0f && sfOut > 0.0f) {
      internal::CvtParams outCvt{1, 0};
      bool                handled = true;
      if (category == BroadcastCategory::LAYER) {
        const float c = to_<std::vector<float>>(second).front();
        if (isa<Mul>(&op)) {
          desc->x1_op.mul_operand = 1;
          outCvt = internal::realToCvt(static_cast<double>(c) * sfA / sfOut);
        } else {
          desc->x1_op.alu_operand = static_cast<int32_t>(std::lround(static_cast<double>(c) / sfA));
          outCvt                 = internal::realToCvt(static_cast<double>(sfA) / sfOut);
        }
      } else {
        const float sfB = getActivationScale(second.getName());
        outs() << "[DEBUG] sfB from tensor " << second.getName() << "\n";
        handled         = (sfB > 0.0f);
        if (handled && isa<Mul>(&op)) {
          outCvt = internal::realToCvt(static_cast<double>(sfA) * sfB / sfOut);
        } else if (handled) {
          // Add/Max/Min: inputs are enforced to one scale upstream (05b), so a
          // single out_cvt suffices -- no per-operand X1 converter (matches nvdla).
          outCvt = internal::realToCvt(static_cast<double>(sfA) / sfOut);
        }
      }
      if (handled) {
        desc->out_cvt.scale    = outCvt.scale;
        desc->out_cvt.truncate = outCvt.truncate;
        const char* kind = isa<Mul>(&op) ? "mul" : (isa<Max>(&op) ? "max" : (isa<Min>(&op) ? "min" : "add"));
        outs() << "[NvDla][int8] eltwise-" << kind << " " << output.getName() << " : SF_a=" << sfA
               << " SF_out=" << sfOut << (category == BroadcastCategory::LAYER ? " const" : "")
               << " out_cvt=" << outCvt.scale << "/" << static_cast<int>(outCvt.truncate) << "\n";
      }
    }
  }

  // auto& surface = getSurface<NvDlaOpType::sdp>(*operation);
  struct emu_sdp_buffer_descs* surface = (struct emu_sdp_buffer_descs*)(&(operation->op_buf));

  const NvDlaCubeInfo firstCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, first);
  // NvDlaDataCubeModifier(surface.src_data, NvDlaMemType::mc)
  //   .setSize(m_pMeta.getMemoryListEntrySize(first))
  //   .setAddress(issueDlaAddr(first, firstCubeInfo))
  //   .setInfo(firstCubeInfo);
  // surface->src_data.type         = static_cast<std::underlying_type<NvDlaMemType>::type>(NvDlaMemType::mc);
  // surface.src_data.address      = issueDlaAddr(first, firstCubeInfo);
  int input_mid = m_pMeta.getMemoryListEntryId(first);
  surface->src_data.addressIndex = issueEmuAddr(input_mid);
  surface->src_data.size         = m_pMeta.getMemoryListEntrySize(first);
  surface->src_data.format       = 3; // fp16
  surface->src_data.width        = firstCubeInfo.dim_w;
  surface->src_data.height       = firstCubeInfo.dim_h;
  surface->src_data.channel      = firstCubeInfo.dim_c;
  surface->src_data.line_stride  = firstCubeInfo.stride_line;
  surface->src_data.surf_stride  = firstCubeInfo.stride_surface;
  // surface->src_data.plane_stride = firstCubeInfo.stride_plane;

  if (category == BroadcastCategory::LAYER) {
    // NvDlaDataCubeModifier(surface->x1_data, NvDlaMemType::hw).setAddress(-1);
    // surface->src_data.type         = static_cast<std::underlying_type<NvDlaMemType>::type>(NvDlaMemType::hw);
    surface->x1_data.addressIndex = -1;
  } else {
    MemoryListEntryId   memoryId;
    const NvDlaCubeInfo secondCubeInfo = makeCubeInfo(*this, getSdpXSingleCubeType(second, DLA_PRECISION), second);
    // NvDlaDataCubeModifier(surface.x1_data, NvDlaMemType::mc)
    //   .setAddress(issueSDPOperand(second, secondCubeInfo, memoryId))
    //   .setSize(m_pMeta.getMemoryListEntrySize(memoryId))
    //   .setInfo(secondCubeInfo);
    // surface->x1_data.type         = static_cast<std::underlying_type<NvDlaMemType>::type>(NvDlaMemType::mc);
    // surface.x1_data.address      = issueSDPOperand(second, secondCubeInfo, memoryId);
    MemoryListEntryId x1_mid;
    if (isConstant(second)) {
      x1_mid = packSDPOperand(&second, nullptr, secondCubeInfo);
      // return issueDlaAddr(memoryId, secondCubeInfo);
    } else {
      x1_mid = m_pMeta.getMemoryListEntryId(second);
      // return issueDlaAddr(second, secondCubeInfo);
    }
    surface->x1_data.addressIndex = issueEmuAddr(x1_mid);
    surface->x1_data.format       = 3; // fp16
    // surface->x1_data.size         = m_pMeta.getMemoryListEntrySize(memoryId);
    surface->x1_data.size         = m_pMeta.getMemoryListEntrySize(x1_mid);
    surface->x1_data.width        = firstCubeInfo.dim_w;
    surface->x1_data.height       = firstCubeInfo.dim_h;
    surface->x1_data.channel      = firstCubeInfo.dim_c;
    surface->x1_data.line_stride  = firstCubeInfo.stride_line;
    surface->x1_data.surf_stride  = firstCubeInfo.stride_surface;
    // surface->x1_data.plane_stride = firstCubeInfo.stride_plane;
  }

  const NvDlaCubeInfo outputCubeInfo = makeCubeInfo(*this, NVDLA_CUBE_FEATURE, output);
  // NvDlaDataCubeModifier(surface.dst_data, NvDlaMemType::mc)
  //   .setSize(m_pMeta.getMemoryListEntrySize(output))
  //   .setAddress(issueDlaAddr(output, outputCubeInfo))
  //   .setInfo(outputCubeInfo);
  // surface->dst_data.type         = static_cast<std::underlying_type<NvDlaMemType>::type>(NvDlaMemType::mc);
  // surface.dst_data.address      = issueDlaAddr(output, outputCubeInfo);
  int output_mid = m_pMeta.getMemoryListEntryId(output);
  surface->dst_data.addressIndex = issueEmuAddr(output_mid);
  surface->dst_data.format       = 3; // fp16
  surface->dst_data.size         = m_pMeta.getMemoryListEntrySize(output);
  surface->dst_data.width        = outputCubeInfo.dim_w;
  surface->dst_data.height       = outputCubeInfo.dim_h;
  surface->dst_data.channel      = outputCubeInfo.dim_c;
  surface->dst_data.line_stride  = outputCubeInfo.stride_line;
  surface->dst_data.surf_stride  = outputCubeInfo.stride_surface;
  // surface->dst_data.plane_stride = outputCubeInfo.stride_plane;

  // issueDlaOp(std::move(operation));
  issueEmuOp(operation);
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

          if (DLA_PRECISION == PRECISION_INT8) {
            const int                 kAbs   = outputChannelOffset + k; // absolute output filter
            const std::vector<float>& scales = m_WeightScales.at(tensor);
            *(blob + blob_ofs)               = static_cast<weight_t>(f2int8_ieee(*(srcData + src_ofs) / scales[kAbs]));
          } else {
            *(blob + blob_ofs) = (weight_t)f2float16_ieee(*(srcData + src_ofs));
          }
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

    if (DLA_PRECISION == PRECISION_INT8) {
      computeWeightScales(&weight, floatTensor->getValues(), NvDlaDims(weight));
      packImageWeightImpl(reinterpret_cast<nv_weight_t<nvdla::ConfigSet::nv_full_int8>*>(blob_data), destDims, &weight,
                          srcData, NvDlaDims(weight), outputChannelOffset);
    } else {
      packImageWeightImpl(reinterpret_cast<nv_weight_t<nvdla::ConfigSet::nv_full>*>(blob_data), destDims, &weight,
                          srcData, NvDlaDims(weight), outputChannelOffset);
    }
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
