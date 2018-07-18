//===- ComputeVisitor.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_VISITOR_H
#define ONNC_IR_COMPUTE_VISITOR_H

namespace onnc {

/// ONNC defined operators
class Initializer;
class InputOperator;
class OutputOperator;

/// ONNX defined operators
class Abs;
class Acos;
class Add;
class And;
class ArgMax;
class ArgMin;
class Asin;
class Atan;
class AveragePool;
class BatchNormalization;
class Cast;
class Ceil;
class Clip;
class Concat;
class Constant;
class Conv;
class ConvTranspose;
class Cos;
class DepthToSpace;
class Div;
class Dropout;
class Elu;
class Equal;
class Exp;
class Flatten;
class Floor;
class GRU;
class Gather;
class Gemm;
class GlobalAveragePool;
class GlobalLpPool;
class GlobalMaxPool;
class Greater;
class HardSigmoid;
class Hardmax;
class Identity;
class InstanceNormalization;
class LRN;
class LSTM;
class LeakyRelu;
class Less;
class Log;
class LogSoftmax;
class LpNormalization;
class LpPool;
class MatMul;
class Max;
class MaxPool;
class MaxRoiPool;
class Mean;
class Min;
class Mul;
class Neg;
class Not;
class Or;
class PRelu;
class Pad;
class Pow;
class RNN;
class RandomNormal;
class RandomNormalLike;
class RandomUniform;
class RandomUniformLike;
class Reciprocal;
class ReduceL1;
class ReduceL2;
class ReduceLogSum;
class ReduceLogSumExp;
class ReduceMax;
class ReduceMean;
class ReduceMin;
class ReduceProd;
class ReduceSum;
class ReduceSumSquare;
class Relu;
class Reshape;
class Selu;
class Shape;
class Sigmoid;
class Sin;
class Size;
class Slice;
class Softmax;
class Softplus;
class Softsign;
class SpaceToDepth;
class Split;
class Sqrt;
class Squeeze;
class Sub;
class Sum;
class Tan;
class Tanh;
class Tile;
class TopK;
class Transpose;
class Unsqueeze;
class Upsample;
class Xor;
class ATen;
class Affine;
class ConstantFill;
class Crop;
class FC;
class GRUUnit;
class GivenTensorFill;
class If;
class ImageScaler;
class Loop;
class LoopIndexTensor;
class MeanVarianceNormalization;
class ParametricSoftplus;
class Scale;
class ScaledTanh;
class ThresholdedRelu;

/** \class ComputeVisitor
 *  \brief ComptueVisitor provides interface to visit operators.
 */
class ComputeVisitor
{
public:
  /// ONNC defined operators @{
  virtual void visit(const Initializer& pInitializer) { }
  virtual void visit(const InputOperator& pInputOperator) { }
  virtual void visit(const OutputOperator& pOutputOperator) { }

