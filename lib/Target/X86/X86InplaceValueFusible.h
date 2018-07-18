//===- X86InplaceValueFusion.h --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_X86_X86_INPLACE_VALUE_FUSIBLE_H_H
#define TARGET_X86_X86_INPLACE_VALUE_FUSIBLE_H_H
#include <onnc/IR/ComputeOperator.h>

namespace onnc {
namespace x86 {

bool IsInplaceValueFusible(const ComputeOperator& pOp);

} // namespace x86
} // namespace onnc

#endif
