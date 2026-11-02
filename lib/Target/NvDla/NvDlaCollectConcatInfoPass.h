//===- NvDlaCollectConcatInfoPass.h ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_NVDLA_NVDLA_COLLECT_CONCAT_INFO_PASS_H_INCLUDED
#define ONNC_TARGET_NVDLA_NVDLA_COLLECT_CONCAT_INFO_PASS_H_INCLUDED

#include "NvDlaDefine.h"
#include "NvDlaMeta.h"

#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Compute/Concat.h>

#include <string>

namespace onnc {

/** \class NvDlaCollectConcatInfoPass
 *  \brief Decide which Concat operators are done "by placement" and record where their inputs go.
 *
 *  A Concat by placement needs no hardware operation: each input gets a place inside the buffer of
 *  the Concat's output (NvDlaBackendMeta::markAsPlaced), so the layer that produces an input writes
 *  it straight to where the joined tensor needs it. This is the only way to build a Concat on
 *  nv_small, which has no RUBIK unit (the unit the other Concat implementation uses).
 *
 *  It is only correct when every input except the last has a whole number of channel groups
 *  (a multiple of FEATURE_ATOM_CUBE_SIZE / ELEMENT_SIZE channels: 8 on nv_small). A layer writes
 *  whole groups, so a partly filled last group of one input cannot be completed by the next one.
 *
 *  Default: placement on nv_small, RUBIK elsewhere. The environment variable ONNC_NVDLA_CONCAT
 *  (rubik | placement) overrides the default. A Concat that cannot be placed stops the compilation
 *  on nv_small and falls back to RUBIK elsewhere.
 */
class NvDlaCollectConcatInfoPass : public CustomPass<NvDlaCollectConcatInfoPass>, private NvDlaConstants
{
public:
  NvDlaCollectConcatInfoPass(const NvDlaConstants& constants, NvDlaBackendMeta& meta) noexcept;

  ReturnType runOnModule(Module& pModule) override;

private:
  /// Place the inputs of @ref concat if all rules hold. If not, return false and say why in @ref reason.
  bool tryPlace(const Concat& concat, std::string& reason);

private:
  NvDlaBackendMeta& m_Meta;
};

} // namespace onnc

#endif
