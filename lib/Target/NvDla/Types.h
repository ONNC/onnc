//===- Types.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_TYPES_H_INCLUDED
#define TARGET_NVDLA_TYPES_H_INCLUDED

#include <onnc/Target/TargetOptions.h>

#include <cstdint>

#ifndef DEFINE_WEIGHT_TYPE
#  define DEFINE_WEIGHT_TYPE(config_set, weight_type) \
    template <>                                       \
    struct get_weight_type<config_set>                \
    {                                                 \
      using type = weight_type;                       \
    }
#else
#  error "macro DEFINE_WEIGHT_TYPE has been define in other file"
#endif

namespace onnc {

template <nvdla::ConfigSet CS>
struct get_weight_type;

template <nvdla::ConfigSet CS>
using nv_weight_t = typename get_weight_type<CS>::type;

DEFINE_WEIGHT_TYPE(nvdla::ConfigSet::nv_full, std::uint16_t);

} // namespace onnc

#undef DEFINE_WEIGHT_TYPE

#endif
