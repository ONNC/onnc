//===- ComputeOperand.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERAND_H
#define ONNC_IR_COMPUTE_OPERAND_H
#include <ostream>
#include <onnx/common/ir.h>
#include <onnc/ADT/Bits/DigraphArc.h>

namespace onnc {

class ComputeOperator;

/** \class ComputeOperand
 * 
 */
class ComputeOperand : public DigraphArc<ComputeOperator, ComputeOperand>
{
};


} // namespace of onnc

#endif
