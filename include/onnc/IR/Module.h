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
#include <onnc/IR/ComputeGraph.h>
#include <onnc/IR/Compute/Define.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/StringMap.h>
#include <onnx/common/ir.h>
#include <vector>
#include <ostream>

namespace onnc {

/** \class Module
 *  \brief Rrepresentation of ONNX model
 */
class Module
{
public:
  typedef StringMap<ComputeGraph*> ComputeGraphList;
  typedef ComputeGraphList::iterator compute_iterator;
  typedef ComputeGraphList::const_iterator const_compute_iterator;

  typedef ComputeGraph::ArcList ComputeOperandList;
  typedef std::vector<onnc::Define*> ComputeDefineList;

  typedef StringMap<Value*> ValueList;

  typedef std::map<std::string, int64_t> OpsetImport;
  typedef std::map<std::string, std::string> MetaDataMap;

  class OnnxInfo
  {
  public:
    OnnxInfo();

    ~OnnxInfo() { }

    void setIRVersion(int64_t pVersion) { m_IRVersion = pVersion; }

    int64_t getIRVersion() const { return m_IRVersion; }

    void setProducer(const std::string& pName, const std::string& pVersion);

    const std::string& getProducerName() const { return m_ProducerName; }

    void setProducerName(const std::string& pName) { m_ProducerName = pName; }

    const std::string& getProducerVersion() const { return m_ProducerVersion; }

    void setProducerVersion(const std::string& pVersion) { m_ProducerVersion = pVersion; }

    const std::string& getDomain() const { return m_Domain; }

    void setDomain(const std::string& pDomain) { m_Domain = pDomain; }

    int64_t getModelVersion() const { return m_ModelVersion; }

    void setModelVersion(int64_t pModelVersion) { m_ModelVersion = pModelVersion; }

    const std::string& getDocString() const { return m_DocString; }

    void setDocString(const std::string& pDocString) { m_DocString = pDocString; }

    /// print the information in @ref pOS
    void print(std::ostream& pOS) const;

    /// print the information to stderrs.
    void dump() const;

  private:
    int64_t m_IRVersion;
    std::string m_ProducerName;
    std::string m_ProducerVersion;
    std::string m_Domain;
    int64_t m_ModelVersion;
    std::string m_DocString;
  };

  using GraphIR = ::onnx::Graph;

public:
  /// default constructor. No graph IR is set.
  Module();

  /// delegation constructor.
  Module(std::unique_ptr< ::onnx::Graph> pGraph);

  /// Destructor. Check and delete IRs.
  /// Module responses for the life cycle of the delegated ::onnx::Graph.
  ~Module();

  // move @ref pGraph from outside.
  Module& delegate(std::unique_ptr< ::onnx::Graph> pGraph);

  // move @ref pGraph from outside.
  Module& delegate(::onnx::Graph& pGraph);

  std::shared_ptr< ::onnx::Graph> getGraphIR() { return m_pOnnxGraph; }

  std::shared_ptr<const ::onnx::Graph> getGraphIR() const { return m_pOnnxGraph; }

  bool hasGraphIR() const { return (0 != m_pOnnxGraph.use_count()); }

  MetaDataMap &getMetaData() { return m_OnnxMetaData; }

  const MetaDataMap &getMetaData() const { return m_OnnxMetaData; }

  OpsetImport &getSetId() { return m_OnnxSetId; }

  const OpsetImport &getSetId() const { return m_OnnxSetId; }

  OnnxInfo& getOnnxInfo() { return m_OnnxInfo; }

  const OnnxInfo& getOnnxInfo() const { return m_OnnxInfo; }

  bool hasRootComputeGraph() const { return (nullptr != m_pRootComputeGraph); }

  ComputeGraph* getRootComputeGraph() { return m_pRootComputeGraph; }

  const ComputeGraph* getRootComputeGraph() const { return m_pRootComputeGraph; }

  /// get the graph named @ref pName
  /// @retval nullptr not found
  ComputeGraph* getComputeGraph(StringRef pName);

  /// get the graph named @ref pName
  /// @retval nullptr not found
  const ComputeGraph* getComputeGraph(StringRef pName) const;

  compute_iterator begin() { return m_ComputeGraphs.begin(); }

  compute_iterator end() { return m_ComputeGraphs.end(); }

  const_compute_iterator begin() const { return m_ComputeGraphs.begin(); }

  const_compute_iterator end() const { return m_ComputeGraphs.end(); }

  /// return the number of compute graphs
  unsigned getNumOfComputeGraphs() const { return m_ComputeGraphs.numOfEntries(); }

  ComputeOperandList& getComputeOperands() { return m_ComputeOperands; }

  const ComputeOperandList& getComputeOperands() const { return m_ComputeOperands; }

  ComputeDefineList& getComputeDefines() { return m_ComputeDefines; }

  const ComputeDefineList& getComputeDefines() const { return m_ComputeDefines; }

  /// create a compute graph and put it in module.
  /// @retval nullptr The graph already exists
  ComputeGraph* createComputeGraph(StringRef pName);

  /// Add a value which is created by ComputeGraph.
  /// Value is deleted by Module.
  void addValue(Value* pValue);

  ValueList& getValueList();

  const ValueList& getValueList() const;

  // print the whole module to @ref pOS.
  void print(std::ostream& pOS) const;

  template<typename PART>
  void print(std::ostream& pOS) const { assert(false && "no part to print!"); }

  template<typename PART>
  void print(std::ostream& pOS, const PART& pPart) const {
    assert(false && "no part to print!");
  }

  /// print the information to stderrs.
  void dump() const;

private:
  // Graph IR field
  std::shared_ptr< ::onnx::Graph> m_pOnnxGraph;
  OnnxInfo m_OnnxInfo;
  OpsetImport m_OnnxSetId;
  MetaDataMap m_OnnxMetaData;

  // compute IR field
  ComputeGraph* m_pRootComputeGraph;
  ComputeGraphList m_ComputeGraphs;
  ComputeOperandList m_ComputeOperands;
  ComputeDefineList m_ComputeDefines;
  ValueList m_Values;
};

template<> void Module::print<Module::OpsetImport>(std::ostream& pOS) const;

template<> void Module::print<Module::MetaDataMap>(std::ostream& pOS) const;

template<> void
Module::print(std::ostream& pOS, const ::onnx::Value& pValue) const;

template<> void
Module::print(std::ostream& pOS, const ::onnx::Node& pNode) const;

} // namespace onnc

#endif
