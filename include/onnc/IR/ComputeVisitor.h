//===- ComputeVisitor.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_VISITOR_H
#define ONNC_IR_COMPUTE_VISITOR_H

#include <cstdint>

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
class Expand;
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
class If;
class InstanceNormalization;
class LRN;
class LSTM;
class LeakyRelu;
class Less;
class Log;
class LogSoftmax;
class Loop;
class LpNormalization;
class LpPool;
class MatMul;
class Max;
class MaxPool;
class MaxRoiPool;
class Mean;
class Min;
class Mul;
class Multinomial;
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
class Scan;
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
class GRUUnit;
class GivenTensorFill;
class ImageScaler;
class MeanVarianceNormalization;
class ParametricSoftplus;
class Scale;
class ScaledTanh;
class ThresholdedRelu;

/** \class ComputeVisitor
 *  \brief ComputeVisitor provides interface to visit operators.
 */
class ComputeVisitor
{
public:
  using VisitorTypeID = std::intptr_t;

public:
  ComputeVisitor() = default;

  virtual VisitorTypeID getVisitorID() const = 0;

  /// ONNC defined operators @{
  virtual void visit(const Initializer&) { }
  virtual void visit(const InputOperator&) { }
  virtual void visit(const OutputOperator&) { }

  /// @}
  /// ONNX defined operators @{
  virtual void visit(const Abs&) { }
  virtual void visit(const Acos&) { }
  virtual void visit(const Add&) { }
  virtual void visit(const And&) { }
  virtual void visit(const ArgMax&) { }
  virtual void visit(const ArgMin&) { }
  virtual void visit(const Asin&) { }
  virtual void visit(const Atan&) { }
  virtual void visit(const AveragePool&) { }
  virtual void visit(const BatchNormalization&) { }
  virtual void visit(const Cast&) { }
  virtual void visit(const Ceil&) { }
  virtual void visit(const Clip&) { }
  virtual void visit(const Concat&) { }
  virtual void visit(const Constant&) { }
  virtual void visit(const Conv&) { }
  virtual void visit(const ConvTranspose&) { }
  virtual void visit(const Cos&) { }
  virtual void visit(const DepthToSpace&) { }
  virtual void visit(const Div&) { }
  virtual void visit(const Dropout&) { }
  virtual void visit(const Elu&) { }
  virtual void visit(const Equal&) { }
  virtual void visit(const Exp&) { }
  virtual void visit(const Expand&) { }
  virtual void visit(const Flatten&) { }
  virtual void visit(const Floor&) { }
  virtual void visit(const GRU&) { }
  virtual void visit(const Gather&) { }
  virtual void visit(const Gemm&) { }
  virtual void visit(const GlobalAveragePool&) { }
  virtual void visit(const GlobalLpPool&) { }
  virtual void visit(const GlobalMaxPool&) { }
  virtual void visit(const Greater&) { }
  virtual void visit(const HardSigmoid&) { }
  virtual void visit(const Hardmax&) { }
  virtual void visit(const Identity&) { }
  virtual void visit(const If&) { }
  virtual void visit(const InstanceNormalization&) { }
  virtual void visit(const LRN&) { }
  virtual void visit(const LSTM&) { }
  virtual void visit(const LeakyRelu&) { }
  virtual void visit(const Less&) { }
  virtual void visit(const Log&) { }
  virtual void visit(const LogSoftmax&) { }
  virtual void visit(const Loop&) { }
  virtual void visit(const LpNormalization&) { }
  virtual void visit(const LpPool&) { }
  virtual void visit(const MatMul&) { }
  virtual void visit(const Max&) { }
  virtual void visit(const MaxPool&) { }
  virtual void visit(const MaxRoiPool&) { }
  virtual void visit(const Mean&) { }
  virtual void visit(const Min&) { }
  virtual void visit(const Mul&) { }
  virtual void visit(const Multinomial&) { }
  virtual void visit(const Neg&) { }
  virtual void visit(const Not&) { }
  virtual void visit(const Or&) { }
  virtual void visit(const PRelu&) { }
  virtual void visit(const Pad&) { }
  virtual void visit(const Pow&) { }
  virtual void visit(const RNN&) { }
  virtual void visit(const RandomNormal&) { }
  virtual void visit(const RandomNormalLike&) { }
  virtual void visit(const RandomUniform&) { }
  virtual void visit(const RandomUniformLike&) { }
  virtual void visit(const Reciprocal&) { }
  virtual void visit(const ReduceL1&) { }
  virtual void visit(const ReduceL2&) { }
  virtual void visit(const ReduceLogSum&) { }
  virtual void visit(const ReduceLogSumExp&) { }
  virtual void visit(const ReduceMax&) { }
  virtual void visit(const ReduceMean&) { }
  virtual void visit(const ReduceMin&) { }
  virtual void visit(const ReduceProd&) { }
  virtual void visit(const ReduceSum&) { }
  virtual void visit(const ReduceSumSquare&) { }
  virtual void visit(const Relu&) { }
  virtual void visit(const Reshape&) { }
  virtual void visit(const Scan&) { }
  virtual void visit(const Selu&) { }
  virtual void visit(const Shape&) { }
  virtual void visit(const Sigmoid&) { }
  virtual void visit(const Sin&) { }
  virtual void visit(const Size&) { }
  virtual void visit(const Slice&) { }
  virtual void visit(const Softmax&) { }
  virtual void visit(const Softplus&) { }
  virtual void visit(const Softsign&) { }
  virtual void visit(const SpaceToDepth&) { }
  virtual void visit(const Split&) { }
  virtual void visit(const Sqrt&) { }
  virtual void visit(const Squeeze&) { }
  virtual void visit(const Sub&) { }
  virtual void visit(const Sum&) { }
  virtual void visit(const Tan&) { }
  virtual void visit(const Tanh&) { }
  virtual void visit(const Tile&) { }
  virtual void visit(const TopK&) { }
  virtual void visit(const Transpose&) { }
  virtual void visit(const Unsqueeze&) { }
  virtual void visit(const Upsample&) { }
  virtual void visit(const Xor&) { }
  virtual void visit(const ATen&) { }
  virtual void visit(const Affine&) { }
  virtual void visit(const ConstantFill&) { }
  virtual void visit(const Crop&) { }
  virtual void visit(const GRUUnit&) { }
  virtual void visit(const GivenTensorFill&) { }
  virtual void visit(const ImageScaler&) { }
  virtual void visit(const MeanVarianceNormalization&) { }
  virtual void visit(const ParametricSoftplus&) { }
  virtual void visit(const Scale&) { }
  virtual void visit(const ScaledTanh&) { }
  virtual void visit(const ThresholdedRelu&) { }
  /// @}

