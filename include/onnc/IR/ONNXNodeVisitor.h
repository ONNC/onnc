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
#include <onnc/Config/ONNX.h>

namespace onnc {

class ONNXNodeVisitor
{
protected:
  ONNXNodeVisitor();

public:
  virtual ~ONNXNodeVisitor(){};
  void visit(xGraph *pGraph)
  {
    for (xNode *n : pGraph->nodes()) {
      visit(n);
    }
  }
  void visit(xNode *pNode)
  {
    // Call to the corresponding virtual member function.
    (this->*(m_JumpTable[static_cast<uint32_t>(pNode->kind())]))(pNode);
  }

  // clang-format off
private:
  void visitUndefined(xNode *pNode) {};
  virtual void visitNode(xNode *pNode) {};
  virtual void visitAbs(xNode *pNode) { visitNode(pNode); };
  virtual void visitAcos(xNode *pNode) { visitNode(pNode); };
  virtual void visitAdd(xNode *pNode) { visitNode(pNode); };
  virtual void visitAnd(xNode *pNode) { visitNode(pNode); };
  virtual void visitArgMax(xNode *pNode) { visitNode(pNode); };
  virtual void visitArgMin(xNode *pNode) { visitNode(pNode); };
  virtual void visitAsin(xNode *pNode) { visitNode(pNode); };
  virtual void visitAtan(xNode *pNode) { visitNode(pNode); };
  virtual void visitAveragePool(xNode *pNode) { visitNode(pNode); };
  virtual void visitBatchNormalization(xNode *pNode) { visitNode(pNode); };
  virtual void visitCast(xNode *pNode) { visitNode(pNode); };
  virtual void visitCeil(xNode *pNode) { visitNode(pNode); };
  virtual void visitClip(xNode *pNode) { visitNode(pNode); };
  virtual void visitConcat(xNode *pNode) { visitNode(pNode); };
  virtual void visitConstant(xNode *pNode) { visitNode(pNode); };
  virtual void visitConv(xNode *pNode) { visitNode(pNode); };
  virtual void visitConvTranspose(xNode *pNode) { visitNode(pNode); };
  virtual void visitCos(xNode *pNode) { visitNode(pNode); };
  virtual void visitDepthToSpace(xNode *pNode) { visitNode(pNode); };
  virtual void visitDiv(xNode *pNode) { visitNode(pNode); };
  virtual void visitDropout(xNode *pNode) { visitNode(pNode); };
  virtual void visitElu(xNode *pNode) { visitNode(pNode); };
  virtual void visitEqual(xNode *pNode) { visitNode(pNode); };
  virtual void visitExp(xNode *pNode) { visitNode(pNode); };
  virtual void visitFlatten(xNode *pNode) { visitNode(pNode); };
  virtual void visitFloor(xNode *pNode) { visitNode(pNode); };
  virtual void visitGRU(xNode *pNode) { visitNode(pNode); };
  virtual void visitGather(xNode *pNode) { visitNode(pNode); };
  virtual void visitGemm(xNode *pNode) { visitNode(pNode); };
  virtual void visitGlobalAveragePool(xNode *pNode) { visitNode(pNode); };
  virtual void visitGlobalLpPool(xNode *pNode) { visitNode(pNode); };
  virtual void visitGlobalMaxPool(xNode *pNode) { visitNode(pNode); };
  virtual void visitGreater(xNode *pNode) { visitNode(pNode); };
  virtual void visitHardSigmoid(xNode *pNode) { visitNode(pNode); };
  virtual void visitHardmax(xNode *pNode) { visitNode(pNode); };
  virtual void visitIdentity(xNode *pNode) { visitNode(pNode); };
  virtual void visitInstanceNormalization(xNode *pNode) { visitNode(pNode); };
  virtual void visitLRN(xNode *pNode) { visitNode(pNode); };
  virtual void visitLSTM(xNode *pNode) { visitNode(pNode); };
  virtual void visitLeakyRelu(xNode *pNode) { visitNode(pNode); };
  virtual void visitLess(xNode *pNode) { visitNode(pNode); };
  virtual void visitLog(xNode *pNode) { visitNode(pNode); };
  virtual void visitLogSoftmax(xNode *pNode) { visitNode(pNode); };
  virtual void visitLpNormalization(xNode *pNode) { visitNode(pNode); };
  virtual void visitLpPool(xNode *pNode) { visitNode(pNode); };
  virtual void visitMatMul(xNode *pNode) { visitNode(pNode); };
  virtual void visitMax(xNode *pNode) { visitNode(pNode); };
  virtual void visitMaxPool(xNode *pNode) { visitNode(pNode); };
  virtual void visitMaxRoiPool(xNode *pNode) { visitNode(pNode); };
  virtual void visitMean(xNode *pNode) { visitNode(pNode); };
  virtual void visitMin(xNode *pNode) { visitNode(pNode); };
  virtual void visitMul(xNode *pNode) { visitNode(pNode); };
  virtual void visitNeg(xNode *pNode) { visitNode(pNode); };
  virtual void visitNot(xNode *pNode) { visitNode(pNode); };
  virtual void visitOr(xNode *pNode) { visitNode(pNode); };
  virtual void visitPRelu(xNode *pNode) { visitNode(pNode); };
  virtual void visitPad(xNode *pNode) { visitNode(pNode); };
  virtual void visitPow(xNode *pNode) { visitNode(pNode); };
  virtual void visitRNN(xNode *pNode) { visitNode(pNode); };
  virtual void visitRandomNormal(xNode *pNode) { visitNode(pNode); };
  virtual void visitRandomNormalLike(xNode *pNode) { visitNode(pNode); };
  virtual void visitRandomUniform(xNode *pNode) { visitNode(pNode); };
  virtual void visitRandomUniformLike(xNode *pNode) { visitNode(pNode); };
  virtual void visitReciprocal(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceL1(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceL2(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceLogSum(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceLogSumExp(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceMax(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceMean(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceMin(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceProd(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceSum(xNode *pNode) { visitNode(pNode); };
  virtual void visitReduceSumSquare(xNode *pNode) { visitNode(pNode); };
  virtual void visitRelu(xNode *pNode) { visitNode(pNode); };
  virtual void visitReshape(xNode *pNode) { visitNode(pNode); };
  virtual void visitSelu(xNode *pNode) { visitNode(pNode); };
  virtual void visitShape(xNode *pNode) { visitNode(pNode); };
  virtual void visitSigmoid(xNode *pNode) { visitNode(pNode); };
  virtual void visitSin(xNode *pNode) { visitNode(pNode); };
  virtual void visitSize(xNode *pNode) { visitNode(pNode); };
  virtual void visitSlice(xNode *pNode) { visitNode(pNode); };
  virtual void visitSoftmax(xNode *pNode) { visitNode(pNode); };
  virtual void visitSoftplus(xNode *pNode) { visitNode(pNode); };
  virtual void visitSoftsign(xNode *pNode) { visitNode(pNode); };
  virtual void visitSpaceToDepth(xNode *pNode) { visitNode(pNode); };
  virtual void visitSplit(xNode *pNode) { visitNode(pNode); };
  virtual void visitSqrt(xNode *pNode) { visitNode(pNode); };
  virtual void visitSqueeze(xNode *pNode) { visitNode(pNode); };
  virtual void visitSub(xNode *pNode) { visitNode(pNode); };
  virtual void visitSum(xNode *pNode) { visitNode(pNode); };
  virtual void visitTan(xNode *pNode) { visitNode(pNode); };
  virtual void visitTanh(xNode *pNode) { visitNode(pNode); };
  virtual void visitTile(xNode *pNode) { visitNode(pNode); };
  virtual void visitTopK(xNode *pNode) { visitNode(pNode); };
  virtual void visitTranspose(xNode *pNode) { visitNode(pNode); };
  virtual void visitUnsqueeze(xNode *pNode) { visitNode(pNode); };
  virtual void visitUpsample(xNode *pNode) { visitNode(pNode); };
  virtual void visitXor(xNode *pNode) { visitNode(pNode); };
  virtual void visitATen(xNode *pNode) { visitNode(pNode); };
  virtual void visitAffine(xNode *pNode) { visitNode(pNode); };
  virtual void visitConstantFill(xNode *pNode) { visitNode(pNode); };
  virtual void visitCrop(xNode *pNode) { visitNode(pNode); };
  virtual void visitFC(xNode *pNode) { visitNode(pNode); };
  virtual void visitGRUUnit(xNode *pNode) { visitNode(pNode); };
  virtual void visitGivenTensorFill(xNode *pNode) { visitNode(pNode); };
  virtual void visitIf(xNode *pNode) { visitNode(pNode); };
  virtual void visitImageScaler(xNode *pNode) { visitNode(pNode); };
  virtual void visitLoop(xNode *pNode) { visitNode(pNode); };
  virtual void visitLoopIndexTensor(xNode *pNode) { visitNode(pNode); };
  virtual void visitMeanVarianceNormalization(xNode *pNode) { visitNode(pNode); };
  virtual void visitParametricSoftplus(xNode *pNode) { visitNode(pNode); };
  virtual void visitScale(xNode *pNode) { visitNode(pNode); };
  virtual void visitScaledTanh(xNode *pNode) { visitNode(pNode); };
  virtual void visitThresholdedRelu(xNode *pNode) { visitNode(pNode); };
  // clang-format on

private:
  typedef void (ONNXNodeVisitor::*VisitorFn)(xNode *);
  typedef std::vector<VisitorFn> VisitorFnTable;
  const VisitorFnTable &m_JumpTable;

protected:
  static const ONNXNodeVisitor::VisitorFnTable &getCachedJumpTable();
};

} // namespace onnc

#endif
