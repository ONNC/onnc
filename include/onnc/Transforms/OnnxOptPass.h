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

namespace onnc {

// XXX: Experimental

/** \class OnnxOptPass
 *  \brief [ONNX IR] Call ONNX optimizer
 */
class OnnxOptPass : public CustomPass<OnnxOptPass>
{
public:
  OnnxOptPass() = default;

  ReturnType runOnModule(Module& pModule) override;
};

} // namespace of onnc

#endif
