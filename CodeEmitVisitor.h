//===- CodeEmitVisitor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_CODE_EMIT_VISITOR_H
#define TARGET_NVDLA_CODE_EMIT_VISITOR_H
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include "NvdlaMeta.h"

namespace onnc {

namespace nvdla {

class CodeEmitVisitor : public ComputeVisitor
{
public:
  NvdlaBackendMeta *m_pMeta;
  static char ID;

  /// ONNC defined operators @{
  void visit(const Initializer& pInitializer) override;
  void visit(const InputOperator& pInputOperator) override;
  void visit(const OutputOperator& pOutputOperator) override;
  /// @}

  /// ONNX defined operators @{
  void visit(const Conv& pConv) override;
  void visit(const Reshape& pReshape) override;
  void visit(const Relu& pRelu) override;
  void visit(const LRN& pLRN) override;
  void visit(const MaxPool& pMaxPool) override;
  void visit(const AveragePool& pAveragePool) override;
  void visit(const Gemm& pGemm) override;
  void visit(const Softmax& pSoftmax) override;
  void visit(const Concat& pConcat) override;
  /// @}
  //void weight_pack(void *buf, float *data, int G, int g, int dim[4], int type);
  void conv(const Tensor *x, const Tensor *w, const Tensor *b, const Tensor *y);
  int packWeight(const Tensor *t, int dims[4], int gidx);
  int packBias(const Tensor *t, int dims[4], int gidx);

  void issueEmuOp(NvdlaEmuOperation *op);
  int issueEmuAddr(int mid);
  void issueDlaOp(NvdlaDlaOperation *op, NvdlaDlaOperation *op_fuse, NvdlaDlaOperation *op_prev);
  int issueDlaAddr(int mid, NvdlaCubeInfo cube, int groups, int gidx, int hofs);
};

} // namespace nvdla
} // namespace onnc

#endif
