//===- TensorPass.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_TENSOR_PASS_H
#define ONNC_CORE_TENSOR_PASS_H
#include <onnc/Core/Pass.h>

namespace onnc {

/** \class onnc::TensorPass
 *  \brief encapsulate transformation algorithms.
 */
class TensorPass : public Pass
{
public:
  explicit TensorPass(char& pPassID)
    : Pass(kPT_Tensor, pPassID) { }

  ~TensorPass() override;

  /// Virtual method overriden by subclasses to process the module
  /// being operated on.
  virtual Pass::ReturnType runOnTensor(Tensor &pTensor) = 0;
};

} // namespace of onnc

#endif
