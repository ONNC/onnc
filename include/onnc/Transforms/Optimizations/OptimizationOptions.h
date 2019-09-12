//===- OptimizationOptions.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_OPTIMIZATION_OPTIONS_H_INCLUDED
#define ONNC_TRANSFORMS_OPTIMIZATION_OPTIONS_H_INCLUDED

#include <onnc/Core/PassManager.h>
#include <onnc/Support/Enum.h>
#include <onnc/Support/TypeTraits.h>
#include <onnc/Transforms/Optimizations/DivideGlobalAPIntoAPs.h>
#include <onnc/Transforms/Optimizations/EliminateIdentity.h>
#include <onnc/Transforms/Optimizations/ExpandBatchNormalization.h>
#include <onnc/Transforms/Optimizations/PropagateConstWithDiffShape.h>
#include <onnc/Transforms/Optimizations/ReplaceGemmByConv.h>
#include <onnc/Transforms/Optimizations/SplitConvPass.h>

#include <iterator>
#include <map>
#include <type_traits>
#include <utility>

namespace onnc {

enum class OptimizationOption : unsigned
{
  divide_globalap_into_aps,
  eliminate_identity,
  propagate_const_with_diff_shape,
  expand_batch_normalization,
  replace_gemm_by_conv,
  split_conv_by_channel,
};

class OptimizationOptions
{
public:
  void disable(OptimizationOption option) { options[option] = false; }

  void enable(OptimizationOption option) { options[option] = true; }

  bool isEnabled(OptimizationOption option, bool defaultValue = false) const
  {
    using std::end;

    const auto found = options.find(option);
    if (found == end(options)) {
      return defaultValue;
    }

    return found->second;
  }

  bool has(OptimizationOption option) const
  {
    using std::end;

    return options.find(option) != end(options);
  }

  void defaultEnable(OptimizationOption option)
  {
    if (!has(option)) {
      enable(option);
    }
  }

  static void add(PassManager& passManager, const OptimizationOptions& options)
  {
    for (const auto& item : options.options) {
      const bool isEnabled = item.second;
      if (isEnabled) {
        const OptimizationOption option = item.first;
        add(passManager, option);
      }
    }
  }

private:
  static void add(PassManager& passManager, OptimizationOption option)
  {
    switch (option) {
    case OptimizationOption::divide_globalap_into_aps:
      passManager.add<DivideGlobalAPIntoAPs>();
      break;
    case OptimizationOption::eliminate_identity:
      passManager.add<EliminateIdentity>();
      break;
    case OptimizationOption::propagate_const_with_diff_shape:
      passManager.add<PropagateConstWithDiffShape>();
      break;
    case OptimizationOption::expand_batch_normalization:
      passManager.add<ExpandBatchNormalization>();
      break;
    case OptimizationOption::replace_gemm_by_conv:
      passManager.add<ReplaceGemmByConv>();
      break;
    default:
      // skip other values
      break;
    }
  }

private:
  std::map<OptimizationOption, bool, enum_less<OptimizationOption>> options;
};

} // namespace onnc

#endif
