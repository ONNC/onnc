//===- CodeEmitVisitor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_CORTEXM_CODE_EMIT_VISITOR_H
#define TARGET_CORTEXM_CODE_EMIT_VISITOR_H
#include "CortexmMeta.h"
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/CustomVisitor.h>

#define ELEMENT_SIZE 1

namespace onnc {

namespace cortexm {

class CodeEmitVisitor : public CustomVisitor<CodeEmitVisitor> {

public:
  explicit CodeEmitVisitor(CortexmBackendMeta& Meta) noexcept;

  // my extra
  struct weight_list* save_weight = (weight_list*)malloc(sizeof(weight_list));
  struct code_list* save_code = (code_list*)malloc(sizeof(code_list));
  struct add_list* save_add = (add_list*)malloc(sizeof(add_list));
  struct matmul_list* save_matmul = (matmul_list*)malloc(sizeof(matmul_list));
  struct shape_list* save_shape = (shape_list*)malloc(sizeof(shape_list));
  int buffer_order = 0;
  int first = 0;
  int add_first = 0;
  int matmul_first = 0;
  int shape_first = 0;

  int layer_id = 0;

  //
  /// ONNC defined operators @{
  void visit(const Initializer& pInitializer) override;
  void visit(const InputOperator& pInputOperator) override;
  void visit(const OutputOperator& pOutputOperator) override;
  /// @}

  /// ONNX defined operators @{
  void visit(const Conv& pConv) override;

  void visit(const MaxPool& pMaxPool) override;
  void visit(const Relu& pRelu) override;
  void visit(const AveragePool& pAveragePool) override;
  void visit(const Softmax& pSoftmax) override;
  void visit(const Gemm& pGemm) override;
  void visit(const Reshape& pReshape) override;

  void visit(const LRN& pLRN) override;
  void visit(const Concat& pConcat) override;

  void visit(const Add& pAdd) override;
  void visit(const MatMul& pMatMul) override;
  /// @}

  /// ONNC defined operators @{
  void visit(Initializer& pInitializer) override;
  void visit(InputOperator& pInputOperator) override;
  void visit(OutputOperator& pOutputOperator) override;
  /// @}

  /// ONNX defined operators @{
  void visit(Conv& pConv) override;

  void visit(MaxPool& pMaxPool) override;
  void visit(Relu& pRelu) override;
  void visit(AveragePool& pAveragePool) override;
  void visit(Softmax& pSoftmax) override;
  void visit(Gemm& pGemm) override;
  void visit(Reshape& pReshape) override;

  void visit(LRN& pLRN) override;
  void visit(Concat& pConcat) override;

  void visit(Add& pAdd) override;
  void visit(MatMul& pMatMul) override;
  /// @}

  // weight &  bias

  float* packWeight_or_Bias(const ComputeOperator& co, const Tensor* t,
                            int dims_0, int gidx, unsigned int size);

private:
  CortexmBackendMeta& m_pMeta;
};

} // namespace cortexm
} // namespace onnc

#endif
