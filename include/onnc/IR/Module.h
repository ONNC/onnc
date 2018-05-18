//===- Module.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_MODULE_H
#define ONNC_IR_MODULE_H
#include <memory>
#include <onnc/ADT/StringMap.h>
#include <onnc/IR/SymbolTable.h>

namespace onnx {
class Graph;
} // namespace onnx

namespace onnc {

/** \class Module
 *  \brief Rrepresentation of ONNX model
 */
class Module
{
  typedef StringMap<std::string> metaDataMap_t;

public:
  Module();
  ~Module();
  ::onnx::Graph *getGraph() { return m_OnnxGraph.get(); }

  const ::onnx::Graph *getGraph() const { return m_OnnxGraph.get(); }

  // for demo
  const std::shared_ptr< ::onnx::Graph> &getGraphSP() { return m_OnnxGraph; }

  // move @ref pGraph from outside.
  Module &delegateGraph(std::unique_ptr< ::onnx::Graph> pGraph);

  metaDataMap_t::const_iterator metaData_cbegin() const
  {
    return m_OnnxMetadata.begin();
  }
  metaDataMap_t::const_iterator metaData_cend() const
  {
    return m_OnnxMetadata.end();
  }

  const std::string &lookupMetaData(const std::string &pKey) const
  {
    auto it = m_OnnxMetadata.find(pKey);
    assert(it != m_OnnxMetadata.end());
    return it->value();
  }

  void insertMetaData(const std::string &pKey, const std::string &pValue)
  {
    bool exist;
    m_OnnxMetadata.insert(pKey, exist)->value() = pValue;
  }

private:
  SymbolTable m_SymbolTable;
  std::shared_ptr< ::onnx::Graph> m_OnnxGraph;
  metaDataMap_t m_OnnxMetadata;
};

} // namespace onnc

#endif
