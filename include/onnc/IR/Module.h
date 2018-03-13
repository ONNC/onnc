//===- Module.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_MODULE_H
#define ONNC_IR_MODULE_H
#include <onnc/IR/SymbolTable.h>
#include <onnx/common/ir.h>

namespace onnc {

/** \class Module
 *  \brief Rrepresentation of ONNX model
 */
class Module
{
public:
  onnx::Graph &getGraph() { return m_onnxGraph; }

private:
  SymbolTable m_SymbolTable;
  onnx::Graph m_onnxGraph;
};

} // namespace of onnc

#endif
