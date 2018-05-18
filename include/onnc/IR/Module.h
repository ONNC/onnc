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
class ModelProto;
} // namespace onnx

namespace onnc {
class Module;
namespace onnx {
void ExportModelProto(::onnx::ModelProto &pModelProto,
                      const ::onnc::Module &pModule);
void ImportModelProto(::onnc::Module &pModule,
                      const ::onnx::ModelProto &pModelProto);
} // namespace onnx
/** \class Module
 *  \brief Rrepresentation of ONNX model
 */
class Module
{
  typedef StringMap<int64_t> opsetImport_t;
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
  friend void onnx::ExportModelProto(::onnx::ModelProto &pModelProto,
                                     const Module &pModule);
  friend void onnx::ImportModelProto(Module &pModule,
                                     const ::onnx::ModelProto &pModelProto);
  SymbolTable m_SymbolTable;
  // onnc keeps all ModelProto info
  int64_t m_OnnxIRVersion;
  std::string m_OnnxProducerName;
  std::string m_OnnxProducerVersion;
  std::string m_OnnxDomain;
  int64_t m_OnnxModelVersion;
  std::string m_OnnxDocString;
  std::shared_ptr< ::onnx::Graph> m_OnnxGraph;
  opsetImport_t m_OnnxSetId;
  metaDataMap_t m_OnnxMetadata;
};

} // namespace onnc

#endif
