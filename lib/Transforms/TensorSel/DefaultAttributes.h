//===- DefaultAttributes.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSORSEL_DEFAULTATTRIBUTES_H
#define ONNC_TRANSFORMS_TENSORSEL_DEFAULTATTRIBUTES_H
#include <onnc/Config/ONNX.h>

namespace onnc {

class ComputeOperator;

void SetDefaultAttributes(xNode &pNode, ComputeOperator& pOp);

} // namespace of onnc

#endif