  /// ONNC defined operators @{
  virtual void visit(Initializer& pInitializer) { visit(const_cast<const Initializer&>(pInitializer)); }
  virtual void visit(InputOperator& pInputOperator) { visit(const_cast<const InputOperator&>(pInputOperator)); }
  virtual void visit(OutputOperator& pOutputOperator) { visit(const_cast<const OutputOperator&>(pOutputOperator)); }

  /// @}
  /// ONNX defined operators @{
  virtual void visit(Abs& pAbs) { visit(const_cast<const Abs&>(pAbs)); }
  virtual void visit(Acos& pAcos) { visit(const_cast<const Acos&>(pAcos)); }
  virtual void visit(Add& pAdd) { visit(const_cast<const Add&>(pAdd)); }
  virtual void visit(And& pAnd) { visit(const_cast<const And&>(pAnd)); }
  virtual void visit(ArgMax& pArgMax) { visit(const_cast<const ArgMax&>(pArgMax)); }
  virtual void visit(ArgMin& pArgMin) { visit(const_cast<const ArgMin&>(pArgMin)); }
  virtual void visit(Asin& pAsin) { visit(const_cast<const Asin&>(pAsin)); }
  virtual void visit(Atan& pAtan) { visit(const_cast<const Atan&>(pAtan)); }
  virtual void visit(AveragePool& pAveragePool) { visit(const_cast<const AveragePool&>(pAveragePool)); }
  virtual void visit(BatchNormalization& pBatchNormalization) { visit(const_cast<const BatchNormalization&>(pBatchNormalization)); }
  virtual void visit(Cast& pCast) { visit(const_cast<const Cast&>(pCast)); }
  virtual void visit(Ceil& pCeil) { visit(const_cast<const Ceil&>(pCeil)); }
  virtual void visit(Clip& pClip) { visit(const_cast<const Clip&>(pClip)); }
  virtual void visit(Concat& pConcat) { visit(const_cast<const Concat&>(pConcat)); }
  virtual void visit(Constant& pConstant) { visit(const_cast<const Constant&>(pConstant)); }
  virtual void visit(Conv& pConv) { visit(const_cast<const Conv&>(pConv)); }
  virtual void visit(ConvTranspose& pConvTranspose) { visit(const_cast<const ConvTranspose&>(pConvTranspose)); }
  virtual void visit(Cos& pCos) { visit(const_cast<const Cos&>(pCos)); }
  virtual void visit(DepthToSpace& pDepthToSpace) { visit(const_cast<const DepthToSpace&>(pDepthToSpace)); }
  virtual void visit(Div& pDiv) { visit(const_cast<const Div&>(pDiv)); }
  virtual void visit(Dropout& pDropout) { visit(const_cast<const Dropout&>(pDropout)); }
  virtual void visit(Elu& pElu) { visit(const_cast<const Elu&>(pElu)); }
  virtual void visit(Equal& pEqual) { visit(const_cast<const Equal&>(pEqual)); }
  virtual void visit(Exp& pExp) { visit(const_cast<const Exp&>(pExp)); }
  virtual void visit(Expand& pExpand) { visit(const_cast<const Expand&>(pExpand)); }
  virtual void visit(Flatten& pFlatten) { visit(const_cast<const Flatten&>(pFlatten)); }
  virtual void visit(Floor& pFloor) { visit(const_cast<const Floor&>(pFloor)); }
  virtual void visit(GRU& pGRU) { visit(const_cast<const GRU&>(pGRU)); }
  virtual void visit(Gather& pGather) { visit(const_cast<const Gather&>(pGather)); }
  virtual void visit(Gemm& pGemm) { visit(const_cast<const Gemm&>(pGemm)); }
  virtual void visit(GlobalAveragePool& pGlobalAveragePool) { visit(const_cast<const GlobalAveragePool&>(pGlobalAveragePool)); }
  virtual void visit(GlobalLpPool& pGlobalLpPool) { visit(const_cast<const GlobalLpPool&>(pGlobalLpPool)); }
  virtual void visit(GlobalMaxPool& pGlobalMaxPool) { visit(const_cast<const GlobalMaxPool&>(pGlobalMaxPool)); }
  virtual void visit(Greater& pGreater) { visit(const_cast<const Greater&>(pGreater)); }
  virtual void visit(HardSigmoid& pHardSigmoid) { visit(const_cast<const HardSigmoid&>(pHardSigmoid)); }
  virtual void visit(Hardmax& pHardmax) { visit(const_cast<const Hardmax&>(pHardmax)); }
  virtual void visit(Identity& pIdentity) { visit(const_cast<const Identity&>(pIdentity)); }
  virtual void visit(If& pIf) { visit(const_cast<const If&>(pIf)); }
  virtual void visit(InstanceNormalization& pInstanceNormalization) { visit(const_cast<const InstanceNormalization&>(pInstanceNormalization)); }
  virtual void visit(LRN& pLRN) { visit(const_cast<const LRN&>(pLRN)); }
  virtual void visit(LSTM& pLSTM) { visit(const_cast<const LSTM&>(pLSTM)); }
  virtual void visit(LeakyRelu& pLeakyRelu) { visit(const_cast<const LeakyRelu&>(pLeakyRelu)); }
  virtual void visit(Less& pLess) { visit(const_cast<const Less&>(pLess)); }
  virtual void visit(Log& pLog) { visit(const_cast<const Log&>(pLog)); }
  virtual void visit(LogSoftmax& pLogSoftmax) { visit(const_cast<const LogSoftmax&>(pLogSoftmax)); }
  virtual void visit(Loop& pLoop) { visit(const_cast<const Loop&>(pLoop)); }
  virtual void visit(LpNormalization& pLpNormalization) { visit(const_cast<const LpNormalization&>(pLpNormalization)); }
  virtual void visit(LpPool& pLpPool) { visit(const_cast<const LpPool&>(pLpPool)); }
  virtual void visit(MatMul& pMatMul) { visit(const_cast<const MatMul&>(pMatMul)); }
  virtual void visit(Max& pMax) { visit(const_cast<const Max&>(pMax)); }
  virtual void visit(MaxPool& pMaxPool) { visit(const_cast<const MaxPool&>(pMaxPool)); }
  virtual void visit(MaxRoiPool& pMaxRoiPool) { visit(const_cast<const MaxRoiPool&>(pMaxRoiPool)); }
  virtual void visit(Mean& pMean) { visit(const_cast<const Mean&>(pMean)); }
  virtual void visit(Min& pMin) { visit(const_cast<const Min&>(pMin)); }
  virtual void visit(Mul& pMul) { visit(const_cast<const Mul&>(pMul)); }
  virtual void visit(Multinomial& pMultinomial) { visit(const_cast<const Multinomial&>(pMultinomial)); }
  virtual void visit(Neg& pNeg) { visit(const_cast<const Neg&>(pNeg)); }
  virtual void visit(Not& pNot) { visit(const_cast<const Not&>(pNot)); }
  virtual void visit(Or& pOr) { visit(const_cast<const Or&>(pOr)); }
  virtual void visit(PRelu& pPRelu) { visit(const_cast<const PRelu&>(pPRelu)); }
  virtual void visit(Pad& pPad) { visit(const_cast<const Pad&>(pPad)); }
  virtual void visit(Pow& pPow) { visit(const_cast<const Pow&>(pPow)); }
  virtual void visit(RNN& pRNN) { visit(const_cast<const RNN&>(pRNN)); }
  virtual void visit(RandomNormal& pRandomNormal) { visit(const_cast<const RandomNormal&>(pRandomNormal)); }
  virtual void visit(RandomNormalLike& pRandomNormalLike) { visit(const_cast<const RandomNormalLike&>(pRandomNormalLike)); }
  virtual void visit(RandomUniform& pRandomUniform) { visit(const_cast<const RandomUniform&>(pRandomUniform)); }
  virtual void visit(RandomUniformLike& pRandomUniformLike) { visit(const_cast<const RandomUniformLike&>(pRandomUniformLike)); }
  virtual void visit(Reciprocal& pReciprocal) { visit(const_cast<const Reciprocal&>(pReciprocal)); }
  virtual void visit(ReduceL1& pReduceL1) { visit(const_cast<const ReduceL1&>(pReduceL1)); }
  virtual void visit(ReduceL2& pReduceL2) { visit(const_cast<const ReduceL2&>(pReduceL2)); }
  virtual void visit(ReduceLogSum& pReduceLogSum) { visit(const_cast<const ReduceLogSum&>(pReduceLogSum)); }
  virtual void visit(ReduceLogSumExp& pReduceLogSumExp) { visit(const_cast<const ReduceLogSumExp&>(pReduceLogSumExp)); }
  virtual void visit(ReduceMax& pReduceMax) { visit(const_cast<const ReduceMax&>(pReduceMax)); }
  virtual void visit(ReduceMean& pReduceMean) { visit(const_cast<const ReduceMean&>(pReduceMean)); }
  virtual void visit(ReduceMin& pReduceMin) { visit(const_cast<const ReduceMin&>(pReduceMin)); }
  virtual void visit(ReduceProd& pReduceProd) { visit(const_cast<const ReduceProd&>(pReduceProd)); }
  virtual void visit(ReduceSum& pReduceSum) { visit(const_cast<const ReduceSum&>(pReduceSum)); }
  virtual void visit(ReduceSumSquare& pReduceSumSquare) { visit(const_cast<const ReduceSumSquare&>(pReduceSumSquare)); }
  virtual void visit(Relu& pRelu) { visit(const_cast<const Relu&>(pRelu)); }
  virtual void visit(Reshape& pReshape) { visit(const_cast<const Reshape&>(pReshape)); }
  virtual void visit(Scan& pScan) { visit(const_cast<const Scan&>(pScan)); }
  virtual void visit(Selu& pSelu) { visit(const_cast<const Selu&>(pSelu)); }
  virtual void visit(Shape& pShape) { visit(const_cast<const Shape&>(pShape)); }
  virtual void visit(Sigmoid& pSigmoid) { visit(const_cast<const Sigmoid&>(pSigmoid)); }
  virtual void visit(Sin& pSin) { visit(const_cast<const Sin&>(pSin)); }
  virtual void visit(Size& pSize) { visit(const_cast<const Size&>(pSize)); }
  virtual void visit(Slice& pSlice) { visit(const_cast<const Slice&>(pSlice)); }
  virtual void visit(Softmax& pSoftmax) { visit(const_cast<const Softmax&>(pSoftmax)); }
  virtual void visit(Softplus& pSoftplus) { visit(const_cast<const Softplus&>(pSoftplus)); }
  virtual void visit(Softsign& pSoftsign) { visit(const_cast<const Softsign&>(pSoftsign)); }
  virtual void visit(SpaceToDepth& pSpaceToDepth) { visit(const_cast<const SpaceToDepth&>(pSpaceToDepth)); }
  virtual void visit(Split& pSplit) { visit(const_cast<const Split&>(pSplit)); }
  virtual void visit(Sqrt& pSqrt) { visit(const_cast<const Sqrt&>(pSqrt)); }
  virtual void visit(Squeeze& pSqueeze) { visit(const_cast<const Squeeze&>(pSqueeze)); }
  virtual void visit(Sub& pSub) { visit(const_cast<const Sub&>(pSub)); }
  virtual void visit(Sum& pSum) { visit(const_cast<const Sum&>(pSum)); }
  virtual void visit(Tan& pTan) { visit(const_cast<const Tan&>(pTan)); }
  virtual void visit(Tanh& pTanh) { visit(const_cast<const Tanh&>(pTanh)); }
  virtual void visit(Tile& pTile) { visit(const_cast<const Tile&>(pTile)); }
  virtual void visit(TopK& pTopK) { visit(const_cast<const TopK&>(pTopK)); }
  virtual void visit(Transpose& pTranspose) { visit(const_cast<const Transpose&>(pTranspose)); }
  virtual void visit(Unsqueeze& pUnsqueeze) { visit(const_cast<const Unsqueeze&>(pUnsqueeze)); }
  virtual void visit(Upsample& pUpsample) { visit(const_cast<const Upsample&>(pUpsample)); }
  virtual void visit(Xor& pXor) { visit(const_cast<const Xor&>(pXor)); }
  virtual void visit(ATen& pATen) { visit(const_cast<const ATen&>(pATen)); }
  virtual void visit(Affine& pAffine) { visit(const_cast<const Affine&>(pAffine)); }
  virtual void visit(ConstantFill& pConstantFill) { visit(const_cast<const ConstantFill&>(pConstantFill)); }
  virtual void visit(Crop& pCrop) { visit(const_cast<const Crop&>(pCrop)); }
  virtual void visit(GRUUnit& pGRUUnit) { visit(const_cast<const GRUUnit&>(pGRUUnit)); }
  virtual void visit(GivenTensorFill& pGivenTensorFill) { visit(const_cast<const GivenTensorFill&>(pGivenTensorFill)); }
  virtual void visit(ImageScaler& pImageScaler) { visit(const_cast<const ImageScaler&>(pImageScaler)); }
  virtual void visit(MeanVarianceNormalization& pMeanVarianceNormalization) { visit(const_cast<const MeanVarianceNormalization&>(pMeanVarianceNormalization)); }
  virtual void visit(ParametricSoftplus& pParametricSoftplus) { visit(const_cast<const ParametricSoftplus&>(pParametricSoftplus)); }
  virtual void visit(Scale& pScale) { visit(const_cast<const Scale&>(pScale)); }
  virtual void visit(ScaledTanh& pScaledTanh) { visit(const_cast<const ScaledTanh&>(pScaledTanh)); }
  virtual void visit(ThresholdedRelu& pThresholdedRelu) { visit(const_cast<const ThresholdedRelu&>(pThresholdedRelu)); }
  /// @}
};

} // namespace of onnc

#endif
