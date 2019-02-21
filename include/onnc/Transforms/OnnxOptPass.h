//===- OnnxOptPass.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_ONNX_OPT_PASS_H
#define ONNC_TRANSFORM_ONNX_OPT_PASS_H
#include <onnc/Core/CustomPass.h>

#include <vector>

namespace onnc {

/** \class OnnxOptPass
 *  \brief [ONNX IR] Call ONNX optimizer
 */
class OnnxOptPass : public CustomPass<OnnxOptPass>
{
public:
  enum Option : unsigned
  {
    extract_constant_to_initializer,
    fuse_add_bias_into_conv,
    fuse_bn_into_conv,
    fuse_consecutive_squeezes,
    fuse_consecutive_transposes,
    fuse_transpose_into_gemm,
    eliminate_identity,
    eliminate_nop_pad,
    eliminate_nop_transpose,
    eliminate_unused_initializer,
  };

public:
  OnnxOptPass() = default;
  OnnxOptPass(const OnnxOptPass&) = default;
  OnnxOptPass(OnnxOptPass&&) = default;

  ReturnType runOnModule(Module& pModule) override;

  OnnxOptPass&  add(Option option) &;
  OnnxOptPass&& add(Option option) &&;

private:
  std::set<std::string> names;
};

} // namespace of onnc

#endif
