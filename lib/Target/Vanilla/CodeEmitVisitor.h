//===- CodeEmitVisitor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_VANILLA_CODE_EMIT_VISITOR_H
#define TARGET_VANILLA_CODE_EMIT_VISITOR_H
#include <onnc/IR/CustomVisitor.h>

namespace onnc {

namespace vanilla {

class CodeEmitVisitor : public CustomVisitor<CodeEmitVisitor>
{
public:
  /// ONNC defined operators @{
  void visit(const Initializer& pInitializer) override;
  void visit(const InputOperator& pInputOperator) override;
  void visit(const OutputOperator& pOutputOperator) override;
  /// @}

  /// ONNX defined operators @{
  void visit(const Conv& pConv) override;
  /// @}

  /// ONNC defined operators @{
  void visit(Initializer& pInitializer) override;
  void visit(InputOperator& pInputOperator) override;
  void visit(OutputOperator& pOutputOperator) override;
  /// @}

  /// ONNX defined operators @{
  void visit(Conv& pConv) override;
  /// @}
  
};
  
} // namespace vanilla
} // namespace onnc

#endif
