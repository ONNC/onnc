//===- Interpreter.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_RUNTIME_INTERPRETER_H
#define ONNC_RUNTIME_INTERPRETER_H
#include <cstddef>
#include <memory>

#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Value.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <onnc/IR/CustomVisitor.h>
#include <onnc/Support/Memory.h>
#include <unordered_map>

namespace onnc {
class BasicInterpreter
{
public:
  // XXX(a127a127): Temporary hack, should not public.
  typedef std::unordered_map<const Value *, void *> AddressTable;
  AddressTable m_ATable;
  void *m_pContext;

  virtual ~BasicInterpreter() = default;

  void visit(Abs& pAbs);
  void visit(Acos& pAcos);
  void visit(Add& pAdd);
  void visit(And& pAnd);
  void visit(ArgMax& pArgMax);
  void visit(ArgMin& pArgMin);
  void visit(Asin& pAsin);
  void visit(Atan& pAtan);
  void visit(AveragePool& pAveragePool);
  void visit(BatchNormalization& pBatchNormalization);
  void visit(Cast& pCast);
  void visit(Ceil& pCeil);
  void visit(Clip& pClip);
  void visit(Concat& pConcat);
  void visit(Constant& pConstant);
  void visit(Conv& pConv);
  void visit(ConvTranspose& pConvTranspose);
  void visit(Cos& pCos);
  void visit(DepthToSpace& pDepthToSpace);
  void visit(Div& pDiv);
  void visit(Dropout& pDropout);
  void visit(Elu& pElu);
  void visit(Equal& pEqual);
  void visit(Exp& pExp);
  void visit(Expand& pExpand);
  void visit(Flatten& pFlatten);
  void visit(Floor& pFloor);
  void visit(GRU& pGRU);
  void visit(Gather& pGather);
  void visit(Gemm& pGemm);
  void visit(GlobalAveragePool& pGlobalAveragePool);
  void visit(GlobalLpPool& pGlobalLpPool);
  void visit(GlobalMaxPool& pGlobalMaxPool);
  void visit(Greater& pGreater);
  void visit(HardSigmoid& pHardSigmoid);
  void visit(Hardmax& pHardmax);
  void visit(Identity& pIdentity);
  void visit(InstanceNormalization& pInstanceNormalization);
  void visit(LRN& pLRN);
  void visit(LSTM& pLSTM);
  void visit(LeakyRelu& pLeakyRelu);
  void visit(Less& pLess);
  void visit(Log& pLog);
  void visit(LogSoftmax& pLogSoftmax);
  void visit(LpNormalization& pLpNormalization);
  void visit(LpPool& pLpPool);
  void visit(MatMul& pMatMul);
  void visit(Max& pMax);
  void visit(MaxPool& pMaxPool);
  void visit(MaxRoiPool& pMaxRoiPool);
  void visit(Mean& pMean);
  void visit(Min& pMin);
  void visit(Mul& pMul);
  void visit(Multinomial& pMultinomial);
  void visit(Neg& pNeg);
  void visit(Not& pNot);
  void visit(Or& pOr);
  void visit(PRelu& pPRelu);
  void visit(Pad& pPad);
  void visit(Pow& pPow);
  void visit(RNN& pRNN);
  void visit(RandomNormal& pRandomNormal);
  void visit(RandomNormalLike& pRandomNormalLike);
  void visit(RandomUniform& pRandomUniform);
  void visit(RandomUniformLike& pRandomUniformLike);
  void visit(Reciprocal& pReciprocal);
  void visit(ReduceL1& pReduceL1);
  void visit(ReduceL2& pReduceL2);
  void visit(ReduceLogSum& pReduceLogSum);
  void visit(ReduceLogSumExp& pReduceLogSumExp);
  void visit(ReduceMax& pReduceMax);
  void visit(ReduceMean& pReduceMean);
  void visit(ReduceMin& pReduceMin);
  void visit(ReduceProd& pReduceProd);
  void visit(ReduceSum& pReduceSum);
  void visit(ReduceSumSquare& pReduceSumSquare);
  void visit(Relu& pRelu);
  void visit(Reshape& pReshape);
  void visit(Selu& pSelu);
  void visit(Shape& pShape);
  void visit(Sigmoid& pSigmoid);
  void visit(Sin& pSin);
  void visit(Size& pSize);
  void visit(Slice& pSlice);
  void visit(Softmax& pSoftmax);
  void visit(Softplus& pSoftplus);
  void visit(Softsign& pSoftsign);
  void visit(SpaceToDepth& pSpaceToDepth);
  void visit(Split& pSplit);
  void visit(Sqrt& pSqrt);
  void visit(Squeeze& pSqueeze);
  void visit(Sub& pSub);
  void visit(Sum& pSum);
  void visit(Tan& pTan);
  void visit(Tanh& pTanh);
  void visit(Tile& pTile);
  void visit(TopK& pTopK);
  void visit(Transpose& pTranspose);
  void visit(Unsqueeze& pUnsqueeze);
  void visit(Upsample& pUpsample);
  void visit(Xor& pXor);
  void visit(ATen& pATen);
  void visit(Affine& pAffine);
  void visit(ConstantFill& pConstantFill);
  void visit(Crop& pCrop);
  void visit(GRUUnit& pGRUUnit);
  void visit(GivenTensorFill& pGivenTensorFill);
  void visit(ImageScaler& pImageScaler);
  void visit(MeanVarianceNormalization& pMeanVarianceNormalization);
  void visit(ParametricSoftplus& pParametricSoftplus);
  void visit(Scale& pScale);
  void visit(ScaledTanh& pScaledTanh);
  void visit(ThresholdedRelu& pThresholdedRelu);
};

// TODO: Re-design BasicInterpreter.
template<typename OperatorVisitorT = ComputeVisitor>
class InterpreterVisitor
  : public CustomVisitor<InterpreterVisitor<OperatorVisitorT>, OperatorVisitorT>
  , public BasicInterpreter
{
public:
  void visit(Abs& pAbs) override { BasicInterpreter::visit(pAbs); }
  void visit(Acos& pAcos) override { BasicInterpreter::visit(pAcos); }
  void visit(Add& pAdd) override { BasicInterpreter::visit(pAdd); }
  void visit(And& pAnd) override { BasicInterpreter::visit(pAnd); }
  void visit(ArgMax& pArgMax) override { BasicInterpreter::visit(pArgMax); }
  void visit(ArgMin& pArgMin) override { BasicInterpreter::visit(pArgMin); }
  void visit(Asin& pAsin) override { BasicInterpreter::visit(pAsin); }
  void visit(Atan& pAtan) override { BasicInterpreter::visit(pAtan); }
  void visit(AveragePool& pAveragePool) override { BasicInterpreter::visit(pAveragePool); }
  void visit(BatchNormalization& pBatchNormalization) override { BasicInterpreter::visit(pBatchNormalization); }
  void visit(Cast& pCast) override { BasicInterpreter::visit(pCast); }
  void visit(Ceil& pCeil) override { BasicInterpreter::visit(pCeil); }
  void visit(Clip& pClip) override { BasicInterpreter::visit(pClip); }
  void visit(Concat& pConcat) override { BasicInterpreter::visit(pConcat); }
  void visit(Constant& pConstant) override { BasicInterpreter::visit(pConstant); }
  void visit(Conv& pConv) override { BasicInterpreter::visit(pConv); }
  void visit(ConvTranspose& pConvTranspose) override { BasicInterpreter::visit(pConvTranspose); }
  void visit(Cos& pCos) override { BasicInterpreter::visit(pCos); }
  void visit(DepthToSpace& pDepthToSpace) override { BasicInterpreter::visit(pDepthToSpace); }
  void visit(Div& pDiv) override { BasicInterpreter::visit(pDiv); }
  void visit(Dropout& pDropout) override { BasicInterpreter::visit(pDropout); }
  void visit(Elu& pElu) override { BasicInterpreter::visit(pElu); }
  void visit(Equal& pEqual) override { BasicInterpreter::visit(pEqual); }
  void visit(Exp& pExp) override { BasicInterpreter::visit(pExp); }
  void visit(Expand& pExpand) override { BasicInterpreter::visit(pExpand); }
  void visit(Flatten& pFlatten) override { BasicInterpreter::visit(pFlatten); }
  void visit(Floor& pFloor) override { BasicInterpreter::visit(pFloor); }
  void visit(GRU& pGRU) override { BasicInterpreter::visit(pGRU); }
  void visit(Gather& pGather) override { BasicInterpreter::visit(pGather); }
  void visit(Gemm& pGemm) override { BasicInterpreter::visit(pGemm); }
  void visit(GlobalAveragePool& pGlobalAveragePool) override { BasicInterpreter::visit(pGlobalAveragePool); }
  void visit(GlobalLpPool& pGlobalLpPool) override { BasicInterpreter::visit(pGlobalLpPool); }
  void visit(GlobalMaxPool& pGlobalMaxPool) override { BasicInterpreter::visit(pGlobalMaxPool); }
  void visit(Greater& pGreater) override { BasicInterpreter::visit(pGreater); }
  void visit(HardSigmoid& pHardSigmoid) override { BasicInterpreter::visit(pHardSigmoid); }
  void visit(Hardmax& pHardmax) override { BasicInterpreter::visit(pHardmax); }
  void visit(Identity& pIdentity) override { BasicInterpreter::visit(pIdentity); }
  void visit(InstanceNormalization& pInstanceNormalization) override { BasicInterpreter::visit(pInstanceNormalization); }
  void visit(LRN& pLRN) override { BasicInterpreter::visit(pLRN); }
  void visit(LSTM& pLSTM) override { BasicInterpreter::visit(pLSTM); }
  void visit(LeakyRelu& pLeakyRelu) override { BasicInterpreter::visit(pLeakyRelu); }
  void visit(Less& pLess) override { BasicInterpreter::visit(pLess); }
  void visit(Log& pLog) override { BasicInterpreter::visit(pLog); }
  void visit(LogSoftmax& pLogSoftmax) override { BasicInterpreter::visit(pLogSoftmax); }
  void visit(LpNormalization& pLpNormalization) override { BasicInterpreter::visit(pLpNormalization); }
  void visit(LpPool& pLpPool) override { BasicInterpreter::visit(pLpPool); }
  void visit(MatMul& pMatMul) override { BasicInterpreter::visit(pMatMul); }
  void visit(Max& pMax) override { BasicInterpreter::visit(pMax); }
  void visit(MaxPool& pMaxPool) override { BasicInterpreter::visit(pMaxPool); }
  void visit(MaxRoiPool& pMaxRoiPool) override { BasicInterpreter::visit(pMaxRoiPool); }
  void visit(Mean& pMean) override { BasicInterpreter::visit(pMean); }
  void visit(Min& pMin) override { BasicInterpreter::visit(pMin); }
  void visit(Mul& pMul) override { BasicInterpreter::visit(pMul); }
  void visit(Multinomial& pMultinomial) override { BasicInterpreter::visit(pMultinomial); }
  void visit(Neg& pNeg) override { BasicInterpreter::visit(pNeg); }
  void visit(Not& pNot) override { BasicInterpreter::visit(pNot); }
  void visit(Or& pOr) override { BasicInterpreter::visit(pOr); }
  void visit(PRelu& pPRelu) override { BasicInterpreter::visit(pPRelu); }
  void visit(Pad& pPad) override { BasicInterpreter::visit(pPad); }
  void visit(Pow& pPow) override { BasicInterpreter::visit(pPow); }
  void visit(RNN& pRNN) override { BasicInterpreter::visit(pRNN); }
  void visit(RandomNormal& pRandomNormal) override { BasicInterpreter::visit(pRandomNormal); }
  void visit(RandomNormalLike& pRandomNormalLike) override { BasicInterpreter::visit(pRandomNormalLike); }
  void visit(RandomUniform& pRandomUniform) override { BasicInterpreter::visit(pRandomUniform); }
  void visit(RandomUniformLike& pRandomUniformLike) override { BasicInterpreter::visit(pRandomUniformLike); }
  void visit(Reciprocal& pReciprocal) override { BasicInterpreter::visit(pReciprocal); }
  void visit(ReduceL1& pReduceL1) override { BasicInterpreter::visit(pReduceL1); }
  void visit(ReduceL2& pReduceL2) override { BasicInterpreter::visit(pReduceL2); }
  void visit(ReduceLogSum& pReduceLogSum) override { BasicInterpreter::visit(pReduceLogSum); }
  void visit(ReduceLogSumExp& pReduceLogSumExp) override { BasicInterpreter::visit(pReduceLogSumExp); }
  void visit(ReduceMax& pReduceMax) override { BasicInterpreter::visit(pReduceMax); }
  void visit(ReduceMean& pReduceMean) override { BasicInterpreter::visit(pReduceMean); }
  void visit(ReduceMin& pReduceMin) override { BasicInterpreter::visit(pReduceMin); }
  void visit(ReduceProd& pReduceProd) override { BasicInterpreter::visit(pReduceProd); }
  void visit(ReduceSum& pReduceSum) override { BasicInterpreter::visit(pReduceSum); }
  void visit(ReduceSumSquare& pReduceSumSquare) override { BasicInterpreter::visit(pReduceSumSquare); }
  void visit(Relu& pRelu) override { BasicInterpreter::visit(pRelu); }
  void visit(Reshape& pReshape) override { BasicInterpreter::visit(pReshape); }
  void visit(Selu& pSelu) override { BasicInterpreter::visit(pSelu); }
  void visit(Shape& pShape) override { BasicInterpreter::visit(pShape); }
  void visit(Sigmoid& pSigmoid) override { BasicInterpreter::visit(pSigmoid); }
  void visit(Sin& pSin) override { BasicInterpreter::visit(pSin); }
  void visit(Size& pSize) override { BasicInterpreter::visit(pSize); }
  void visit(Slice& pSlice) override { BasicInterpreter::visit(pSlice); }
  void visit(Softmax& pSoftmax) override { BasicInterpreter::visit(pSoftmax); }
  void visit(Softplus& pSoftplus) override { BasicInterpreter::visit(pSoftplus); }
  void visit(Softsign& pSoftsign) override { BasicInterpreter::visit(pSoftsign); }
  void visit(SpaceToDepth& pSpaceToDepth) override { BasicInterpreter::visit(pSpaceToDepth); }
  void visit(Split& pSplit) override { BasicInterpreter::visit(pSplit); }
  void visit(Sqrt& pSqrt) override { BasicInterpreter::visit(pSqrt); }
  void visit(Squeeze& pSqueeze) override { BasicInterpreter::visit(pSqueeze); }
  void visit(Sub& pSub) override { BasicInterpreter::visit(pSub); }
  void visit(Sum& pSum) override { BasicInterpreter::visit(pSum); }
  void visit(Tan& pTan) override { BasicInterpreter::visit(pTan); }
  void visit(Tanh& pTanh) override { BasicInterpreter::visit(pTanh); }
  void visit(Tile& pTile) override { BasicInterpreter::visit(pTile); }
  void visit(TopK& pTopK) override { BasicInterpreter::visit(pTopK); }
  void visit(Transpose& pTranspose) override { BasicInterpreter::visit(pTranspose); }
  void visit(Unsqueeze& pUnsqueeze) override { BasicInterpreter::visit(pUnsqueeze); }
  void visit(Upsample& pUpsample) override { BasicInterpreter::visit(pUpsample); }
  void visit(Xor& pXor) override { BasicInterpreter::visit(pXor); }
  void visit(ATen& pATen) override { BasicInterpreter::visit(pATen); }
  void visit(Affine& pAffine) override { BasicInterpreter::visit(pAffine); }
  void visit(ConstantFill& pConstantFill) override { BasicInterpreter::visit(pConstantFill); }
  void visit(Crop& pCrop) override { BasicInterpreter::visit(pCrop); }
  void visit(GRUUnit& pGRUUnit) override { BasicInterpreter::visit(pGRUUnit); }
  void visit(GivenTensorFill& pGivenTensorFill) override { BasicInterpreter::visit(pGivenTensorFill); }
  void visit(ImageScaler& pImageScaler) override { BasicInterpreter::visit(pImageScaler); }
  void visit(MeanVarianceNormalization& pMeanVarianceNormalization) override { BasicInterpreter::visit(pMeanVarianceNormalization); }
  void visit(ParametricSoftplus& pParametricSoftplus) override { BasicInterpreter::visit(pParametricSoftplus); }
  void visit(Scale& pScale) override { BasicInterpreter::visit(pScale); }
  void visit(ScaledTanh& pScaledTanh) override { BasicInterpreter::visit(pScaledTanh); }
  void visit(ThresholdedRelu& pThresholdedRelu) override { BasicInterpreter::visit(pThresholdedRelu); }
};

/** \class Interpreter
 *  \brief Interpreter dispatch compute ir to runtime.
 */
class Interpreter
{
protected:
  Interpreter(ComputeVisitor *pIV) : m_pIV(pIV) {}

public:
  Interpreter()
      : m_pIV{std::make_unique<InterpreterVisitor<>>()} {}
  Interpreter(const Interpreter&) = delete;
  virtual ~Interpreter() = default;

  // XXX(a127a127):
  //   Temporary hack, should use something like "Context" or "Environment",
  //   should not expose Interpreter visitor implementation.
  virtual BasicInterpreter *getBasicInterpreter() {
    return static_cast<InterpreterVisitor<>*>(m_pIV.get());
  }

  ComputeVisitor &getVisitor() { return *m_pIV; }

private:
  std::unique_ptr<ComputeVisitor> m_pIV;
};

} // namespace of onnc

#endif
