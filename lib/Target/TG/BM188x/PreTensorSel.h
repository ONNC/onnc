//===- PreTensorSel.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_TG_BM188X_PRE_TENSOR_SELECTION_H
#define TARGET_TG_BM188X_PRE_TENSOR_SELECTION_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include "BM188xBackend.h"
#include <onnx/common/ir.h>

namespace onnc {
namespace bm188x {

/** \class PreTensorSel
 *  \brief PreTensorSel creates ComputeGraph objects and converts ONNX's
 *  initializers to ComputeOperators
 */
class PreTensorSel : public ModulePass
{
public:
  static char ID;

public:
  PreTensorSel(BM1880Backend *pBackend);

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

private:
  TGBackend *m_pTarget; // NOLINT
};

} // namespace of bm188x
} // namespace of onnc

#endif
