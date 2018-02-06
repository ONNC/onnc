//===- TensorOperator.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_TENSOR_OPERATOR_H
#define ONNC_IR_TENSOR_OPERATOR_H
#include <onnc/ADT/Digraph.h>

namespace onnc {

/** \class TensorOperator
 */
class TensorOperator : public Digraph::Node
{
};

} // namespace of onnc

#endif