  /// @}
  /// ONNX defined operators @{
  virtual void visit(const Abs& pAbs) { }
  virtual void visit(const Acos& pAcos) { }
  virtual void visit(const Add& pAdd) { }
  virtual void visit(const And& pAnd) { }
  virtual void visit(const ArgMax& pArgMax) { }
  virtual void visit(const ArgMin& pArgMin) { }
  virtual void visit(const Asin& pAsin) { }
  virtual void visit(const Atan& pAtan) { }
  virtual void visit(const AveragePool& pAveragePool) { }
  virtual void visit(const BatchNormalization& pBatchNormalization) { }
  virtual void visit(const Cast& pCast) { }
  virtual void visit(const Ceil& pCeil) { }
  virtual void visit(const Clip& pClip) { }
  virtual void visit(const Concat& pConcat) { }
  virtual void visit(const Constant& pConstant) { }
  virtual void visit(const Conv& pConv) { }
  virtual void visit(const ConvTranspose& pConvTranspose) { }
  virtual void visit(const Cos& pCos) { }
  virtual void visit(const DepthToSpace& pDepthToSpace) { }
  virtual void visit(const Div& pDiv) { }
  virtual void visit(const Dropout& pDropout) { }
  virtual void visit(const Elu& pElu) { }
  virtual void visit(const Equal& pEqual) { }
  virtual void visit(const Exp& pExp) { }
  virtual void visit(const Flatten& pFlatten) { }
  virtual void visit(const Floor& pFloor) { }
  virtual void visit(const GRU& pGRU) { }
  virtual void visit(const Gather& pGather) { }
  virtual void visit(const Gemm& pGemm) { }
  virtual void visit(const GlobalAveragePool& pGlobalAveragePool) { }
  virtual void visit(const GlobalLpPool& pGlobalLpPool) { }
  virtual void visit(const GlobalMaxPool& pGlobalMaxPool) { }
  virtual void visit(const Greater& pGreater) { }
  virtual void visit(const HardSigmoid& pHardSigmoid) { }
  virtual void visit(const Hardmax& pHardmax) { }
  virtual void visit(const Identity& pIdentity) { }
  virtual void visit(const InstanceNormalization& pInstanceNormalization) { }
  virtual void visit(const LRN& pLRN) { }
  virtual void visit(const LSTM& pLSTM) { }
  virtual void visit(const LeakyRelu& pLeakyRelu) { }
  virtual void visit(const Less& pLess) { }
  virtual void visit(const Log& pLog) { }
  virtual void visit(const LogSoftmax& pLogSoftmax) { }
  virtual void visit(const LpNormalization& pLpNormalization) { }
  virtual void visit(const LpPool& pLpPool) { }
  virtual void visit(const MatMul& pMatMul) { }
  virtual void visit(const Max& pMax) { }
  virtual void visit(const MaxPool& pMaxPool) { }
  virtual void visit(const MaxRoiPool& pMaxRoiPool) { }
  virtual void visit(const Mean& pMean) { }
  virtual void visit(const Min& pMin) { }
  virtual void visit(const Mul& pMul) { }
  virtual void visit(const Neg& pNeg) { }
  virtual void visit(const Not& pNot) { }
  virtual void visit(const Or& pOr) { }
  virtual void visit(const PRelu& pPRelu) { }
  virtual void visit(const Pad& pPad) { }
  virtual void visit(const Pow& pPow) { }
  virtual void visit(const RNN& pRNN) { }
  virtual void visit(const RandomNormal& pRandomNormal) { }
  virtual void visit(const RandomNormalLike& pRandomNormalLike) { }
  virtual void visit(const RandomUniform& pRandomUniform) { }
  virtual void visit(const RandomUniformLike& pRandomUniformLike) { }
  virtual void visit(const Reciprocal& pReciprocal) { }
  virtual void visit(const ReduceL1& pReduceL1) { }
  virtual void visit(const ReduceL2& pReduceL2) { }
  virtual void visit(const ReduceLogSum& pReduceLogSum) { }
  virtual void visit(const ReduceLogSumExp& pReduceLogSumExp) { }
  virtual void visit(const ReduceMax& pReduceMax) { }
  virtual void visit(const ReduceMean& pReduceMean) { }
  virtual void visit(const ReduceMin& pReduceMin) { }
  virtual void visit(const ReduceProd& pReduceProd) { }
  virtual void visit(const ReduceSum& pReduceSum) { }
  virtual void visit(const ReduceSumSquare& pReduceSumSquare) { }
  virtual void visit(const Relu& pRelu) { }
  virtual void visit(const Reshape& pReshape) { }
  virtual void visit(const Selu& pSelu) { }
  virtual void visit(const Shape& pShape) { }
  virtual void visit(const Sigmoid& pSigmoid) { }
  virtual void visit(const Sin& pSin) { }
  virtual void visit(const Size& pSize) { }
  virtual void visit(const Slice& pSlice) { }
  virtual void visit(const Softmax& pSoftmax) { }
  virtual void visit(const Softplus& pSoftplus) { }
  virtual void visit(const Softsign& pSoftsign) { }
  virtual void visit(const SpaceToDepth& pSpaceToDepth) { }
  virtual void visit(const Split& pSplit) { }
  virtual void visit(const Sqrt& pSqrt) { }
  virtual void visit(const Squeeze& pSqueeze) { }
  virtual void visit(const Sub& pSub) { }
  virtual void visit(const Sum& pSum) { }
  virtual void visit(const Tan& pTan) { }
  virtual void visit(const Tanh& pTanh) { }
  virtual void visit(const Tile& pTile) { }
  virtual void visit(const TopK& pTopK) { }
  virtual void visit(const Transpose& pTranspose) { }
  virtual void visit(const Unsqueeze& pUnsqueeze) { }
  virtual void visit(const Upsample& pUpsample) { }
  virtual void visit(const Xor& pXor) { }
  virtual void visit(const ATen& pATen) { }
  virtual void visit(const Affine& pAffine) { }
  virtual void visit(const ConstantFill& pConstantFill) { }
  virtual void visit(const Crop& pCrop) { }
  virtual void visit(const FC& pFC) { }
  virtual void visit(const GRUUnit& pGRUUnit) { }
  virtual void visit(const GivenTensorFill& pGivenTensorFill) { }
  virtual void visit(const If& pIf) { }
  virtual void visit(const ImageScaler& pImageScaler) { }
  virtual void visit(const Loop& pLoop) { }
  virtual void visit(const LoopIndexTensor& pLoopIndexTensor) { }
  virtual void visit(const MeanVarianceNormalization& pMeanVarianceNormalization) { }
  virtual void visit(const ParametricSoftplus& pParametricSoftplus) { }
  virtual void visit(const Scale& pScale) { }
  virtual void visit(const ScaledTanh& pScaledTanh) { }
  virtual void visit(const ThresholdedRelu& pThresholdedRelu) { }
  /// @}

