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
#include <onnc/IR/Graph/Initializer.h>
#include <onnx/common/ir_pb_converter.h>
#include <onnx/common/ir.h>
#include <onnc/ADT/StringMap.h>
#include <onnc/ADT/StringList.h>
#include <vector>

namespace onnc {

/** \class IRBuilder
 *  \brief IRBullder encapsulates creating parts of a onnc::Module
 *
 *  IRBuilder provides atomic functions to create basic units of Compute IR,
 *  such as ComputeOperator, ComputeOperand and so on.
 */
class IRBuilder
{
private:
  using CreateValues = StringMap<::onnx::Value*>;

private:
  Module& m_Module; //< The target module.

  /// current target tensor graph
  ::onnx::Graph* m_pTargetTG;

  /// current target tensor graph node
  ::onnx::Node* m_pTargetTNode;

  /// current target compute graph
  ComputeGraph* m_pTargetCG;

  /// all ::onnx::Value created
  CreateValues m_CreatedValues;

public:
  /// set the target module @ref pModel
  IRBuilder(Module& pModule);

  /// update @ref pModule by @ref pProto
  /// This constructor calls update.
  IRBuilder(Module& pModule, const ::onnx::ModelProto& pProto);

  /// Get the target module
  Module& getModule() { return m_Module; }

  /// Get the target module
  const Module& getModule() const { return m_Module; }

  /// update the inserted module by @ref pProto
  void update(const ::onnx::ModelProto& pProto);

  /// change the insertion point to @ref pCG
  void setComputeGraph(ComputeGraph* pCG) { m_pTargetCG = pCG; }

  /// get current insertion point of tensor graph.
  /// @retval nullptr not set
  ::onnx::Graph* getTensorGraph() { return m_pTargetTG; }

  bool hasTensorGraph() const { return (nullptr != m_pTargetTG); }

  /// Add an input in tensor graph.
  /// @param[in] pSizes a list of onnx::Dimension
  /// @param[in] pKind  hide onnx::TensorProto_DataType
  ::onnx::Value* AddInput(const std::string& pName,
                          const std::vector<::onnx::Dimension>& pSizes,
                          onnc::Value::Type pKind);

  /// Add an input in tensor graph.
  /// @param[in] pSizes a list of integer in NCHW format.
  /// @param[in] pKind  hide onnx::TensorProto_DataType
  ::onnx::Value* AddInput(const std::string& pName,
                          const std::vector<int>& pSizes,
                          onnc::Value::Type pKind);

  /// Add a node in tensor graph
  /// @param[in] pInputs a list of input names for this node
  ::onnx::Node* AddNode(const std::string& pName, const StringList& pInputNames);

  ::onnx::Node*
  AddTensorOp(const std::string& pName, const StringList& pInputNames) {
    return AddNode(pName, pInputNames);
  }

  /// Add an initializer
  /// @return The appended Initializer. If it fails, the function return an
  /// invalid Initializer
  onnc::Initializer
  AddInitializer(const std::string& pName, onnc::Value::Type pKind);

  ::onnx::Node* getTensorNode() { return m_pTargetTNode; }

  bool hasTensorNode() const { return (nullptr != m_pTargetTNode); }

  /// Add an output in the target node.
  ::onnx::Value* AddOutput(const std::string& pName,
                           const std::vector<::onnx::Dimension>& pSizes,
                           onnc::Value::Type pKind);

  /// Add an output in the target node.
  ::onnx::Value* AddOutput(const std::string& pName,
                           const std::vector<int>& pSizes,
                           onnc::Value::Type pKind);

  /// Finalize tensor graph
  bool FinalizeTensorGraph(const StringList& pOutputList);

  /// get current insertion point of compute graph
  /// @retval nullptr not set.
  ComputeGraph* getComputeGraph() { return m_pTargetCG; }

  bool hasComputeGraph() const { return (nullptr != m_pTargetCG); }

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

  template<typename OpType, typename ... CtorParams>
  OpType* AddComputeOp(CtorParams&& ... pParams) {
    if (!hasComputeGraph())
      return nullptr;

    OpType* op = getComputeGraph()->addOperator<OpType>(pParams...);
    RegisterComputeOperator(op);
    return op;
  }
};

} // namespace onnc

#endif
