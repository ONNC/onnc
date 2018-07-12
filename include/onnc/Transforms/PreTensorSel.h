//===- PreTensorSel.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_PRE_TENSOR_SELECTION_H
#define ONNC_TRANSFORM_PRE_TENSOR_SELECTION_H
#include <onnc/Core/ModulePass.h>

namespace onnc {

/** \class PreTensorSel
 *  \brief PreTensorSel creates ComputeGraph objects and converts ONNX's
 *  initializers to ComputeOperators
 */
class PreTensorSel : public ModulePass
{
public:
  static char ID;

public:
  PreTensorSel() : ModulePass(ID) { }

  ~PreTensorSel() { }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;
};

ModulePass *CreatePreTensorSel();

} // namespace of onnc

#endif
