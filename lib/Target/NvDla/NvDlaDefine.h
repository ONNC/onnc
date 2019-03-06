//===- NvDlaDefine.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_DEFINE_H
#define TARGET_NVDLA_NVDLA_DEFINE_H

#include "dla_interface.h"

#include <onnc/Target/TargetOptions.h>

#include <type_traits>

namespace onnc {

class NvDlaConstants
{
public:
  using NvDlaConfigSet     = TargetOptions::NvDlaConfigSet;
  using NvDlaExecutionMode = TargetOptions::NvDlaExecutionMode;

  template <NvDlaConfigSet ConfigSet>
  using nv_weight_t = typename std::conditional<ConfigSet == NvDlaConfigSet::nv_small, char, short>::type;

public:
  NvDlaConstants(NvDlaConfigSet pNvdlaConfigSet, NvDlaExecutionMode pNvdlaExecutionMode,
                 bool pEnableLayerFusion) noexcept
    : DLA_NV_SMALL{pNvdlaConfigSet == NvDlaConfigSet::nv_small}
    , HAS_IMAGE_MODE{pNvdlaExecutionMode == NvDlaExecutionMode::image}
    , HAS_LAYER_FUSION{pEnableLayerFusion}
    , DLA_PRECISION{DLA_NV_SMALL ? PRECISION_INT8 : PRECISION_FP16}
    , FEATURE_ATOM_CUBE_SIZE{DLA_NV_SMALL ? 8 : 32}
    , WEIGHT_ATOM_CUBE_SIZE{DLA_NV_SMALL ? 8 : 128}
    , ELEMENT_SIZE{DLA_NV_SMALL ? 1 : 2}
    , MAC_ATOMIC_C{DLA_NV_SMALL ? 8 : 64}
    , MAC_ATOMIC_K{DLA_NV_SMALL ? 8 : 16}
    , CBUF_BANK_NUM{DLA_NV_SMALL ? 32 : 16}
    , CBUF_BANK_WIDTH{DLA_NV_SMALL ? 8 : 64}
    , CBUF_BANK_DEPTH{DLA_NV_SMALL ? 512 : 256}
  {}

protected:
  float __gnu_h2f_ieee(short param);
  short __gnu_f2h_ieee(float param);
  void  weight_pack(void* buf, const float* data, unsigned long long size, int G, int dims[4], int type,
                    bool shouldPadZero);

protected:
  const bool DLA_NV_SMALL;
  const bool HAS_IMAGE_MODE;
  const bool HAS_LAYER_FUSION;
  const int  DLA_PRECISION;
  const int  FEATURE_ATOM_CUBE_SIZE;
  const int  WEIGHT_ATOM_CUBE_SIZE;
  const int  ELEMENT_SIZE;
  const int  MAC_ATOMIC_C;
  const int  MAC_ATOMIC_K;
  const int  CBUF_BANK_NUM;
  const int  CBUF_BANK_WIDTH;
  const int  CBUF_BANK_DEPTH;
};

} // namespace onnc

#endif
