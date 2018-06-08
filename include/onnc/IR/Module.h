//===- Module.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_MODULE_H
#define ONNC_IR_MODULE_H
#include <map>
#include <memory>
#include <onnc/ADT/StringMap.h>
#include <onnc/IR/SymbolTable.h>
#include <onnc/IR/ComputeGraph.h>
#include <onnc/IR/Dump.h>

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
public:
  typedef std::map<std::string, int64_t> OpsetImportType;
  typedef std::map<std::string, std::string> MetaDataMapType;

public:
  Module();

  ~Module();

  std::shared_ptr< ::onnx::Graph> getGraph() { return m_pOnnxGraph; }

  std::shared_ptr<const ::onnx::Graph> getGraph() const { return m_pOnnxGraph; }

  // move @ref pGraph from outside.
  Module &delegateGraph(std::unique_ptr< ::onnx::Graph> pGraph);

  MetaDataMapType &getMetaData() { return m_OnnxMetaData; }

  const MetaDataMapType &getMetaData() const { return m_OnnxMetaData; }

  OpsetImportType &getSetId() { return m_OnnxSetId; }

  const OpsetImportType &getSetId() const { return m_OnnxSetId; }

  void setIRVersion(int64_t pVersion) { m_OnnxIRVersion = pVersion; }

  int64_t getIRVersion() const { return m_OnnxIRVersion; }

  void setProducer(const std::string& pName, const std::string& pVersion);

  const std::string& getProducerName() const { return m_OnnxProducerName; }

  void setProducerName(const std::string& pName) { m_OnnxProducerName = pName; }

  const std::string& getProducerVersion() const { return m_OnnxProducerVersion; }

  void setProducerVersion(const std::string& pVersion) { m_OnnxProducerVersion = pVersion; }

  const std::string& getDomain() const { return m_OnnxDomain; }

  void setDomain(const std::string& pDomain) { m_OnnxDomain = pDomain; }

  int64_t getModelVersion() const { return m_OnnxModelVersion; }

  void setModelVersion(int64_t pModelVersion) { m_OnnxModelVersion = pModelVersion; }

  const std::string& getDocString() const { return m_OnnxDocString; }

  void setDocString(const std::string& pDocString) { m_OnnxDocString = pDocString; }

  ComputeGraph& getComputeIR() { return m_ComputeGraph; }

  const ComputeGraph& getComputeIR() const { return m_ComputeGraph; }

private:
  SymbolTable m_SymbolTable;

  // onnc keeps all ModelProto info
  int64_t m_OnnxIRVersion;
  std::string m_OnnxProducerName;
  std::string m_OnnxProducerVersion;
  std::string m_OnnxDomain;
  int64_t m_OnnxModelVersion;
  std::string m_OnnxDocString;

  std::shared_ptr< ::onnx::Graph> m_pOnnxGraph;
  OpsetImportType m_OnnxSetId;
  MetaDataMapType m_OnnxMetaData;

  // compute IR field
  ComputeGraph m_ComputeGraph;
};

} // namespace onnc

#endif
