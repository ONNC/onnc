// Auto-generated
//===- ONNXNodeVisitor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/ONNXNodeVisitor.h>

#include <algorithm>
#include <mutex>

namespace onnc {

ONNXNodeVisitor::ONNXNodeVisitor() : m_JumpTable(getCachedJumpTable()) {}

// constexpr constructor, constant initialization, no runtime overhead.
static std::once_flag g_cached_jump_table;

const ONNXNodeVisitor::VisitorFnTable &ONNXNodeVisitor::getCachedJumpTable()
{
  static VisitorFnTable jump_table;
  std::call_once(g_cached_jump_table, []() {
    std::vector<std::pair<uint32_t, ONNXNodeVisitor::VisitorFn> > table = {
      // clang-format off
      { xSymbol("Undefined"), &ONNXNodeVisitor::visitUndefined },
      { xSymbol("Abs"), &ONNXNodeVisitor::visitAbs },
      { xSymbol("Acos"), &ONNXNodeVisitor::visitAcos },
      { xSymbol("Add"), &ONNXNodeVisitor::visitAdd },
      { xSymbol("And"), &ONNXNodeVisitor::visitAnd },
      { xSymbol("ArgMax"), &ONNXNodeVisitor::visitArgMax },
      { xSymbol("ArgMin"), &ONNXNodeVisitor::visitArgMin },
      { xSymbol("Asin"), &ONNXNodeVisitor::visitAsin },
      { xSymbol("Atan"), &ONNXNodeVisitor::visitAtan },
      { xSymbol("AveragePool"), &ONNXNodeVisitor::visitAveragePool },
      { xSymbol("BatchNormalization"), &ONNXNodeVisitor::visitBatchNormalization },
      { xSymbol("Cast"), &ONNXNodeVisitor::visitCast },
      { xSymbol("Ceil"), &ONNXNodeVisitor::visitCeil },
      { xSymbol("Clip"), &ONNXNodeVisitor::visitClip },
      { xSymbol("Concat"), &ONNXNodeVisitor::visitConcat },
      { xSymbol("Constant"), &ONNXNodeVisitor::visitConstant },
      { xSymbol("Conv"), &ONNXNodeVisitor::visitConv },
      { xSymbol("ConvTranspose"), &ONNXNodeVisitor::visitConvTranspose },
      { xSymbol("Cos"), &ONNXNodeVisitor::visitCos },
      { xSymbol("DepthToSpace"), &ONNXNodeVisitor::visitDepthToSpace },
      { xSymbol("Div"), &ONNXNodeVisitor::visitDiv },
      { xSymbol("Dropout"), &ONNXNodeVisitor::visitDropout },
      { xSymbol("Elu"), &ONNXNodeVisitor::visitElu },
      { xSymbol("Equal"), &ONNXNodeVisitor::visitEqual },
      { xSymbol("Exp"), &ONNXNodeVisitor::visitExp },
      { xSymbol("Flatten"), &ONNXNodeVisitor::visitFlatten },
      { xSymbol("Floor"), &ONNXNodeVisitor::visitFloor },
      { xSymbol("GRU"), &ONNXNodeVisitor::visitGRU },
      { xSymbol("Gather"), &ONNXNodeVisitor::visitGather },
      { xSymbol("Gemm"), &ONNXNodeVisitor::visitGemm },
      { xSymbol("GlobalAveragePool"), &ONNXNodeVisitor::visitGlobalAveragePool },
      { xSymbol("GlobalLpPool"), &ONNXNodeVisitor::visitGlobalLpPool },
      { xSymbol("GlobalMaxPool"), &ONNXNodeVisitor::visitGlobalMaxPool },
      { xSymbol("Greater"), &ONNXNodeVisitor::visitGreater },
      { xSymbol("HardSigmoid"), &ONNXNodeVisitor::visitHardSigmoid },
      { xSymbol("Hardmax"), &ONNXNodeVisitor::visitHardmax },
      { xSymbol("Identity"), &ONNXNodeVisitor::visitIdentity },
      { xSymbol("InstanceNormalization"), &ONNXNodeVisitor::visitInstanceNormalization },
      { xSymbol("LRN"), &ONNXNodeVisitor::visitLRN },
      { xSymbol("LSTM"), &ONNXNodeVisitor::visitLSTM },
      { xSymbol("LeakyRelu"), &ONNXNodeVisitor::visitLeakyRelu },
      { xSymbol("Less"), &ONNXNodeVisitor::visitLess },
      { xSymbol("Log"), &ONNXNodeVisitor::visitLog },
      { xSymbol("LogSoftmax"), &ONNXNodeVisitor::visitLogSoftmax },
      { xSymbol("LpNormalization"), &ONNXNodeVisitor::visitLpNormalization },
      { xSymbol("LpPool"), &ONNXNodeVisitor::visitLpPool },
      { xSymbol("MatMul"), &ONNXNodeVisitor::visitMatMul },
      { xSymbol("Max"), &ONNXNodeVisitor::visitMax },
      { xSymbol("MaxPool"), &ONNXNodeVisitor::visitMaxPool },
      { xSymbol("MaxRoiPool"), &ONNXNodeVisitor::visitMaxRoiPool },
      { xSymbol("Mean"), &ONNXNodeVisitor::visitMean },
      { xSymbol("Min"), &ONNXNodeVisitor::visitMin },
      { xSymbol("Mul"), &ONNXNodeVisitor::visitMul },
      { xSymbol("Neg"), &ONNXNodeVisitor::visitNeg },
      { xSymbol("Not"), &ONNXNodeVisitor::visitNot },
      { xSymbol("Or"), &ONNXNodeVisitor::visitOr },
      { xSymbol("PRelu"), &ONNXNodeVisitor::visitPRelu },
      { xSymbol("Pad"), &ONNXNodeVisitor::visitPad },
      { xSymbol("Pow"), &ONNXNodeVisitor::visitPow },
      { xSymbol("RNN"), &ONNXNodeVisitor::visitRNN },
      { xSymbol("RandomNormal"), &ONNXNodeVisitor::visitRandomNormal },
      { xSymbol("RandomNormalLike"), &ONNXNodeVisitor::visitRandomNormalLike },
      { xSymbol("RandomUniform"), &ONNXNodeVisitor::visitRandomUniform },
      { xSymbol("RandomUniformLike"), &ONNXNodeVisitor::visitRandomUniformLike },
      { xSymbol("Reciprocal"), &ONNXNodeVisitor::visitReciprocal },
      { xSymbol("ReduceL1"), &ONNXNodeVisitor::visitReduceL1 },
      { xSymbol("ReduceL2"), &ONNXNodeVisitor::visitReduceL2 },
      { xSymbol("ReduceLogSum"), &ONNXNodeVisitor::visitReduceLogSum },
      { xSymbol("ReduceLogSumExp"), &ONNXNodeVisitor::visitReduceLogSumExp },
      { xSymbol("ReduceMax"), &ONNXNodeVisitor::visitReduceMax },
      { xSymbol("ReduceMean"), &ONNXNodeVisitor::visitReduceMean },
      { xSymbol("ReduceMin"), &ONNXNodeVisitor::visitReduceMin },
      { xSymbol("ReduceProd"), &ONNXNodeVisitor::visitReduceProd },
      { xSymbol("ReduceSum"), &ONNXNodeVisitor::visitReduceSum },
      { xSymbol("ReduceSumSquare"), &ONNXNodeVisitor::visitReduceSumSquare },
      { xSymbol("Relu"), &ONNXNodeVisitor::visitRelu },
      { xSymbol("Reshape"), &ONNXNodeVisitor::visitReshape },
      { xSymbol("Selu"), &ONNXNodeVisitor::visitSelu },
      { xSymbol("Shape"), &ONNXNodeVisitor::visitShape },
      { xSymbol("Sigmoid"), &ONNXNodeVisitor::visitSigmoid },
      { xSymbol("Sin"), &ONNXNodeVisitor::visitSin },
      { xSymbol("Size"), &ONNXNodeVisitor::visitSize },
      { xSymbol("Slice"), &ONNXNodeVisitor::visitSlice },
      { xSymbol("Softmax"), &ONNXNodeVisitor::visitSoftmax },
      { xSymbol("Softplus"), &ONNXNodeVisitor::visitSoftplus },
      { xSymbol("Softsign"), &ONNXNodeVisitor::visitSoftsign },
      { xSymbol("SpaceToDepth"), &ONNXNodeVisitor::visitSpaceToDepth },
      { xSymbol("Split"), &ONNXNodeVisitor::visitSplit },
      { xSymbol("Sqrt"), &ONNXNodeVisitor::visitSqrt },
      { xSymbol("Squeeze"), &ONNXNodeVisitor::visitSqueeze },
      { xSymbol("Sub"), &ONNXNodeVisitor::visitSub },
      { xSymbol("Sum"), &ONNXNodeVisitor::visitSum },
      { xSymbol("Tan"), &ONNXNodeVisitor::visitTan },
      { xSymbol("Tanh"), &ONNXNodeVisitor::visitTanh },
      { xSymbol("Tile"), &ONNXNodeVisitor::visitTile },
      { xSymbol("TopK"), &ONNXNodeVisitor::visitTopK },
      { xSymbol("Transpose"), &ONNXNodeVisitor::visitTranspose },
      { xSymbol("Unsqueeze"), &ONNXNodeVisitor::visitUnsqueeze },
      { xSymbol("Upsample"), &ONNXNodeVisitor::visitUpsample },
      { xSymbol("Xor"), &ONNXNodeVisitor::visitXor },
      { xSymbol("ATen"), &ONNXNodeVisitor::visitATen },
      { xSymbol("Affine"), &ONNXNodeVisitor::visitAffine },
      { xSymbol("ConstantFill"), &ONNXNodeVisitor::visitConstantFill },
      { xSymbol("Crop"), &ONNXNodeVisitor::visitCrop },
      { xSymbol("FC"), &ONNXNodeVisitor::visitFC },
      { xSymbol("GRUUnit"), &ONNXNodeVisitor::visitGRUUnit },
      { xSymbol("GivenTensorFill"), &ONNXNodeVisitor::visitGivenTensorFill },
      { xSymbol("If"), &ONNXNodeVisitor::visitIf },
      { xSymbol("ImageScaler"), &ONNXNodeVisitor::visitImageScaler },
      { xSymbol("Loop"), &ONNXNodeVisitor::visitLoop },
      { xSymbol("LoopIndexTensor"), &ONNXNodeVisitor::visitLoopIndexTensor },
      { xSymbol("MeanVarianceNormalization"), &ONNXNodeVisitor::visitMeanVarianceNormalization },
      { xSymbol("ParametricSoftplus"), &ONNXNodeVisitor::visitParametricSoftplus },
      { xSymbol("Scale"), &ONNXNodeVisitor::visitScale },
      { xSymbol("ScaledTanh"), &ONNXNodeVisitor::visitScaledTanh },
      { xSymbol("ThresholdedRelu"), &ONNXNodeVisitor::visitThresholdedRelu }
      // clang-format off
    };
    uint32_t max = 0;
    for (const auto &idx_fn_pair : table) {
      max = std::max(max, idx_fn_pair.first);
    }
    jump_table.resize(max + 1);
    for (const auto &idx_fn_pair : table) {
      jump_table[idx_fn_pair.first] = idx_fn_pair.second;
    }
  });
  return jump_table;
}

} // namespace onnc
