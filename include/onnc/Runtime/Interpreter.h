//===- Interpreter.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_RUNTIME_INTERPRETER_H
#define ONNC_RUNTIME_INTERPRETER_H
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Value.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <unordered_map>
#include <cstddef>

namespace onnc {
/** \class Interpreter
 *  \brief Interpreter dispatch compute ir to runtime.
 */
class InterpreterBase : public ComputeVisitor
{
public:
  // XXX
  typedef std::unordered_map<Value *, void *> AddressTable;
  AddressTable m_ATable;
  void *m_pContext;

  virtual void visit(Abs& pAbs);
  virtual void visit(Acos& pAcos);
  virtual void visit(Add& pAdd);
  virtual void visit(And& pAnd);
  virtual void visit(ArgMax& pArgMax);
  virtual void visit(ArgMin& pArgMin);
  virtual void visit(Asin& pAsin);
  virtual void visit(Atan& pAtan);
  virtual void visit(AveragePool& pAveragePool);
  virtual void visit(BatchNormalization& pBatchNormalization);
  virtual void visit(Cast& pCast);
  virtual void visit(Ceil& pCeil);
  virtual void visit(Clip& pClip);
  virtual void visit(Concat& pConcat);
  virtual void visit(Constant& pConstant);
  virtual void visit(Conv& pConv);
  virtual void visit(ConvTranspose& pConvTranspose);
  virtual void visit(Cos& pCos);
  virtual void visit(DepthToSpace& pDepthToSpace);
  virtual void visit(Div& pDiv);
  virtual void visit(Dropout& pDropout);
  virtual void visit(Elu& pElu);
  virtual void visit(Equal& pEqual);
  virtual void visit(Exp& pExp);
  virtual void visit(Expand& pExpand);
  virtual void visit(Flatten& pFlatten);
  virtual void visit(Floor& pFloor);
  virtual void visit(GRU& pGRU);
  virtual void visit(Gather& pGather);
  virtual void visit(Gemm& pGemm);
  virtual void visit(GlobalAveragePool& pGlobalAveragePool);
  virtual void visit(GlobalLpPool& pGlobalLpPool);
  virtual void visit(GlobalMaxPool& pGlobalMaxPool);
  virtual void visit(Greater& pGreater);
  virtual void visit(HardSigmoid& pHardSigmoid);
  virtual void visit(Hardmax& pHardmax);
  virtual void visit(Identity& pIdentity);
  virtual void visit(InstanceNormalization& pInstanceNormalization);
  virtual void visit(LRN& pLRN);
  virtual void visit(LSTM& pLSTM);
  virtual void visit(LeakyRelu& pLeakyRelu);
  virtual void visit(Less& pLess);
  virtual void visit(Log& pLog);
  virtual void visit(LogSoftmax& pLogSoftmax);
  virtual void visit(LpNormalization& pLpNormalization);
  virtual void visit(LpPool& pLpPool);
  virtual void visit(MatMul& pMatMul);
  virtual void visit(Max& pMax);
  virtual void visit(MaxPool& pMaxPool);
  virtual void visit(MaxRoiPool& pMaxRoiPool);
  virtual void visit(Mean& pMean);
  virtual void visit(Min& pMin);
  virtual void visit(Mul& pMul);
  virtual void visit(Multinomial& pMultinomial);
  virtual void visit(Neg& pNeg);
  virtual void visit(Not& pNot);
  virtual void visit(Or& pOr);
  virtual void visit(PRelu& pPRelu);
  virtual void visit(Pad& pPad);
  virtual void visit(Pow& pPow);
  virtual void visit(RNN& pRNN);
  virtual void visit(RandomNormal& pRandomNormal);
  virtual void visit(RandomNormalLike& pRandomNormalLike);
  virtual void visit(RandomUniform& pRandomUniform);
  virtual void visit(RandomUniformLike& pRandomUniformLike);
  virtual void visit(Reciprocal& pReciprocal);
  virtual void visit(ReduceL1& pReduceL1);
  virtual void visit(ReduceL2& pReduceL2);
  virtual void visit(ReduceLogSum& pReduceLogSum);
  virtual void visit(ReduceLogSumExp& pReduceLogSumExp);
  virtual void visit(ReduceMax& pReduceMax);
  virtual void visit(ReduceMean& pReduceMean);
  virtual void visit(ReduceMin& pReduceMin);
  virtual void visit(ReduceProd& pReduceProd);
  virtual void visit(ReduceSum& pReduceSum);
  virtual void visit(ReduceSumSquare& pReduceSumSquare);
  virtual void visit(Relu& pRelu);
  virtual void visit(Reshape& pReshape);
  virtual void visit(Selu& pSelu);
  virtual void visit(Shape& pShape);
  virtual void visit(Sigmoid& pSigmoid);
  virtual void visit(Sin& pSin);
  virtual void visit(Size& pSize);
  virtual void visit(Slice& pSlice);
  virtual void visit(Softmax& pSoftmax);
  virtual void visit(Softplus& pSoftplus);
  virtual void visit(Softsign& pSoftsign);
  virtual void visit(SpaceToDepth& pSpaceToDepth);
  virtual void visit(Split& pSplit);
  virtual void visit(Sqrt& pSqrt);
  virtual void visit(Squeeze& pSqueeze);
  virtual void visit(Sub& pSub);
  virtual void visit(Sum& pSum);
  virtual void visit(Tan& pTan);
  virtual void visit(Tanh& pTanh);
  virtual void visit(Tile& pTile);
  virtual void visit(TopK& pTopK);
  virtual void visit(Transpose& pTranspose);
  virtual void visit(Unsqueeze& pUnsqueeze);
  virtual void visit(Upsample& pUpsample);
  virtual void visit(Xor& pXor);
  virtual void visit(ATen& pATen);
  virtual void visit(Affine& pAffine);
  virtual void visit(ConstantFill& pConstantFill);
  virtual void visit(Crop& pCrop);
  virtual void visit(GRUUnit& pGRUUnit);
  virtual void visit(GivenTensorFill& pGivenTensorFill);
  virtual void visit(ImageScaler& pImageScaler);
  virtual void visit(MeanVarianceNormalization& pMeanVarianceNormalization);
  virtual void visit(ParametricSoftplus& pParametricSoftplus);
  virtual void visit(Scale& pScale);
  virtual void visit(ScaledTanh& pScaledTanh);
  virtual void visit(ThresholdedRelu& pThresholdedRelu);
};

template<typename FullOperatorVisitor>
class Interpreter : public FullOperatorVisitor, public InterpreterBase
{
public:
  virtual void visit(Abs& pAbs) { InterpreterBase::visit(pAbs); }
  virtual void visit(Acos& pAcos) { InterpreterBase::visit(pAcos); }
  virtual void visit(Add& pAdd) { InterpreterBase::visit(pAdd); }
  virtual void visit(And& pAnd) { InterpreterBase::visit(pAnd); }
  virtual void visit(ArgMax& pArgMax) { InterpreterBase::visit(pArgMax); }
  virtual void visit(ArgMin& pArgMin) { InterpreterBase::visit(pArgMin); }
  virtual void visit(Asin& pAsin) { InterpreterBase::visit(pAsin); }
  virtual void visit(Atan& pAtan) { InterpreterBase::visit(pAtan); }
  virtual void visit(AveragePool& pAveragePool) { InterpreterBase::visit(pAveragePool); }
  virtual void visit(BatchNormalization& pBatchNormalization) { InterpreterBase::visit(pBatchNormalization); }
  virtual void visit(Cast& pCast) { InterpreterBase::visit(pCast); }
  virtual void visit(Ceil& pCeil) { InterpreterBase::visit(pCeil); }
  virtual void visit(Clip& pClip) { InterpreterBase::visit(pClip); }
  virtual void visit(Concat& pConcat) { InterpreterBase::visit(pConcat); }
  virtual void visit(Constant& pConstant) { InterpreterBase::visit(pConstant); }
  virtual void visit(Conv& pConv) { InterpreterBase::visit(pConv); }
  virtual void visit(ConvTranspose& pConvTranspose) { InterpreterBase::visit(pConvTranspose); }
  virtual void visit(Cos& pCos) { InterpreterBase::visit(pCos); }
  virtual void visit(DepthToSpace& pDepthToSpace) { InterpreterBase::visit(pDepthToSpace); }
  virtual void visit(Div& pDiv) { InterpreterBase::visit(pDiv); }
  virtual void visit(Dropout& pDropout) { InterpreterBase::visit(pDropout); }
  virtual void visit(Elu& pElu) { InterpreterBase::visit(pElu); }
  virtual void visit(Equal& pEqual) { InterpreterBase::visit(pEqual); }
  virtual void visit(Exp& pExp) { InterpreterBase::visit(pExp); }
  virtual void visit(Expand& pExpand) { InterpreterBase::visit(pExpand); }
  virtual void visit(Flatten& pFlatten) { InterpreterBase::visit(pFlatten); }
  virtual void visit(Floor& pFloor) { InterpreterBase::visit(pFloor); }
  virtual void visit(GRU& pGRU) { InterpreterBase::visit(pGRU); }
  virtual void visit(Gather& pGather) { InterpreterBase::visit(pGather); }
  virtual void visit(Gemm& pGemm) { InterpreterBase::visit(pGemm); }
  virtual void visit(GlobalAveragePool& pGlobalAveragePool) { InterpreterBase::visit(pGlobalAveragePool); }
  virtual void visit(GlobalLpPool& pGlobalLpPool) { InterpreterBase::visit(pGlobalLpPool); }
  virtual void visit(GlobalMaxPool& pGlobalMaxPool) { InterpreterBase::visit(pGlobalMaxPool); }
  virtual void visit(Greater& pGreater) { InterpreterBase::visit(pGreater); }
  virtual void visit(HardSigmoid& pHardSigmoid) { InterpreterBase::visit(pHardSigmoid); }
  virtual void visit(Hardmax& pHardmax) { InterpreterBase::visit(pHardmax); }
  virtual void visit(Identity& pIdentity) { InterpreterBase::visit(pIdentity); }
  virtual void visit(InstanceNormalization& pInstanceNormalization) { InterpreterBase::visit(pInstanceNormalization); }
  virtual void visit(LRN& pLRN) { InterpreterBase::visit(pLRN); }
  virtual void visit(LSTM& pLSTM) { InterpreterBase::visit(pLSTM); }
  virtual void visit(LeakyRelu& pLeakyRelu) { InterpreterBase::visit(pLeakyRelu); }
  virtual void visit(Less& pLess) { InterpreterBase::visit(pLess); }
  virtual void visit(Log& pLog) { InterpreterBase::visit(pLog); }
  virtual void visit(LogSoftmax& pLogSoftmax) { InterpreterBase::visit(pLogSoftmax); }
  virtual void visit(LpNormalization& pLpNormalization) { InterpreterBase::visit(pLpNormalization); }
  virtual void visit(LpPool& pLpPool) { InterpreterBase::visit(pLpPool); }
  virtual void visit(MatMul& pMatMul) { InterpreterBase::visit(pMatMul); }
  virtual void visit(Max& pMax) { InterpreterBase::visit(pMax); }
  virtual void visit(MaxPool& pMaxPool) { InterpreterBase::visit(pMaxPool); }
  virtual void visit(MaxRoiPool& pMaxRoiPool) { InterpreterBase::visit(pMaxRoiPool); }
  virtual void visit(Mean& pMean) { InterpreterBase::visit(pMean); }
  virtual void visit(Min& pMin) { InterpreterBase::visit(pMin); }
  virtual void visit(Mul& pMul) { InterpreterBase::visit(pMul); }
  virtual void visit(Multinomial& pMultinomial) { InterpreterBase::visit(pMultinomial); }
  virtual void visit(Neg& pNeg) { InterpreterBase::visit(pNeg); }
  virtual void visit(Not& pNot) { InterpreterBase::visit(pNot); }
  virtual void visit(Or& pOr) { InterpreterBase::visit(pOr); }
  virtual void visit(PRelu& pPRelu) { InterpreterBase::visit(pPRelu); }
  virtual void visit(Pad& pPad) { InterpreterBase::visit(pPad); }
  virtual void visit(Pow& pPow) { InterpreterBase::visit(pPow); }
  virtual void visit(RNN& pRNN) { InterpreterBase::visit(pRNN); }
  virtual void visit(RandomNormal& pRandomNormal) { InterpreterBase::visit(pRandomNormal); }
  virtual void visit(RandomNormalLike& pRandomNormalLike) { InterpreterBase::visit(pRandomNormalLike); }
  virtual void visit(RandomUniform& pRandomUniform) { InterpreterBase::visit(pRandomUniform); }
  virtual void visit(RandomUniformLike& pRandomUniformLike) { InterpreterBase::visit(pRandomUniformLike); }
  virtual void visit(Reciprocal& pReciprocal) { InterpreterBase::visit(pReciprocal); }
  virtual void visit(ReduceL1& pReduceL1) { InterpreterBase::visit(pReduceL1); }
  virtual void visit(ReduceL2& pReduceL2) { InterpreterBase::visit(pReduceL2); }
  virtual void visit(ReduceLogSum& pReduceLogSum) { InterpreterBase::visit(pReduceLogSum); }
  virtual void visit(ReduceLogSumExp& pReduceLogSumExp) { InterpreterBase::visit(pReduceLogSumExp); }
  virtual void visit(ReduceMax& pReduceMax) { InterpreterBase::visit(pReduceMax); }
  virtual void visit(ReduceMean& pReduceMean) { InterpreterBase::visit(pReduceMean); }
  virtual void visit(ReduceMin& pReduceMin) { InterpreterBase::visit(pReduceMin); }
  virtual void visit(ReduceProd& pReduceProd) { InterpreterBase::visit(pReduceProd); }
  virtual void visit(ReduceSum& pReduceSum) { InterpreterBase::visit(pReduceSum); }
  virtual void visit(ReduceSumSquare& pReduceSumSquare) { InterpreterBase::visit(pReduceSumSquare); }
  virtual void visit(Relu& pRelu) { InterpreterBase::visit(pRelu); }
  virtual void visit(Reshape& pReshape) { InterpreterBase::visit(pReshape); }
  virtual void visit(Selu& pSelu) { InterpreterBase::visit(pSelu); }
  virtual void visit(Shape& pShape) { InterpreterBase::visit(pShape); }
  virtual void visit(Sigmoid& pSigmoid) { InterpreterBase::visit(pSigmoid); }
  virtual void visit(Sin& pSin) { InterpreterBase::visit(pSin); }
  virtual void visit(Size& pSize) { InterpreterBase::visit(pSize); }
  virtual void visit(Slice& pSlice) { InterpreterBase::visit(pSlice); }
  virtual void visit(Softmax& pSoftmax) { InterpreterBase::visit(pSoftmax); }
  virtual void visit(Softplus& pSoftplus) { InterpreterBase::visit(pSoftplus); }
  virtual void visit(Softsign& pSoftsign) { InterpreterBase::visit(pSoftsign); }
  virtual void visit(SpaceToDepth& pSpaceToDepth) { InterpreterBase::visit(pSpaceToDepth); }
  virtual void visit(Split& pSplit) { InterpreterBase::visit(pSplit); }
  virtual void visit(Sqrt& pSqrt) { InterpreterBase::visit(pSqrt); }
  virtual void visit(Squeeze& pSqueeze) { InterpreterBase::visit(pSqueeze); }
  virtual void visit(Sub& pSub) { InterpreterBase::visit(pSub); }
  virtual void visit(Sum& pSum) { InterpreterBase::visit(pSum); }
  virtual void visit(Tan& pTan) { InterpreterBase::visit(pTan); }
  virtual void visit(Tanh& pTanh) { InterpreterBase::visit(pTanh); }
  virtual void visit(Tile& pTile) { InterpreterBase::visit(pTile); }
  virtual void visit(TopK& pTopK) { InterpreterBase::visit(pTopK); }
  virtual void visit(Transpose& pTranspose) { InterpreterBase::visit(pTranspose); }
  virtual void visit(Unsqueeze& pUnsqueeze) { InterpreterBase::visit(pUnsqueeze); }
  virtual void visit(Upsample& pUpsample) { InterpreterBase::visit(pUpsample); }
  virtual void visit(Xor& pXor) { InterpreterBase::visit(pXor); }
  virtual void visit(ATen& pATen) { InterpreterBase::visit(pATen); }
  virtual void visit(Affine& pAffine) { InterpreterBase::visit(pAffine); }
  virtual void visit(ConstantFill& pConstantFill) { InterpreterBase::visit(pConstantFill); }
  virtual void visit(Crop& pCrop) { InterpreterBase::visit(pCrop); }
  virtual void visit(GRUUnit& pGRUUnit) { InterpreterBase::visit(pGRUUnit); }
  virtual void visit(GivenTensorFill& pGivenTensorFill) { InterpreterBase::visit(pGivenTensorFill); }
  virtual void visit(ImageScaler& pImageScaler) { InterpreterBase::visit(pImageScaler); }
  virtual void visit(MeanVarianceNormalization& pMeanVarianceNormalization) { InterpreterBase::visit(pMeanVarianceNormalization); }
  virtual void visit(ParametricSoftplus& pParametricSoftplus) { InterpreterBase::visit(pParametricSoftplus); }
  virtual void visit(Scale& pScale) { InterpreterBase::visit(pScale); }
  virtual void visit(ScaledTanh& pScaledTanh) { InterpreterBase::visit(pScaledTanh); }
  virtual void visit(ThresholdedRelu& pThresholdedRelu) { InterpreterBase::visit(pThresholdedRelu); }
};

} // namespace of onnc

#endif
