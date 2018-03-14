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
#include <memory>

namespace onnx {
  class Graph;
} // namespace of onnx

namespace onnc {

/** \class Module
 *  \brief Rrepresentation of ONNX model
 */
class Module
{
public:
  Module();
  ~Module();
  onnx::Graph *getGraph() { return m_pOnnxGraph; }

  const onnx::Graph *getGraph() const { return m_pOnnxGraph; }

  // move @ref pGraph from outside.
  Module& delegateGraph(std::unique_ptr<onnx::Graph> pGraph);

private:
  SymbolTable m_SymbolTable;
  onnx::Graph* m_pOnnxGraph;
};

} // namespace of onnc

#endif
