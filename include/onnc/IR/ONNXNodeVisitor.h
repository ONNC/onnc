// Auto-generated
//===- ONNXNodeVisitor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_ONNXNODEVISITOR_H
#define ONNC_IR_ONNXNODEVISITOR_H
#include <onnx/common/ir.h>

namespace onnc {

class ONNXNodeVisitor
{
protected:
  ONNXNodeVisitor();

public:
  virtual ~ONNXNodeVisitor(){};
  void visit(::onnx::Graph *pGraph)
  {
    for (::onnx::Node *n : pGraph->nodes()) {
      visit(n);
    }
  }
  void visit(::onnx::Node *pNode)
  {
    // Call to the corresponding virtual member function.
    (this->*(m_JumpTable[static_cast<uint32_t>(pNode->kind())]))(pNode);
  }

  // clang-format off
private:
  void visitUndefined(::onnx::Node *pNode) {};
  virtual void visitNode(::onnx::Node *pNode) {};
  virtual void visitAbs(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitAcos(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitAdd(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitAnd(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitArgMax(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitArgMin(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitAsin(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitAtan(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitAveragePool(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitBatchNormalization(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitCast(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitCeil(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitClip(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitConcat(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitConstant(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitConv(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitConvTranspose(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitCos(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitDepthToSpace(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitDiv(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitDropout(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitElu(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitEqual(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitExp(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitFlatten(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitFloor(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGRU(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGather(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGemm(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGlobalAveragePool(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGlobalLpPool(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGlobalMaxPool(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGreater(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitHardSigmoid(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitHardmax(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitIdentity(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitInstanceNormalization(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLRN(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLSTM(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLeakyRelu(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLess(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLog(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLogSoftmax(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLpNormalization(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLpPool(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitMatMul(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitMax(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitMaxPool(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitMaxRoiPool(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitMean(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitMin(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitMul(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitNeg(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitNot(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitOr(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitPRelu(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitPad(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitPow(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitRNN(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitRandomNormal(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitRandomNormalLike(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitRandomUniform(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitRandomUniformLike(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReciprocal(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceL1(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceL2(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceLogSum(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceLogSumExp(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceMax(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceMean(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceMin(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceProd(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceSum(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReduceSumSquare(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitRelu(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitReshape(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSelu(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitShape(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSigmoid(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSin(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSize(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSlice(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSoftmax(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSoftplus(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSoftsign(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSpaceToDepth(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSplit(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSqrt(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSqueeze(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSub(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitSum(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitTan(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitTanh(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitTile(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitTopK(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitTranspose(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitUnsqueeze(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitUpsample(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitXor(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitATen(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitAffine(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitConstantFill(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitCrop(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitFC(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGRUUnit(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitGivenTensorFill(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitIf(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitImageScaler(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLoop(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitLoopIndexTensor(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitMeanVarianceNormalization(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitParametricSoftplus(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitScale(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitScaledTanh(::onnx::Node *pNode) { visitNode(pNode); };
  virtual void visitThresholdedRelu(::onnx::Node *pNode) { visitNode(pNode); };
  // clang-format on

private:
  typedef void (ONNXNodeVisitor::*VisitorFn)(::onnx::Node *);
  typedef std::vector<VisitorFn> VisitorFnTable;
  const VisitorFnTable &m_JumpTable;

protected:
  static const ONNXNodeVisitor::VisitorFnTable &getCachedJumpTable();
};

} // namespace onnc

#endif
