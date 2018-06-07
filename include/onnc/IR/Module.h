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

private:
  // XXX: use accessors to explore private data member.
  friend void onnx::ExportModelProto(::onnx::ModelProto &pModelProto,
                                     const Module &pModule);

  // XXX: use accessors to explore private data member.
  friend void onnx::ImportModelProto(Module &pModule,
                                     const ::onnx::ModelProto &pModelProto);

  // XXX: use accessors to explore private data member.
  friend void onnc::DumpModule(Module &pModule);

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
};

} // namespace onnc

#endif
