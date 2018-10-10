//===- Interpreter.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_INTERPRETER_INTERPRETER_H
#define ONNC_INTERPRETER_INTERPRETER_H
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Value.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <unordered_map>
#include <cstddef>

namespace onnc {
/** \class Interpreter
 *  \brief Interpreter dispatch compute ir to runtime.
 */
class Interpreter : public ComputeVisitor
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

} // namespace of onnc

#endif
