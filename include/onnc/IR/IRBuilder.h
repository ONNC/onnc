//===- IRBuilder.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_IR_BUILDER_H
#define ONNC_IR_IR_BUILDER_H
#include <onnc/IR/Module.h>
#include <onnc/IR/InsertionPoint.h>
#include <onnc/IR/Tensor/InitializerProxy.h>
#include <onnc/Config/ONNX.h>
#include <onnc/ADT/StringMap.h>
#include <onnc/ADT/StringList.h>
#include <vector>

namespace onnc {
class Tensor;

/** \class IRBuilder
 *  \brief IRBullder encapsulates creating parts of a onnc::Module
 *
 *  IRBuilder provides atomic functions to create basic units of Compute IR,
 *  such as ComputeOperator, ComputeOperand and so on.
 */
class IRBuilder
{
public:
  /// set the target module @ref pModel
  IRBuilder(Module& pModule);

  /// update @ref pModule by @ref pProto
  /// This constructor calls update.
  IRBuilder(Module& pModule, const xProto& pProto);

  InsertionPoint& getInsertionPoint() { return m_InsertionPoint; }

  const InsertionPoint& getInsertionPoint() const { return m_InsertionPoint; }

  void setInsertionPoint(const InsertionPoint& pIP);

  /// Get the target module
  Module& getModule() { return *getInsertionPoint().getModule(); }

  /// Get the target module
  const Module& getModule() const { return *getInsertionPoint().getModule(); }

  /// update the inserted module by @ref pProto
  void update(const xProto& pProto);

  /// change the insertion point to @ref pCG
  void setComputeGraph(ComputeGraph* pCG);

  /// create a tensor graph
  xGraph* CreateTensorGraph();

  /// create a tensor graph whose name is @ref pName
  xGraph* CreateTensorGraph(StringRef pName);

  /// get current insertion point of tensor graph.
  /// @retval nullptr not set
  xGraph* getTensorGraph() { return getInsertionPoint().getTensorGraph(); }

  bool hasTensorGraph() const { return getInsertionPoint().hasTensorGraph(); }

  /// Add an input in tensor graph.
  /// @param[in] pSizes a list of onnx::Dimension
  /// @param[in] pKind  hide onnx::TensorProto_DataType
  xValue* AddInput(const std::string& pName,
                   const std::vector<xDimension>& pSizes,
                   onnc::Value::Type pKind = onnc::Value::kFloat);

  /// Add a node in tensor graph
  /// @param[in] pInputs a list of input names for this node
  xNode* AddNode(const std::string& pName, const StringList& pInputNames);

  xNode*
  AddTensorOp(const std::string& pName, const StringList& pInputNames) {
    return AddNode(pName, pInputNames);
  }

  /// Return a copy of the specified node, but without embedding it into the
  /// graph.
  /// - kind,
  /// - graph,
  /// - doc_strings, and
  /// - attributes.
  /// This function doesn't copy:
  /// - stage,
  /// - inputs, and
  /// - outputs.
  /// This function doesn't insert the node into the graph. Use Graph::appendNode,
  /// Graph::prependNode functions to insert the node.
  /// This function also set target node to the cloned node.
  /// @param[in] pName The name of the new node
  xNode* CloneNode(xNode& pNode, const std::string& pName = std::string());

  /// Add an initializer. If @ref pSizes is empty, find dimensions in inputs.
  /// @return The appended Initializer. If it fails, the function return an
  /// invalid Initializer
  onnc::InitializerProxy
  AddInitializer(const std::string& pName,
                 const std::vector<xDimension>& pSizes = { },
                 onnc::Value::Type pKind = onnc::Value::kFloat);

  xNode* getTensorNode() { return getInsertionPoint().getTensorNode(); }

  bool hasTensorNode() const { return getInsertionPoint().hasTensorNode(); }

  /// Add an output in the target node.
  xValue* AddOutput(const std::string& pName,
                    const std::vector<xDimension>& pSizes,
                    onnc::Value::Type pKind = onnc::Value::kFloat);

  /// Finalize tensor graph
  bool FinalizeTensorGraph(const StringList& pOutputList);

  /// get current insertion point of compute graph
  /// @retval nullptr not set.
  ComputeGraph* getComputeGraph() { return getInsertionPoint().getComputeGraph(); }

  bool hasComputeGraph() const { return getInsertionPoint().hasComputeGraph(); }

  /// create a compute graph
  /// @retval nullptr The graph already exists in module.
  ComputeGraph* CreateComputeGraph(StringRef pName) {
    ComputeGraph* cg = getModule().createComputeGraph(pName);
    setComputeGraph(cg);
    return cg;
  }

  template<typename OpType>
  IRBuilder& RegisterComputeOperator(OpType* pOperator) {
    // target compute graph shall add pOperator automatically in operator
    // created.
    getModule().getComputeDefines().push_back(pOperator);
    return *this;
  }

  /// Create and add an compute operator in the compute graph.
  template<typename OpType, typename ... CtorParams>
  OpType* AddComputeOp(CtorParams&& ... pParams);

  /// Create and add an compute operator in the compute graph.
  template<typename OpType, typename ... CtorParams>
  OpType* AddComputeOp(const xNode& pNode, CtorParams&& ... pParams);

  /// Create and add an comute operand in the compute graph.
  /// Two operators are connected as well.
  template<typename OpndType, typename OpType, typename ... CtorParams>
  OpndType* AddComputeOpnd(OpType& pFrom, OpType& pTo, CtorParams&& ... pParams);

  /// Return a copy of a specified operator, but without embedding it into the
  /// compute graph.
  /// TODO: use unique_ptr.
  template<typename OpType>
  OpType* CloneComputeOp(const OpType& pOp);

  /// Create tensor from onnx value. The method is used when creating
  /// compute tensor for all of onnx graph inputs.
  /// @param pTensor The tensor from an initializer.
  Tensor* CreateComputeTensor(const xValue& pValue,
                              const xTensor& pTensor);

  /// Create tensor from onnx value. The method is used when creating
  /// compute tensor for all of onnx graph inputs.
  static Tensor* CreateComputeTensor(ComputeGraph& pCG, const xValue& pValue);

  static Tensor* CreateComputeTensor(ComputeGraph& pCG,
                                     const xValue& pValue,
                                     const xTensor& pTensor);

  /// Create tensor from onnx value. The method is used when creating
  /// compute tensor for all of onnx graph inputs.
  Tensor* CreateComputeTensor(const xValue& pValue);

private:
  InsertionPoint m_InsertionPoint;
};

#include "Bits/IRBuilder.tcc"

} // namespace onnc

#endif
