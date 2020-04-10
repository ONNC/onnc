//===- LegalizePadPass.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===------------------------------------------------------------------------------===//
#ifndef LEGALIZE_PAD_PASS_H
#define LEGALIZE_PAD_PASS_H

#include "NvDlaMeta.h"

#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Compute/Pad.h>

namespace onnc {
namespace nvdla {

class LegalizePadPass : public CustomPass<LegalizePadPass>
{
public:
  enum class OperatorWithPads {
    AveragePool,
    Conv,
    MaxPool,
    LpPool
  };
  
  using PadsType = IntsAttr::ValueType;
  using PadsLimitType = std::set<std::pair<OperatorWithPads, PadsType>>;

  class BackendSetting {
    public:
      friend class LegalizePadPass;
      
      BackendSetting() 
      : pads_limit() {}

      BackendSetting(const PadsLimitType& pads_limit) 
      : pads_limit(pads_limit) {}

    private:
      const PadsLimitType pads_limit;
  };

  LegalizePadPass(const PadsLimitType& setting) 
  : m_backend_setting(BackendSetting(setting)) {}

  ReturnType runOnModule(Module& pModule) override;
  Pass::ReturnType runOnComputeGraph(ComputeGraph& pCG) override;
  
private:
  void remove_pad(Pad* pad, ComputeGraph& pCG);

  template<typename OperatorType>
  void set_pads_impl(OperatorType* op, Pad* pad);

  template<typename OperatorType>
  bool is_valid_pads(OperatorType* op, Pad* pad, PadsType upper_bound);

  bool is_removable_pad(Pad* pad);

  bool is_followed_by_averagepool(Pad* pad);
  bool is_followed_by_conv(Pad* pad);
  bool is_followed_by_maxpool(Pad* pad);
  bool is_followed_by_lppool(Pad* pad);

  const BackendSetting m_backend_setting;

};

} // namespace nvdla
} // namespace onnc

#endif // MODELSIM_LEGALIZE_PAD_PASS_H
