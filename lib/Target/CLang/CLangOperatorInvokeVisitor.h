#ifndef TARGET_CLANG_OPERATOR_INVOKE_VISITOR_H_INCLUDED
#define TARGET_CLANG_OPERATOR_INVOKE_VISITOR_H_INCLUDED

#include "CLangMeta.h"
#include "internal/Indent.h"

#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/CustomVisitor.h>
#include <onnc/IR/Module.h>
#include <onnc/Support/Preprocessor.h>

#include <iterator>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#ifndef PP_GEN_VISIT_PARAM_TYPE
#  define PP_GEN_VISIT_PARAM_TYPE(type) const type&
#endif

#ifndef PP_GEN_VISIT_RETURN_TYPE
#  define PP_GEN_VISIT_RETURN_TYPE() void
#endif

#ifndef PP_GEN_VISIT_DECL
#  define PP_GEN_VISIT_DECL(type)                                             \
    PP_GEN_VISIT_RETURN_TYPE() visit(PP_GEN_VISIT_PARAM_TYPE(type)) override; \
    PP_GEN_VISIT_RETURN_TYPE() visitImpl(PP_GEN_VISIT_PARAM_TYPE(type), internal::Indent, const std::string&);
#endif

#ifndef PP_VISIT_TYPE_LIST
#  define PP_VISIT_TYPE_LIST                                                                                           \
    (Abs, Acos, Add, Affine, And, ArgMax, ArgMin, Asin, Atan, ATen, AveragePool, BatchNormalization, Cast, Ceil, Clip, \
     Concat, Constant, ConstantFill, Conv, ConvTranspose, Cos, Crop, DepthToSpace, Div, Dropout, Elu, Equal, Exp,      \
     Expand, Flatten, Floor, Gather, Gemm, GivenTensorFill, GlobalAveragePool, GlobalLpPool, GlobalMaxPool, Greater,   \
     GRU, GRUUnit, Hardmax, HardSigmoid, Identity, ImageScaler, InstanceNormalization, LeakyRelu, Less, Log,           \
     LogSoftmax, LpNormalization, LpPool, LRN, LSTM, MatMul, Max, MaxPool, MaxRoiPool, Mean,                           \
     MeanVarianceNormalization, Min, Mul, Multinomial, Neg, Not, Or, Pad, ParametricSoftplus, Pow, PRelu,              \
     RandomNormal, RandomNormalLike, RandomUniform, RandomUniformLike, Reciprocal, ReduceL1, ReduceL2, ReduceLogSum,   \
     ReduceLogSumExp, ReduceMax, ReduceMean, ReduceMin, ReduceProd, ReduceSum, ReduceSumSquare, Relu, Reshape, RNN,    \
     Scale, ScaledTanh, Selu, Shape, Sigmoid, Sin, Size, Slice, Softmax, Softplus, Softsign, SpaceToDepth, Split,      \
     Sqrt, Squeeze, Sub, Sum, Tan, Tanh, ThresholdedRelu, Tile, TopK, Transpose, Unsqueeze, Upsample, Xor)
#endif

namespace onnc {

class CLangOperatorInvokeVisitor : public CustomVisitor<CLangOperatorInvokeVisitor>
{
public:
  using stream_type     = std::ostream;
  using identifier_type = std::string;
  using expression_type = std::string;

  enum class TensorType : unsigned short
  {
    inputs,
    outputs,
    input  = inputs,
    output = outputs,
    data_0 = inputs,
  };

  CLangOperatorInvokeVisitor();
  CLangOperatorInvokeVisitor(const CLangMeta& meta, stream_type& stream, internal::Indent indent,
                             identifier_type memory, identifier_type input, identifier_type weight)
    : meta{meta}
    , stream{stream}
    , indent_{indent}
    , memory{std::move(memory)}
    , input{std::move(input)}
    , weight{std::move(weight)}
  {}

  CLangOperatorInvokeVisitor(const CLangOperatorInvokeVisitor&) = delete;
  CLangOperatorInvokeVisitor(CLangOperatorInvokeVisitor&&)      = delete;

  ~CLangOperatorInvokeVisitor() = default;

  CLangOperatorInvokeVisitor& operator=(const CLangOperatorInvokeVisitor&) = delete;
  CLangOperatorInvokeVisitor& operator=(CLangOperatorInvokeVisitor&&) = delete;

  void visit(const Module&);

  PP_LIST_FOR_EACH(PP_GEN_VISIT_DECL, PP_UNWRAP(PP_VISIT_TYPE_LIST))

  identifier_type defineTensor(internal::Indent indent, const Tensor& tensor);

  template <TensorType type>
  identifier_type defineTensors(internal::Indent indent, const ComputeOperator& op, std::size_t first = 0);

private:
  enum class MemoryType : unsigned short
  {
    none,
    input,
    weight,
    internal
  };

  using memory_types_type = std::unordered_map<const Tensor*, MemoryType>;

  void       prepareMemoryTypes();
  MemoryType getMemoryType(const Tensor& tensor);

  const CLangMeta&       meta;
  stream_type&           stream;
  const internal::Indent indent_;
  const identifier_type  memory;
  const identifier_type  input;
  const identifier_type  weight;
  memory_types_type      memoryTypes;
};

} // namespace onnc

#undef PP_GEN_METHOD_DECL

#endif
