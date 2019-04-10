#ifndef TARGET_CLANG_OPERATOR_INVOKE_VISITOR_H_INCLUDED
#define TARGET_CLANG_OPERATOR_INVOKE_VISITOR_H_INCLUDED

#include "CLangMeta.h"
#include "internal/Indent.h"

#include <onnc/IR/CustomVisitor.h>
#include <onnc/IR/Module.h>
#include <onnc/Support/Preprocessor.h>

#include <ostream>

#ifndef PP_GEN_VISIT_PARAM_TYPE
# define PP_GEN_VISIT_PARAM_TYPE(type) const type&
#endif

#ifndef PP_GEN_VISIT_RETURN_TYPE
# define PP_GEN_VISIT_RETURN_TYPE() void
#endif

#ifndef PP_GEN_VISIT_DECL
# define PP_GEN_VISIT_DECL(type) \
  PP_GEN_VISIT_RETURN_TYPE() visit(PP_GEN_VISIT_PARAM_TYPE(type)) override; \
  PP_GEN_VISIT_RETURN_TYPE() visitImpl(PP_GEN_VISIT_PARAM_TYPE(type), internal::Indent);
#endif

namespace onnc {

class CLangOperatorInvokeVisitor : public CustomVisitor<CLangOperatorInvokeVisitor>
{
public:
  using stream_type = std::ostream;

  CLangOperatorInvokeVisitor();
  CLangOperatorInvokeVisitor(const CLangMeta& meta, stream_type& stream, internal::Indent indent)
    : meta{meta}
    , stream{stream}
    , indent_{indent}
  {}

  CLangOperatorInvokeVisitor(const CLangOperatorInvokeVisitor&) = delete;
  CLangOperatorInvokeVisitor(CLangOperatorInvokeVisitor&&) = delete;

  ~CLangOperatorInvokeVisitor() = default;

  CLangOperatorInvokeVisitor& operator=(const CLangOperatorInvokeVisitor&) = delete;
  CLangOperatorInvokeVisitor& operator=(CLangOperatorInvokeVisitor&&) = delete;

	void visit(const Module&);

  /*
  PP_LIST_FOR_EACH(PP_GEN_VISIT_DECL,
    Abs, Acos, Add, Affine, And, ArgMax, ArgMin, Asin, Atan, ATen, AveragePool, BatchNormalization, Cast, Ceil, Clip,
    Concat, Constant, ConstantFill, Conv, ConvTranspose, Cos, Crop, DepthToSpace, Div, Dropout, Elu, Equal, Exp,
    Expand, Flatten, Floor, Gather, Gemm, GivenTensorFill, GlobalAveragePool, GlobalLpPool, GlobalMaxPool, Greater,
    GRU, GRUUnit, Hardmax, HardSigmoid, Identity, ImageScaler, InstanceNormalization, LeakyRelu, Less, Log, LogSoftmax,
    LpNormalization, LpPool, LRN, LSTM, MatMul, Max, MaxPool, MaxRoiPool, Mean, MeanVarianceNormalization, Min, Mul,
    Multinomial, Neg, Not, Or, Pad, ParametricSoftplus, Pow, PRelu, RandomNormal, RandomNormalLike, RandomUniform,
    RandomUniformLike, Reciprocal, ReduceL1, ReduceL2, ReduceLogSum, ReduceLogSumExp, ReduceMax, ReduceMean, ReduceMin,
    ReduceProd, ReduceSum, ReduceSumSquare, Relu, Reshape, RNN, Scale, ScaledTanh, Selu, Shape, Sigmoid, Sin, Size,
    Slice, Softmax, Softplus, Softsign, SpaceToDepth, Split, Sqrt, Squeeze, Sub, Sum, Tan, Tanh, ThresholdedRelu, Tile,
    TopK, Transpose, Unsqueeze, Upsample, Xor
  )
  */

  PP_LIST_FOR_EACH(PP_GEN_VISIT_DECL,
    Softmax, Conv, Gemm, LRN, Relu, MaxPool, Reshape
  )

private:
  const CLangMeta&       meta;
  stream_type&           stream;
  const internal::Indent indent_;
};

} // namespace onnc

#undef PP_GEN_METHOD_DECL

#endif
