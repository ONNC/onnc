//===- DefaultAttributes.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSORSEL_DEFAULTATTRIBUTES_H
#define ONNC_TRANSFORMS_TENSORSEL_DEFAULTATTRIBUTES_H

namespace onnx {
class Node;
} // namespace of onnx

namespace onnc {

class ComputeOperator;

void SetDefaultAttributes(::onnx::Node &pNode, ComputeOperator& pOp);

} // namespace of onnc

#endif
