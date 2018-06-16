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
};

} // namespace of onnc

#endif
