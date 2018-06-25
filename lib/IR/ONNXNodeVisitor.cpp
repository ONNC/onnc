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

ONNXNodeVisitor::ONNXNodeVisitor()
    : m_JumpTable(getCachedJumpTable()) {}

// constexpr constructor, constant initialization, no runtime overhead.
static std::once_flag g_cached_jump_table;

const ONNXNodeVisitor::VisitorFnTable &ONNXNodeVisitor::getCachedJumpTable()
{
  static VisitorFnTable jump_table;
  std::call_once(g_cached_jump_table, [&jump_table](){
    std::vector<std::pair<uint32_t, ONNXNodeVisitor::VisitorFn>> table = {
      {::onnx::Symbol("Undefined"), &ONNXNodeVisitor::visitUndefined},
      {::onnx::Symbol("Abs"), &ONNXNodeVisitor::visitAbs},
      {::onnx::Symbol("Acos"), &ONNXNodeVisitor::visitAcos},
      {::onnx::Symbol("Add"), &ONNXNodeVisitor::visitAdd},
      {::onnx::Symbol("And"), &ONNXNodeVisitor::visitAnd},
      {::onnx::Symbol("ArgMax"), &ONNXNodeVisitor::visitArgMax},
      {::onnx::Symbol("ArgMin"), &ONNXNodeVisitor::visitArgMin},
      {::onnx::Symbol("Asin"), &ONNXNodeVisitor::visitAsin},
      {::onnx::Symbol("Atan"), &ONNXNodeVisitor::visitAtan},
      {::onnx::Symbol("AveragePool"), &ONNXNodeVisitor::visitAveragePool},
      {::onnx::Symbol("BatchNormalization"), &ONNXNodeVisitor::visitBatchNormalization},
      {::onnx::Symbol("Cast"), &ONNXNodeVisitor::visitCast},
      {::onnx::Symbol("Ceil"), &ONNXNodeVisitor::visitCeil},
      {::onnx::Symbol("Clip"), &ONNXNodeVisitor::visitClip},
      {::onnx::Symbol("Concat"), &ONNXNodeVisitor::visitConcat},
      {::onnx::Symbol("Constant"), &ONNXNodeVisitor::visitConstant},
      {::onnx::Symbol("Conv"), &ONNXNodeVisitor::visitConv},
      {::onnx::Symbol("ConvTranspose"), &ONNXNodeVisitor::visitConvTranspose},
      {::onnx::Symbol("Cos"), &ONNXNodeVisitor::visitCos},
      {::onnx::Symbol("DepthToSpace"), &ONNXNodeVisitor::visitDepthToSpace},
      {::onnx::Symbol("Div"), &ONNXNodeVisitor::visitDiv},
      {::onnx::Symbol("Dropout"), &ONNXNodeVisitor::visitDropout},
      {::onnx::Symbol("Elu"), &ONNXNodeVisitor::visitElu},
      {::onnx::Symbol("Equal"), &ONNXNodeVisitor::visitEqual},
      {::onnx::Symbol("Exp"), &ONNXNodeVisitor::visitExp},
      {::onnx::Symbol("Flatten"), &ONNXNodeVisitor::visitFlatten},
      {::onnx::Symbol("Floor"), &ONNXNodeVisitor::visitFloor},
      {::onnx::Symbol("GRU"), &ONNXNodeVisitor::visitGRU},
      {::onnx::Symbol("Gather"), &ONNXNodeVisitor::visitGather},
      {::onnx::Symbol("Gemm"), &ONNXNodeVisitor::visitGemm},
      {::onnx::Symbol("GlobalAveragePool"), &ONNXNodeVisitor::visitGlobalAveragePool},
      {::onnx::Symbol("GlobalLpPool"), &ONNXNodeVisitor::visitGlobalLpPool},
      {::onnx::Symbol("GlobalMaxPool"), &ONNXNodeVisitor::visitGlobalMaxPool},
      {::onnx::Symbol("Greater"), &ONNXNodeVisitor::visitGreater},
      {::onnx::Symbol("HardSigmoid"), &ONNXNodeVisitor::visitHardSigmoid},
      {::onnx::Symbol("Hardmax"), &ONNXNodeVisitor::visitHardmax},
      {::onnx::Symbol("Identity"), &ONNXNodeVisitor::visitIdentity},
      {::onnx::Symbol("InstanceNormalization"), &ONNXNodeVisitor::visitInstanceNormalization},
      {::onnx::Symbol("LRN"), &ONNXNodeVisitor::visitLRN},
      {::onnx::Symbol("LSTM"), &ONNXNodeVisitor::visitLSTM},
      {::onnx::Symbol("LeakyRelu"), &ONNXNodeVisitor::visitLeakyRelu},
      {::onnx::Symbol("Less"), &ONNXNodeVisitor::visitLess},
      {::onnx::Symbol("Log"), &ONNXNodeVisitor::visitLog},
      {::onnx::Symbol("LogSoftmax"), &ONNXNodeVisitor::visitLogSoftmax},
      {::onnx::Symbol("LpNormalization"), &ONNXNodeVisitor::visitLpNormalization},
      {::onnx::Symbol("LpPool"), &ONNXNodeVisitor::visitLpPool},
      {::onnx::Symbol("MatMul"), &ONNXNodeVisitor::visitMatMul},
      {::onnx::Symbol("Max"), &ONNXNodeVisitor::visitMax},
      {::onnx::Symbol("MaxPool"), &ONNXNodeVisitor::visitMaxPool},
      {::onnx::Symbol("MaxRoiPool"), &ONNXNodeVisitor::visitMaxRoiPool},
      {::onnx::Symbol("Mean"), &ONNXNodeVisitor::visitMean},
      {::onnx::Symbol("Min"), &ONNXNodeVisitor::visitMin},
      {::onnx::Symbol("Mul"), &ONNXNodeVisitor::visitMul},
      {::onnx::Symbol("Neg"), &ONNXNodeVisitor::visitNeg},
      {::onnx::Symbol("Not"), &ONNXNodeVisitor::visitNot},
      {::onnx::Symbol("Or"), &ONNXNodeVisitor::visitOr},
      {::onnx::Symbol("PRelu"), &ONNXNodeVisitor::visitPRelu},
      {::onnx::Symbol("Pad"), &ONNXNodeVisitor::visitPad},
      {::onnx::Symbol("Pow"), &ONNXNodeVisitor::visitPow},
      {::onnx::Symbol("RNN"), &ONNXNodeVisitor::visitRNN},
      {::onnx::Symbol("RandomNormal"), &ONNXNodeVisitor::visitRandomNormal},
      {::onnx::Symbol("RandomNormalLike"), &ONNXNodeVisitor::visitRandomNormalLike},
      {::onnx::Symbol("RandomUniform"), &ONNXNodeVisitor::visitRandomUniform},
      {::onnx::Symbol("RandomUniformLike"), &ONNXNodeVisitor::visitRandomUniformLike},
      {::onnx::Symbol("Reciprocal"), &ONNXNodeVisitor::visitReciprocal},
      {::onnx::Symbol("ReduceL1"), &ONNXNodeVisitor::visitReduceL1},
      {::onnx::Symbol("ReduceL2"), &ONNXNodeVisitor::visitReduceL2},
      {::onnx::Symbol("ReduceLogSum"), &ONNXNodeVisitor::visitReduceLogSum},
      {::onnx::Symbol("ReduceLogSumExp"), &ONNXNodeVisitor::visitReduceLogSumExp},
      {::onnx::Symbol("ReduceMax"), &ONNXNodeVisitor::visitReduceMax},
      {::onnx::Symbol("ReduceMean"), &ONNXNodeVisitor::visitReduceMean},
      {::onnx::Symbol("ReduceMin"), &ONNXNodeVisitor::visitReduceMin},
      {::onnx::Symbol("ReduceProd"), &ONNXNodeVisitor::visitReduceProd},
      {::onnx::Symbol("ReduceSum"), &ONNXNodeVisitor::visitReduceSum},
      {::onnx::Symbol("ReduceSumSquare"), &ONNXNodeVisitor::visitReduceSumSquare},
      {::onnx::Symbol("Relu"), &ONNXNodeVisitor::visitRelu},
      {::onnx::Symbol("Reshape"), &ONNXNodeVisitor::visitReshape},
      {::onnx::Symbol("Selu"), &ONNXNodeVisitor::visitSelu},
      {::onnx::Symbol("Shape"), &ONNXNodeVisitor::visitShape},
      {::onnx::Symbol("Sigmoid"), &ONNXNodeVisitor::visitSigmoid},
      {::onnx::Symbol("Sin"), &ONNXNodeVisitor::visitSin},
      {::onnx::Symbol("Size"), &ONNXNodeVisitor::visitSize},
      {::onnx::Symbol("Slice"), &ONNXNodeVisitor::visitSlice},
      {::onnx::Symbol("Softmax"), &ONNXNodeVisitor::visitSoftmax},
      {::onnx::Symbol("Softplus"), &ONNXNodeVisitor::visitSoftplus},
      {::onnx::Symbol("Softsign"), &ONNXNodeVisitor::visitSoftsign},
      {::onnx::Symbol("SpaceToDepth"), &ONNXNodeVisitor::visitSpaceToDepth},
      {::onnx::Symbol("Split"), &ONNXNodeVisitor::visitSplit},
      {::onnx::Symbol("Sqrt"), &ONNXNodeVisitor::visitSqrt},
      {::onnx::Symbol("Squeeze"), &ONNXNodeVisitor::visitSqueeze},
      {::onnx::Symbol("Sub"), &ONNXNodeVisitor::visitSub},
      {::onnx::Symbol("Sum"), &ONNXNodeVisitor::visitSum},
      {::onnx::Symbol("Tan"), &ONNXNodeVisitor::visitTan},
      {::onnx::Symbol("Tanh"), &ONNXNodeVisitor::visitTanh},
      {::onnx::Symbol("Tile"), &ONNXNodeVisitor::visitTile},
      {::onnx::Symbol("TopK"), &ONNXNodeVisitor::visitTopK},
      {::onnx::Symbol("Transpose"), &ONNXNodeVisitor::visitTranspose},
      {::onnx::Symbol("Unsqueeze"), &ONNXNodeVisitor::visitUnsqueeze},
      {::onnx::Symbol("Upsample"), &ONNXNodeVisitor::visitUpsample},
      {::onnx::Symbol("Xor"), &ONNXNodeVisitor::visitXor},
      {::onnx::Symbol("ATen"), &ONNXNodeVisitor::visitATen},
      {::onnx::Symbol("Affine"), &ONNXNodeVisitor::visitAffine},
      {::onnx::Symbol("ConstantFill"), &ONNXNodeVisitor::visitConstantFill},
      {::onnx::Symbol("Crop"), &ONNXNodeVisitor::visitCrop},
      {::onnx::Symbol("FC"), &ONNXNodeVisitor::visitFC},
      {::onnx::Symbol("GRUUnit"), &ONNXNodeVisitor::visitGRUUnit},
      {::onnx::Symbol("GivenTensorFill"), &ONNXNodeVisitor::visitGivenTensorFill},
      {::onnx::Symbol("If"), &ONNXNodeVisitor::visitIf},
      {::onnx::Symbol("ImageScaler"), &ONNXNodeVisitor::visitImageScaler},
      {::onnx::Symbol("Loop"), &ONNXNodeVisitor::visitLoop},
      {::onnx::Symbol("LoopIndexTensor"), &ONNXNodeVisitor::visitLoopIndexTensor},
      {::onnx::Symbol("MeanVarianceNormalization"), &ONNXNodeVisitor::visitMeanVarianceNormalization},
      {::onnx::Symbol("ParametricSoftplus"), &ONNXNodeVisitor::visitParametricSoftplus},
      {::onnx::Symbol("Scale"), &ONNXNodeVisitor::visitScale},
      {::onnx::Symbol("ScaledTanh"), &ONNXNodeVisitor::visitScaledTanh},
      {::onnx::Symbol("ThresholdedRelu"), &ONNXNodeVisitor::visitThresholdedRelu},
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
