//===- CodeEmitVisitor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_VANILLA_CODE_EMIT_VISITOR_H
#define TARGET_VANILLA_CODE_EMIT_VISITOR_H
#include <onnc/IR/ComputeVisitor.h>

namespace onnc {

namespace vanilla {

class CodeEmitVisitor : public ComputeVisitor
{
public:
  static char ID;
#if 0 //FIXME: should remove all the #if 0
  /// ONNC defined operators @{
  void visit(const Initializer& pInitializer) override;
  void visit(const InputOperator& pInputOperator) override;
  void visit(const OutputOperator& pOutputOperator) override;
#endif
  /// @}
#if 0
  /// ONNX defined operators @{
  void visit(const Abs& pAbs) override;
  void visit(const Acos& pAcos) override;
  void visit(const Add& pAdd) override;
  void visit(const And& pAnd) override;
  void visit(const ArgMax& pArgMax) override;
  void visit(const ArgMin& pArgMin) override;
  void visit(const Asin& pAsin) override;
  void visit(const Atan& pAtan) override;
  void visit(const AveragePool& pAveragePool) override;
  void visit(const BatchNormalization& pBatchNormalization) override;
  void visit(const Cast& pCast) override;
  void visit(const Ceil& pCeil) override;
  void visit(const Clip& pClip) override;
  void visit(const Concat& pConcat) override;
  void visit(const Constant& pConstant) override;
#endif
  void visit(const Conv& pConv) override;
#if 0
  void visit(const ConvTranspose& pConvTranspose) override;
  void visit(const Cos& pCos) override;
  void visit(const DepthToSpace& pDepthToSpace) override;
  void visit(const Div& pDiv) override;
  void visit(const Dropout& pDropout) override;
  void visit(const Elu& pElu) override;
  void visit(const Equal& pEqual) override;
  void visit(const Exp& pExp) override;
  void visit(const Expand& pExpand) override;
  void visit(const Flatten& pFlatten) override;
  void visit(const Floor& pFloor) override;
  void visit(const GRU& pGRU) override;
  void visit(const Gather& pGather) override;
  void visit(const Gemm& pGemm) override;
  void visit(const GlobalAveragePool& pGlobalAveragePool) override;
  void visit(const GlobalLpPool& pGlobalLpPool) override;
  void visit(const GlobalMaxPool& pGlobalMaxPool) override;
  void visit(const Greater& pGreater) override;
  void visit(const HardSigmoid& pHardSigmoid) override;
  void visit(const Hardmax& pHardmax) override;
  void visit(const Identity& pIdentity) override;
  void visit(const If& pIf) override;
  void visit(const InstanceNormalization& pInstanceNormalization) override;
  void visit(const LRN& pLRN) override;
  void visit(const LSTM& pLSTM) override;
  void visit(const LeakyRelu& pLeakyRelu) override;
  void visit(const Less& pLess) override;
  void visit(const Log& pLog) override;
  void visit(const LogSoftmax& pLogSoftmax) override;
  void visit(const Loop& pLoop) override;
  void visit(const LpNormalization& pLpNormalization) override;
  void visit(const LpPool& pLpPool) override;
  void visit(const MatMul& pMatMul) override;
  void visit(const Max& pMax) override;
  void visit(const MaxPool& pMaxPool) override;
  void visit(const MaxRoiPool& pMaxRoiPool) override;
  void visit(const Mean& pMean) override;
  void visit(const Min& pMin) override;
  void visit(const Mul& pMul) override;
  void visit(const Multinomial& pMultinomial) override;
  void visit(const Neg& pNeg) override;
  void visit(const Not& pNot) override;
  void visit(const Or& pOr) override;
  void visit(const PRelu& pPRelu) override;
  void visit(const Pad& pPad) override;
  void visit(const Pow& pPow) override;
  void visit(const RNN& pRNN) override;
  void visit(const RandomNormal& pRandomNormal) override;
  void visit(const RandomNormalLike& pRandomNormalLike) override;
  void visit(const RandomUniform& pRandomUniform) override;
  void visit(const RandomUniformLike& pRandomUniformLike) override;
  void visit(const Reciprocal& pReciprocal) override;
  void visit(const ReduceL1& pReduceL1) override;
  void visit(const ReduceL2& pReduceL2) override;
  void visit(const ReduceLogSum& pReduceLogSum) override;
  void visit(const ReduceLogSumExp& pReduceLogSumExp) override;
  void visit(const ReduceMax& pReduceMax) override;
  void visit(const ReduceMean& pReduceMean) override;
  void visit(const ReduceMin& pReduceMin) override;
  void visit(const ReduceProd& pReduceProd) override;
  void visit(const ReduceSum& pReduceSum) override;
  void visit(const ReduceSumSquare& pReduceSumSquare) override;
  void visit(const Relu& pRelu) override;
  void visit(const Reshape& pReshape) override;
  void visit(const Scan& pScan) override;
  void visit(const Selu& pSelu) override;
  void visit(const Shape& pShape) override;
  void visit(const Sigmoid& pSigmoid) override;
  void visit(const Sin& pSin) override;
  void visit(const Size& pSize) override;
  void visit(const Slice& pSlice) override;
  void visit(const Softmax& pSoftmax) override;
  void visit(const Softplus& pSoftplus) override;
  void visit(const Softsign& pSoftsign) override;
  void visit(const SpaceToDepth& pSpaceToDepth) override;
  void visit(const Split& pSplit) override;
  void visit(const Sqrt& pSqrt) override;
  void visit(const Squeeze& pSqueeze) override;
  void visit(const Sub& pSub) override;
  void visit(const Sum& pSum) override;
  void visit(const Tan& pTan) override;
  void visit(const Tanh& pTanh) override;
  void visit(const Tile& pTile) override;
  void visit(const TopK& pTopK) override;
  void visit(const Transpose& pTranspose) override;
  void visit(const Unsqueeze& pUnsqueeze) override;
  void visit(const Upsample& pUpsample) override;
  void visit(const Xor& pXor) override;
  void visit(const ATen& pATen) override;
  void visit(const Affine& pAffine) override;
  void visit(const ConstantFill& pConstantFill) override;
  void visit(const Crop& pCrop) override;
  void visit(const GRUUnit& pGRUUnit) override;
  void visit(const GivenTensorFill& pGivenTensorFill) override;
  void visit(const ImageScaler& pImageScaler) override;
  void visit(const MeanVarianceNormalization& pMeanVarianceNormalization) override;
  void visit(const ParametricSoftplus& pParametricSoftplus) override;
  void visit(const Scale& pScale) override;
  void visit(const ScaledTanh& pScaledTanh) override;
  void visit(const ThresholdedRelu& pThresholdedRelu) override;
  /// @}
#endif
#if 0
  /// ONNC defined operators @{
  void visit(Initializer& pInitializer) override;
  void visit(InputOperator& pInputOperator) override;
  void visit(OutputOperator& pOutputOperator) override;
#endif
#if 0
  /// @}
  /// ONNX defined operators @{
  void visit(Abs& pAbs) override;
  void visit(Acos& pAcos) override;
  void visit(Add& pAdd) override;
  void visit(And& pAnd) override;
  void visit(ArgMax& pArgMax) override;
  void visit(ArgMin& pArgMin) override;
  void visit(Asin& pAsin) override;
  void visit(Atan& pAtan) override;
  void visit(AveragePool& pAveragePool) override;
  void visit(BatchNormalization& pBatchNormalization) override;
  void visit(Cast& pCast) override;
  void visit(Ceil& pCeil) override;
  void visit(Clip& pClip) override;
  void visit(Concat& pConcat) override;
  void visit(Constant& pConstant) override;
#endif
  void visit(Conv& pConv) override;
#if 0
  void visit(ConvTranspose& pConvTranspose) override;
  void visit(Cos& pCos) override;
  void visit(DepthToSpace& pDepthToSpace) override;
  void visit(Div& pDiv) override;
  void visit(Dropout& pDropout) override;
  void visit(Elu& pElu) override;
  void visit(Equal& pEqual) override;
  void visit(Exp& pExp) override;
  void visit(Expand& pExpand) override;
  void visit(Flatten& pFlatten) override;
  void visit(Floor& pFloor) override;
  void visit(GRU& pGRU) override;
  void visit(Gather& pGather) override;
  void visit(Gemm& pGemm) override;
  void visit(GlobalAveragePool& pGlobalAveragePool) override;
  void visit(GlobalLpPool& pGlobalLpPool) override;
  void visit(GlobalMaxPool& pGlobalMaxPool) override;
  void visit(Greater& pGreater) override;
  void visit(HardSigmoid& pHardSigmoid) override;
  void visit(Hardmax& pHardmax) override;
  void visit(Identity& pIdentity) override;
  void visit(If& pIf) override;
  void visit(InstanceNormalization& pInstanceNormalization) override;
  void visit(LRN& pLRN) override;
  void visit(LSTM& pLSTM) override;
  void visit(LeakyRelu& pLeakyRelu) override;
  void visit(Less& pLess) override;
  void visit(Log& pLog) override;
  void visit(LogSoftmax& pLogSoftmax) override;
  void visit(Loop& pLoop) override;
  void visit(LpNormalization& pLpNormalization) override;
  void visit(LpPool& pLpPool) override;
  void visit(MatMul& pMatMul) override;
  void visit(Max& pMax) override;
  void visit(MaxPool& pMaxPool) override;
  void visit(MaxRoiPool& pMaxRoiPool) override;
  void visit(Mean& pMean) override;
  void visit(Min& pMin) override;
  void visit(Mul& pMul) override;
  void visit(Multinomial& pMultinomial) override;
  void visit(Neg& pNeg) override;
  void visit(Not& pNot) override;
  void visit(Or& pOr) override;
  void visit(PRelu& pPRelu) override;
  void visit(Pad& pPad) override;
  void visit(Pow& pPow) override;
  void visit(RNN& pRNN) override;
  void visit(RandomNormal& pRandomNormal) override;
  void visit(RandomNormalLike& pRandomNormalLike) override;
  void visit(RandomUniform& pRandomUniform) override;
  void visit(RandomUniformLike& pRandomUniformLike) override;
  void visit(Reciprocal& pReciprocal) override;
  void visit(ReduceL1& pReduceL1) override;
  void visit(ReduceL2& pReduceL2) override;
  void visit(ReduceLogSum& pReduceLogSum) override;
  void visit(ReduceLogSumExp& pReduceLogSumExp) override;
  void visit(ReduceMax& pReduceMax) override;
  void visit(ReduceMean& pReduceMean) override;
  void visit(ReduceMin& pReduceMin) override;
  void visit(ReduceProd& pReduceProd) override;
  void visit(ReduceSum& pReduceSum) override;
  void visit(ReduceSumSquare& pReduceSumSquare) override;
  void visit(Relu& pRelu) override;
  void visit(Reshape& pReshape) override;
  void visit(Scan& pScan) override;
  void visit(Selu& pSelu) override;
  void visit(Shape& pShape) override;
  void visit(Sigmoid& pSigmoid) override;
  void visit(Sin& pSin) override;
  void visit(Size& pSize) override;
  void visit(Slice& pSlice) override;
  void visit(Softmax& pSoftmax) override;
  void visit(Softplus& pSoftplus) override;
  void visit(Softsign& pSoftsign) override;
  void visit(SpaceToDepth& pSpaceToDepth) override;
  void visit(Split& pSplit) override;
  void visit(Sqrt& pSqrt) override;
  void visit(Squeeze& pSqueeze) override;
  void visit(Sub& pSub) override;
  void visit(Sum& pSum) override;
  void visit(Tan& pTan) override;
  void visit(Tanh& pTanh) override;
  void visit(Tile& pTile) override;
  void visit(TopK& pTopK) override;
  void visit(Transpose& pTranspose) override;
  void visit(Unsqueeze& pUnsqueeze) override;
  void visit(Upsample& pUpsample) override;
  void visit(Xor& pXor) override;
  void visit(ATen& pATen) override;
  void visit(Affine& pAffine) override;
  void visit(ConstantFill& pConstantFill) override;
  void visit(Crop& pCrop) override;
  void visit(GRUUnit& pGRUUnit) override;
  void visit(GivenTensorFill& pGivenTensorFill) override;
  void visit(ImageScaler& pImageScaler) override;
  void visit(MeanVarianceNormalization& pMeanVarianceNormalization) override;
  void visit(ParametricSoftplus& pParametricSoftplus) override;
  void visit(Scale& pScale) override;
  void visit(ScaledTanh& pScaledTanh) override;
  void visit(ThresholdedRelu& pThresholdedRelu) override;
  /// @}
#endif
  
};
  
} // namespace vanilla
} // namespace onnc

#endif
