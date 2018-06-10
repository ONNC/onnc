#ifndef ONNC_IR_IR_BUILDER_H
#define ONNC_IR_IR_BUILDER_H
//===- IRBuilder.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Module.h>
#include <onnx/common/ir_pb_converter.h>

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
  Module& m_Module; //< The target module.

  /// current target compute graph
  ComputeGraph* m_pTargetCG;

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

  void setTargetComputeGraph(ComputeGraph* pCG) { m_pTargetCG = pCG; }

  /// create a compute graph
  /// @retval nullptr The graph already exists in module.
  ComputeGraph* CreateComputeGraph(StringRef pName) {
    ComputeGraph* cg = getModule().createComputeGraph(pName);
    setTargetComputeGraph(cg);
    return cg;
  }
};

} // namespace onnc

#endif
