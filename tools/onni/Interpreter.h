//===- Interpreter.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_INTERPRETER_INTERPRETER_H
#define ONNC_INTERPRETER_INTERPRETER_H
#include <onnc/IR/ONNXNodeVisitor.h>
#include <unordered_map>
#include <onnx/onnx.pb.h>

/** \class Interpreter
 *  \brief Interpreter dispatch compute ir to runtime.
 */
class Interpreter : public onnc::ONNXNodeVisitor
{
public:
  Interpreter(void *pContext) : m_pContext(pContext) {};

  typedef std::unordered_map<::onnx::Value *, void *> AddressTable;
  // XXX
  AddressTable m_ATable;
  void visitMultinomial(onnx::Node*);

  virtual void visitNode(::onnx::Node *pNode);
  virtual void visitAbs(::onnx::Node *pNode);
  virtual void visitAcos(::onnx::Node *pNode);
  virtual void visitAdd(::onnx::Node *pNode);
  virtual void visitAnd(::onnx::Node *pNode);
  virtual void visitArgMax(::onnx::Node *pNode);
  virtual void visitArgMin(::onnx::Node *pNode);
  virtual void visitAsin(::onnx::Node *pNode);
  virtual void visitAtan(::onnx::Node *pNode);
  virtual void visitAveragePool(::onnx::Node *pNode);
  virtual void visitBatchNormalization(::onnx::Node *pNode);
  virtual void visitCast(::onnx::Node *pNode);
  virtual void visitCeil(::onnx::Node *pNode);
  virtual void visitClip(::onnx::Node *pNode);
  virtual void visitConcat(::onnx::Node *pNode);
  virtual void visitConstant(::onnx::Node *pNode);
  virtual void visitConv(::onnx::Node *pNode);
  virtual void visitConvTranspose(::onnx::Node *pNode);
  virtual void visitCos(::onnx::Node *pNode);
  virtual void visitDepthToSpace(::onnx::Node *pNode);
  virtual void visitDiv(::onnx::Node *pNode);
  virtual void visitDropout(::onnx::Node *pNode);
  virtual void visitElu(::onnx::Node *pNode);
  virtual void visitEqual(::onnx::Node *pNode);
  virtual void visitExp(::onnx::Node *pNode);
  virtual void visitFlatten(::onnx::Node *pNode);
  virtual void visitFloor(::onnx::Node *pNode);
  virtual void visitGRU(::onnx::Node *pNode);
  virtual void visitGather(::onnx::Node *pNode);
  virtual void visitGemm(::onnx::Node *pNode);
  virtual void visitGlobalAveragePool(::onnx::Node *pNode);
  virtual void visitGlobalLpPool(::onnx::Node *pNode);
  virtual void visitGlobalMaxPool(::onnx::Node *pNode);
  virtual void visitGreater(::onnx::Node *pNode);
  virtual void visitHardSigmoid(::onnx::Node *pNode);
  virtual void visitHardmax(::onnx::Node *pNode);
  virtual void visitIdentity(::onnx::Node *pNode);
  virtual void visitInstanceNormalization(::onnx::Node *pNode);
  virtual void visitLRN(::onnx::Node *pNode);
  virtual void visitLSTM(::onnx::Node *pNode);
  virtual void visitLeakyRelu(::onnx::Node *pNode);
  virtual void visitLess(::onnx::Node *pNode);
  virtual void visitLog(::onnx::Node *pNode);
  virtual void visitLogSoftmax(::onnx::Node *pNode);
  virtual void visitLpNormalization(::onnx::Node *pNode);
  virtual void visitLpPool(::onnx::Node *pNode);
  virtual void visitMatMul(::onnx::Node *pNode);
  virtual void visitMax(::onnx::Node *pNode);
  virtual void visitMaxPool(::onnx::Node *pNode);
  virtual void visitMaxRoiPool(::onnx::Node *pNode);
  virtual void visitMean(::onnx::Node *pNode);
  virtual void visitMin(::onnx::Node *pNode);
  virtual void visitMul(::onnx::Node *pNode);
  virtual void visitNeg(::onnx::Node *pNode);
  virtual void visitNot(::onnx::Node *pNode);
  virtual void visitOr(::onnx::Node *pNode);
  virtual void visitPRelu(::onnx::Node *pNode);
  virtual void visitPad(::onnx::Node *pNode);
  virtual void visitPow(::onnx::Node *pNode);
  virtual void visitRNN(::onnx::Node *pNode);
  virtual void visitRandomNormal(::onnx::Node *pNode);
  virtual void visitRandomNormalLike(::onnx::Node *pNode);
  virtual void visitRandomUniform(::onnx::Node *pNode);
  virtual void visitRandomUniformLike(::onnx::Node *pNode);
  virtual void visitReciprocal(::onnx::Node *pNode);
  virtual void visitReduceL1(::onnx::Node *pNode);
  virtual void visitReduceL2(::onnx::Node *pNode);
  virtual void visitReduceLogSum(::onnx::Node *pNode);
  virtual void visitReduceLogSumExp(::onnx::Node *pNode);
  virtual void visitReduceMax(::onnx::Node *pNode);
  virtual void visitReduceMean(::onnx::Node *pNode);
  virtual void visitReduceMin(::onnx::Node *pNode);
  virtual void visitReduceProd(::onnx::Node *pNode);
  virtual void visitReduceSum(::onnx::Node *pNode);
  virtual void visitReduceSumSquare(::onnx::Node *pNode);
  virtual void visitRelu(::onnx::Node *pNode);
  virtual void visitReshape(::onnx::Node *pNode);
  virtual void visitSelu(::onnx::Node *pNode);
  virtual void visitShape(::onnx::Node *pNode);
  virtual void visitSigmoid(::onnx::Node *pNode);
  virtual void visitSin(::onnx::Node *pNode);
  virtual void visitSize(::onnx::Node *pNode);
  virtual void visitSlice(::onnx::Node *pNode);
  virtual void visitSoftmax(::onnx::Node *pNode);
  virtual void visitSoftplus(::onnx::Node *pNode);
  virtual void visitSoftsign(::onnx::Node *pNode);
  virtual void visitSpaceToDepth(::onnx::Node *pNode);
  virtual void visitSplit(::onnx::Node *pNode);
  virtual void visitSqrt(::onnx::Node *pNode);
  virtual void visitSqueeze(::onnx::Node *pNode);
  virtual void visitSub(::onnx::Node *pNode);
  virtual void visitSum(::onnx::Node *pNode);
  virtual void visitTan(::onnx::Node *pNode);
  virtual void visitTanh(::onnx::Node *pNode);
  virtual void visitTile(::onnx::Node *pNode);
  virtual void visitTopK(::onnx::Node *pNode);
  virtual void visitTranspose(::onnx::Node *pNode);
  virtual void visitUnsqueeze(::onnx::Node *pNode);
  virtual void visitUpsample(::onnx::Node *pNode);
  virtual void visitXor(::onnx::Node *pNode);
  virtual void visitATen(::onnx::Node *pNode);
  virtual void visitAffine(::onnx::Node *pNode);
  virtual void visitConstantFill(::onnx::Node *pNode);
  virtual void visitCrop(::onnx::Node *pNode);
  virtual void visitFC(::onnx::Node *pNode) {};
  virtual void visitGRUUnit(::onnx::Node *pNode);
  virtual void visitGivenTensorFill(::onnx::Node *pNode);
  virtual void visitIf(::onnx::Node *pNode);
  virtual void visitImageScaler(::onnx::Node *pNode);
  virtual void visitLoop(::onnx::Node *pNode);
  virtual void visitMeanVarianceNormalization(::onnx::Node *pNode);
  virtual void visitParametricSoftplus(::onnx::Node *pNode);
  virtual void visitScale(::onnx::Node *pNode);
  virtual void visitScaledTanh(::onnx::Node *pNode);
  virtual void visitThresholdedRelu(::onnx::Node *pNode);
  virtual void visitExpand(::onnx::Node *pNode);
  virtual void visitScan(::onnx::Node *pNode);

private:
  void *m_pContext;
};

#endif
