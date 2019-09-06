//===- NvFull.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaDefine.h"
#include "include/nvdla/IRuntime.h"

namespace onnc {

template <>
NvDlaConstants getConfig<nvdla::ConfigSet::nv_full>(nvdla::ExecutionMode mode, bool enableLayerFusion)
{
  NvDlaConstants config;

  config.CONFIG_SET              = nvdla::ConfigSet::nv_full;
  config.EXECUTION_MODE          = mode;
  config.IS_LAYER_FUSION_ENABLED = enableLayerFusion;
  config.DLA_PRECISION           = PRECISION_FP16;
  config.FEATURE_ATOM_CUBE_SIZE  = 32;
  config.WEIGHT_ATOM_CUBE_SIZE   = 128;
  config.ELEMENT_SIZE            = 2;
  config.MAC_ATOMIC_C            = 64;
  config.MAC_ATOMIC_K            = 16;
  config.CBUF_BANK_NUM           = 16;
  config.CBUF_BANK_WIDTH         = 128;
  config.CBUF_BANK_DEPTH         = 256;
  config.MAX_MEM_TRANSACTION_NUM = 8;
  config.DATA_TYPE               = ::nvdla::loadable::DataType::DataType_HALF;
  config.INPUT_PIXEL_FORMAT  = TENSOR_PIXEL_FORMAT_FEATURE;
  config.OUTPUT_PIXEL_FORMAT = TENSOR_PIXEL_FORMAT_FEATURE;

  return config;
}

} // namespace onnc