  /// ONNC defined operators @{
  virtual void visit(Initializer& pInitializer) { }
  virtual void visit(InputOperator& pInputOperator) { }
  virtual void visit(OutputOperator& pOutputOperator) { }

  /// @}
  /// ONNX defined operators @{
  virtual void visit(Abs& pAbs) { }
  virtual void visit(Acos& pAcos) { }
  virtual void visit(Add& pAdd) { }
  virtual void visit(And& pAnd) { }
  virtual void visit(ArgMax& pArgMax) { }
  virtual void visit(ArgMin& pArgMin) { }
  virtual void visit(Asin& pAsin) { }
  virtual void visit(Atan& pAtan) { }
  virtual void visit(AveragePool& pAveragePool) { }
  virtual void visit(BatchNormalization& pBatchNormalization) { }
  virtual void visit(Cast& pCast) { }
  virtual void visit(Ceil& pCeil) { }
  virtual void visit(Clip& pClip) { }
  virtual void visit(Concat& pConcat) { }
  virtual void visit(Constant& pConstant) { }
  virtual void visit(Conv& pConv) { }
  virtual void visit(ConvTranspose& pConvTranspose) { }
  virtual void visit(Cos& pCos) { }
  virtual void visit(DepthToSpace& pDepthToSpace) { }
  virtual void visit(Div& pDiv) { }
  virtual void visit(Dropout& pDropout) { }
  virtual void visit(Elu& pElu) { }
  virtual void visit(Equal& pEqual) { }
  virtual void visit(Exp& pExp) { }
  virtual void visit(Flatten& pFlatten) { }
  virtual void visit(Floor& pFloor) { }
  virtual void visit(GRU& pGRU) { }
  virtual void visit(Gather& pGather) { }
  virtual void visit(Gemm& pGemm) { }
  virtual void visit(GlobalAveragePool& pGlobalAveragePool) { }
  virtual void visit(GlobalLpPool& pGlobalLpPool) { }
  virtual void visit(GlobalMaxPool& pGlobalMaxPool) { }
  virtual void visit(Greater& pGreater) { }
  virtual void visit(HardSigmoid& pHardSigmoid) { }
  virtual void visit(Hardmax& pHardmax) { }
  virtual void visit(Identity& pIdentity) { }
  virtual void visit(InstanceNormalization& pInstanceNormalization) { }
  virtual void visit(LRN& pLRN) { }
  virtual void visit(LSTM& pLSTM) { }
  virtual void visit(LeakyRelu& pLeakyRelu) { }
  virtual void visit(Less& pLess) { }
  virtual void visit(Log& pLog) { }
  virtual void visit(LogSoftmax& pLogSoftmax) { }
  virtual void visit(LpNormalization& pLpNormalization) { }
  virtual void visit(LpPool& pLpPool) { }
  virtual void visit(MatMul& pMatMul) { }
  virtual void visit(Max& pMax) { }
  virtual void visit(MaxPool& pMaxPool) { }
  virtual void visit(MaxRoiPool& pMaxRoiPool) { }
  virtual void visit(Mean& pMean) { }
  virtual void visit(Min& pMin) { }
  virtual void visit(Mul& pMul) { }
  virtual void visit(Neg& pNeg) { }
  virtual void visit(Not& pNot) { }
  virtual void visit(Or& pOr) { }
  virtual void visit(PRelu& pPRelu) { }
  virtual void visit(Pad& pPad) { }
  virtual void visit(Pow& pPow) { }
  virtual void visit(RNN& pRNN) { }
  virtual void visit(RandomNormal& pRandomNormal) { }
  virtual void visit(RandomNormalLike& pRandomNormalLike) { }
  virtual void visit(RandomUniform& pRandomUniform) { }
  virtual void visit(RandomUniformLike& pRandomUniformLike) { }
  virtual void visit(Reciprocal& pReciprocal) { }
  virtual void visit(ReduceL1& pReduceL1) { }
  virtual void visit(ReduceL2& pReduceL2) { }
  virtual void visit(ReduceLogSum& pReduceLogSum) { }
  virtual void visit(ReduceLogSumExp& pReduceLogSumExp) { }
  virtual void visit(ReduceMax& pReduceMax) { }
  virtual void visit(ReduceMean& pReduceMean) { }
  virtual void visit(ReduceMin& pReduceMin) { }
  virtual void visit(ReduceProd& pReduceProd) { }
  virtual void visit(ReduceSum& pReduceSum) { }
  virtual void visit(ReduceSumSquare& pReduceSumSquare) { }
  virtual void visit(Relu& pRelu) { }
  virtual void visit(Reshape& pReshape) { }
  virtual void visit(Selu& pSelu) { }
  virtual void visit(Shape& pShape) { }
  virtual void visit(Sigmoid& pSigmoid) { }
  virtual void visit(Sin& pSin) { }
  virtual void visit(Size& pSize) { }
  virtual void visit(Slice& pSlice) { }
  virtual void visit(Softmax& pSoftmax) { }
  virtual void visit(Softplus& pSoftplus) { }
  virtual void visit(Softsign& pSoftsign) { }
  virtual void visit(SpaceToDepth& pSpaceToDepth) { }
  virtual void visit(Split& pSplit) { }
  virtual void visit(Sqrt& pSqrt) { }
  virtual void visit(Squeeze& pSqueeze) { }
  virtual void visit(Sub& pSub) { }
  virtual void visit(Sum& pSum) { }
  virtual void visit(Tan& pTan) { }
  virtual void visit(Tanh& pTanh) { }
  virtual void visit(Tile& pTile) { }
  virtual void visit(TopK& pTopK) { }
  virtual void visit(Transpose& pTranspose) { }
  virtual void visit(Unsqueeze& pUnsqueeze) { }
  virtual void visit(Upsample& pUpsample) { }
  virtual void visit(Xor& pXor) { }
  virtual void visit(ATen& pATen) { }
  virtual void visit(Affine& pAffine) { }
  virtual void visit(ConstantFill& pConstantFill) { }
  virtual void visit(Crop& pCrop) { }
  virtual void visit(FC& pFC) { }
  virtual void visit(GRUUnit& pGRUUnit) { }
  virtual void visit(GivenTensorFill& pGivenTensorFill) { }
  virtual void visit(If& pIf) { }
  virtual void visit(ImageScaler& pImageScaler) { }
  virtual void visit(Loop& pLoop) { }
  virtual void visit(LoopIndexTensor& pLoopIndexTensor) { }
  virtual void visit(MeanVarianceNormalization& pMeanVarianceNormalization) { }
  virtual void visit(ParametricSoftplus& pParametricSoftplus) { }
  virtual void visit(Scale& pScale) { }
  virtual void visit(ScaledTanh& pScaledTanh) { }
  virtual void visit(ThresholdedRelu& pThresholdedRelu) { }
  /// @}
};

} // namespace of onnc

#endif
