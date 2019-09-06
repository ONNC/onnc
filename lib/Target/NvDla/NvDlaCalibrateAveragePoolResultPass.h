//===- NvDlaCalibrateAveragePoolResultPass.h ------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_NVDLA_NVDLA_CALIBRATE_AVERAGE_POOL_RESULT_PASS_H_INCLUDED
#define ONNC_TARGET_NVDLA_NVDLA_CALIBRATE_AVERAGE_POOL_RESULT_PASS_H_INCLUDED

#include <onnc/Core/CustomPass.h>

namespace onnc {

class NvDlaCalibrateAveragePoolResultPass : public CustomPass<NvDlaCalibrateAveragePoolResultPass>
{
public:
  NvDlaCalibrateAveragePoolResultPass()                                           = default;
  NvDlaCalibrateAveragePoolResultPass(const NvDlaCalibrateAveragePoolResultPass&) = default;
  NvDlaCalibrateAveragePoolResultPass(NvDlaCalibrateAveragePoolResultPass&&)      = default;

  ~NvDlaCalibrateAveragePoolResultPass() = default;

  NvDlaCalibrateAveragePoolResultPass& operator=(const NvDlaCalibrateAveragePoolResultPass&) = default;
  NvDlaCalibrateAveragePoolResultPass& operator=(NvDlaCalibrateAveragePoolResultPass&&) = default;

  ReturnType runOnModule(Module& module) override;

private:
  ReturnType runOnComputeGraph(ComputeGraph& graph);
};

} // namespace onnc

#endif
