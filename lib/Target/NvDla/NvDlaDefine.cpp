//===- NvDlaDefine.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaDefine.h"

#include "NvDlaMeta.h"
#include "half.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

namespace onnc {

bool operator==(const NvDlaDims& lhs, const NvDlaDims& rhs)
{
  return lhs.n == rhs.n && lhs.c == rhs.c && lhs.h == rhs.h && lhs.w == rhs.w;
}

bool operator!=(const NvDlaDims& lhs, const NvDlaDims& rhs) { return !(lhs == rhs); }

std::uint16_t f2float16_ieee(float param) { return half_float::detail::float2half<std::round_toward_zero>(param); }

std::int8_t f2int8_ieee(float param)
{
  unsigned int param_bit = *((unsigned int*)(&param));
  int          sign      = param_bit >> 31;
  int          mantissa  = param_bit & 0x007FFFFF;
  int          exp_bit   = (param_bit & 0x7F800000) >> 23;
  int          exp       = exp_bit - 127;
  int8_t       res;

  if (exp_bit == 0 && mantissa == 0) { // exactly zero
    res = 0;
  } else if (exp_bit == 0x000000FF && mantissa == 0) { // infinity
    res = 127;
  } else if (exp_bit == 0 && mantissa == 0x007FFFFF) { // denormalised
    assert(0 && "Do not handle denormalised case.");

    if (sign == 0 && mantissa > 127) { // > 127
      res = 127;
    } else if (sign == 1 && mantissa > 128) { // < -128
      res = -128;
    } else {
      res = (sign) ? -mantissa : mantissa;
    }
  } else if (exp_bit == 0x000000FF && mantissa == 0x007FFFFF) { // NaN
    assert(0 && "Cannot handle NaN.");
    res = 0;
  } else if (exp < -1) { // underflow
    res = 0;
  } else if (exp == -1) {
    // 0.5xxx and -0.5xxx. After rounding, get 1 and -1.
    res = (sign) ? -1 : 1;
  } else if (exp > 6) { // overflow
    res = (sign) ? -128 : 127;
  } else {
    int32_t v = (0x80 | ((mantissa & 0x007F0000) >> 16)) >> (6 - exp);

    if (v & 0x1) { // roundup
      v += 1;
    }
    v = v >> 1;
    if (sign)
      v = -v;
    if (v > 127)
      res = 127;
    else if (v < -128)
      res = -128;
    else
      res = v;
  }
  return res;
}

std::int16_t f2int16_ieee(float param)
{
  unsigned int  param_bit = *((unsigned int*)(&param));
  int           sign      = param_bit >> 31;
  int           mantissa  = param_bit & 0x007FFFFF;
  int           exp_bit   = (param_bit & 0x7F800000) >> 23;
  int           exp       = exp_bit - 127;
  int16_t       res;
  const int16_t kMax      = 32767;
  const int16_t kMin      = -32768;
  const size_t  kBitWidth = 8 * sizeof(int16_t);

  if (exp_bit == 0 && mantissa == 0) { // exactly zero
    res = 0;
  } else if (exp_bit == 0x000000FF && mantissa == 0) { // infinity
    res = kMax;
  } else if (exp_bit == 0 && mantissa == 0x007FFFFF) { // denormalised
    assert(0 && "Do not handle denormalised case.");

    if (sign == 0 && mantissa > kMax) { // > kMax
      res = kMax;
    } else if (sign == 1 && mantissa > (-kMin)) { // < kMin
      res = kMin;
    } else {
      res = (sign) ? -mantissa : mantissa;
    }
  } else if (exp_bit == 0x000000FF && mantissa == 0x007FFFFF) { // NaN
    assert(0 && "Cannot handle NaN.");
    res = 0;
  } else if (exp < -1) { // underflow
    res = 0;
  } else if (exp == -1) {
    // 0.5xxx and -0.5xxx. After rounding, get 1 and -1.
    res = (sign) ? -1 : 1;
  } else if (exp > (kBitWidth - 2)) { // overflow
    res = (sign) ? kMin : kMax;
  } else {
    int32_t v = (0x8000 | ((mantissa & 0x007FFF00) >> 8)) >> (kBitWidth - 2 - exp);

    if (v & 0x1) { // roundup
      v += 1;
    }
    v = v >> 1;
    if (sign)
      v = -v;
    if (v > kMax)
      res = kMax;
    else if (v < kMin)
      res = kMin;
    else
      res = v;
  }

  return res;
}

int NvDlaConstants::getONNXInitializerOffset(int k, int c, int h, int w, NvDlaDims dims)
{
  // ONNX order: (K, C, H, W)
  return (k * (dims.c * dims.h * dims.w) + c * (dims.h * dims.w) + h * (dims.w) + w);
}

int NvDlaConstants::getBlobOffsetForDirectWeight(int k, int c, int h, int w, NvDlaDims dims)
{
  // JUNE
  int element_per_cbuf_entry =
    WEIGHT_ATOM_CUBE_SIZE / ELEMENT_SIZE; // use new variable instead of ATOMIC_C to cover INT8 in nv_full
  // Find parameters
  // printf("MAC_ATOMIC_K=%d  WEIGHT_ATOM_CUBE_SIZE=%d   ELEMENT_SIZE=%d", MAC_ATOMIC_K, WEIGHT_ATOM_CUBE_SIZE,
  // ELEMENT_SIZE);
  int kgrpi = k / MAC_ATOMIC_K;
  int cgrpi = c / element_per_cbuf_entry;

  int cur_kgrp_elem = ((dims.n - kgrpi * MAC_ATOMIC_K) < MAC_ATOMIC_K) ? (dims.n - kgrpi * MAC_ATOMIC_K) : MAC_ATOMIC_K;
  int cur_cgrp_elem = ((dims.c - cgrpi * element_per_cbuf_entry) < element_per_cbuf_entry)
                        ? (dims.c - cgrpi * element_per_cbuf_entry)
                        : element_per_cbuf_entry;

  int ki = k % MAC_ATOMIC_K;
  int ci = c % element_per_cbuf_entry;

  int full_kgrp_elem = MAC_ATOMIC_K * dims.c * dims.h * dims.w;
  int full_cgrp_elem = cur_kgrp_elem * element_per_cbuf_entry * dims.h * dims.w;

  // element index
  int element_index = kgrpi * full_kgrp_elem + cgrpi * full_cgrp_elem +
                      cur_kgrp_elem * cur_cgrp_elem * (dims.w * h + w) + ki * cur_cgrp_elem + ci;

  int addr_offset = element_index;
  assert((addr_offset < (dims.n * dims.c * dims.h * dims.w)) &&
         "Wrong calculation of addr_offset for getBlobOffsetForDirectWeight.");

  return addr_offset;
}

int NvDlaConstants::getBlobOffsetForImageWeight(int k, int c, int h, int w, NvDlaDims dims)
{
  // In image mode, all weight cubes are pre-extended to dims.n x (dims.c*dims.w) x dims.h x 1
  // Then placement in memory is identical to DirectWeight
  // JUNE
  const int IMG_C_4 = 4; // Onlyl support 4 channels in image mode
  int       prext_k = k;
  int       prext_c = w * IMG_C_4 + c;
  int       prext_h = h;
  int       prext_w = 0;

  dims.c = dims.w * IMG_C_4;
  dims.w = 1;

  int addr_offset = getBlobOffsetForDirectWeight(prext_k, prext_c, prext_h, prext_w, dims);

  return addr_offset;
}

int NvDlaConstants::getBlobOffsetForSDPOperand(int c, int h, int w, const NvDlaCubeInfo& cubeInfo)
{
  assert((cubeInfo.dim_n == 1) && "Do not support batch.");
  assert(c < cubeInfo.dim_c);
  assert(h < cubeInfo.dim_h);
  assert(w < cubeInfo.dim_w);

  int byte_per_element;
  int op_num;

  switch (cubeInfo.mode) {
  case NVDLA_CUBE_SDP_X_ALU_OR_MUL_ONE_BYTE:
  case NVDLA_CUBE_SDP_Y_ALU_OR_MUL_ONE_BYTE:
    byte_per_element = 1;
    op_num           = 1;
    break;

  case NVDLA_CUBE_SDP_X_BOTH_ONE_BYTE:
  case NVDLA_CUBE_SDP_Y_BOTH_ONE_BYTE:
    byte_per_element = 1;
    op_num           = 2;
    break;

  case NVDLA_CUBE_SDP_X_ALU_OR_MUL_TWO_BYTE:
  case NVDLA_CUBE_SDP_Y_ALU_OR_MUL_TWO_BYTE:
    byte_per_element = 2;
    op_num           = 1;
    break;

  case NVDLA_CUBE_SDP_X_BOTH_TWO_BYTE:
  case NVDLA_CUBE_SDP_Y_BOTH_TWO_BYTE:
    byte_per_element = 2;
    op_num           = 2;
    break;

  default:
    assert(0);
    byte_per_element = 1;
    op_num           = 1;
    break;
  }

  int element_per_entry = (FEATURE_ATOM_CUBE_SIZE / ELEMENT_SIZE);
  int cgrpi             = c / element_per_entry;
  int ci                = c % element_per_entry;

  int addr_offset = cgrpi * (cubeInfo.stride_surface / (byte_per_element * op_num)) +
                    h * (cubeInfo.stride_line / (byte_per_element * op_num)) + w * element_per_entry + ci;

  assert((addr_offset < (cubeInfo.size / (byte_per_element * op_num))) &&
         "Wrong calculation of addr_offset in getBlobOffsetForSDPOperand.");

  return addr_offset;
}

NvDlaConstants getConfig(nvdla::ConfigSet configSet, nvdla::ExecutionMode mode, bool enableLayerFusion)
{
  switch (configSet) {
  case nvdla::ConfigSet::nv_full:
    return getConfig<nvdla::ConfigSet::nv_full>(mode, enableLayerFusion);
  }

  assert(false && "Meet unsupported config set");
  return NvDlaConstants();
}

} // namespace onnc
