//===- NvDlaDefine.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_DEFINE_H
#define TARGET_NVDLA_NVDLA_DEFINE_H

#include "Types.h"
#include "dla_interface.h"
#include "priv/loadable_generated.h"

#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Target/TargetOptions.h>

#include <cassert>
#include <cstdint>
#include <tuple>
#include <type_traits>

namespace onnc {

struct NvDlaDims
{
  using value_type = Tensor::Dimension;

  template <typename T, typename = typename std::enable_if<std::is_convertible<T, value_type>::value>::type>
  explicit NvDlaDims(T (&dimensions)[4])
    : n(dimensions[0])
    , c(dimensions[1])
    , h(dimensions[2])
    , w(dimensions[3])
  {}

  NvDlaDims(value_type n, value_type c, value_type h, value_type w)
    : n(n)
    , c(c)
    , h(h)
    , w(w)
  {}

  explicit NvDlaDims(const Tensor& tensor)
    : NvDlaDims(1, 1, 1, 1)
  {
    assert(tensor.getNumOfDimensions() <= 4);

    switch (tensor.getNumOfDimensions()) {
    case 4:
      w = tensor.dimension(3);
      h = tensor.dimension(2);
      c = tensor.dimension(1);
      n = tensor.dimension(0);
      break;
    case 3:
      w = tensor.dimension(2);
      h = tensor.dimension(1);
      c = tensor.dimension(0);
      break;
    case 2:
      w = tensor.dimension(2);
      h = tensor.dimension(1);
      break;
    case 1:
      c = tensor.dimension(0);
      break;
    default:
      assert(false && "meet unexpected # of dimensions");
      break;
    }
  }

  value_type size() const noexcept { return n * c * h * w; }

  value_type n;
  value_type c;
  value_type h;
  value_type w;
};

bool operator==(const NvDlaDims& lhs, const NvDlaDims& rhs);
bool operator!=(const NvDlaDims& lhs, const NvDlaDims& rhs);

struct NvDlaPoint
{
  using value_type = Tensor::Dimension;

  NvDlaPoint(value_type x, value_type y)
    : x(x)
    , y(y)
  {}

  const value_type x;
  const value_type y;
};

struct NvDlaRect
{
  using value_type = Tensor::Dimension;

  NvDlaRect(value_type h, value_type w)
    : h(h)
    , w(w)
  {}

  value_type size() const noexcept { return h * w; }

  const value_type h;
  const value_type w;
};

std::uint16_t f2float16_ieee(float param);
std::int8_t   f2int8_ieee(float param);
std::int16_t  f2int16_ieee(float param);

class NvDlaCubeInfo;

class NvDlaConstants
{
public:
  template <nvdla::ConfigSet CS>
  friend NvDlaConstants getConfig(nvdla::ExecutionMode mode, bool enableLayerFusion);

public:
  NvDlaConstants()                      = default;
  NvDlaConstants(const NvDlaConstants&) = default;
  NvDlaConstants(NvDlaConstants&&)      = default;

  NvDlaConstants& operator=(const NvDlaConstants&) = default;
  NvDlaConstants& operator=(NvDlaConstants&&) = default;

  int getONNXInitializerOffset(int k, int c, int h, int w, NvDlaDims dims);
  int getBlobOffsetForDirectWeight(int k, int c, int h, int w, NvDlaDims dims);
  int getBlobOffsetForImageWeight(int k, int c, int h, int w, NvDlaDims dims);
  int getBlobOffsetForSDPOperand(int c, int h, int w, const NvDlaCubeInfo& cubeInfo);

protected:
  nvdla::ConfigSet            CONFIG_SET;
  nvdla::ExecutionMode        EXECUTION_MODE;
  bool                        IS_LAYER_FUSION_ENABLED;
  unsigned                    DLA_PRECISION;
  unsigned                    FEATURE_ATOM_CUBE_SIZE;
  unsigned                    WEIGHT_ATOM_CUBE_SIZE;
  unsigned                    ELEMENT_SIZE;
  unsigned                    MAC_ATOMIC_C;
  unsigned                    MAC_ATOMIC_K;
  unsigned                    CBUF_BANK_NUM;
  unsigned                    CBUF_BANK_WIDTH;
  unsigned                    CBUF_BANK_DEPTH;
  unsigned                    MAX_MEM_TRANSACTION_NUM;
  ::nvdla::loadable::DataType DATA_TYPE;
  std::uint8_t                INPUT_PIXEL_FORMAT;
  std::uint8_t                OUTPUT_PIXEL_FORMAT;
};

template <nvdla::ConfigSet CS>
NvDlaConstants getConfig(nvdla::ExecutionMode mode, bool enableLayerFusion);

NvDlaConstants getConfig(nvdla::ConfigSet configSet, nvdla::ExecutionMode mode, bool enableLayerFusion);

} // namespace onnc

#endif
