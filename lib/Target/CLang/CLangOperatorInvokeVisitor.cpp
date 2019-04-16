#include "CLangOperatorInvokeVisitor.h"

#include <onnc/ADT/StringRef.h>
#include <onnc/IR/Compute.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/ComputeOperator.h>

#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#define PP_GEN_CLASS_NAME() CLangOperatorInvokeVisitor
#define PP_GEN_VISIT_DEF(type, param)                                                          \
  PP_GEN_VISIT_RETURN_TYPE() PP_GEN_CLASS_NAME()::visit(PP_GEN_VISIT_PARAM_TYPE(type) param)   \
  {                                                                                            \
    auto&& target = internal::getTarget(PP_STRINGIFY(type));                                   \
    stream << indent_ << "// " << PP_STRINGIFY(type) << '\n';                                  \
    PP_ENTER_SCOPE(stream, indent_);                                                           \
    visitImpl(param, indent_ + 1, target);                                                     \
    PP_LEAVE_SCOPE(stream, indent_);                                                           \
  }                                                                                            \
  PP_GEN_VISIT_RETURN_TYPE()                                                                   \
  PP_GEN_CLASS_NAME()::visitImpl(PP_GEN_VISIT_PARAM_TYPE(type) param, internal::Indent indent, \
                                 const identifier_type& target)

#define PP_ENTER_SCOPE(stream, indent) stream << indent << "{\n"
#define PP_LEAVE_SCOPE(stream, indent) stream << indent << "}\n"

#define PP_BUILTIN_TYPE_LIST                                                                                           \
  (int32_t, int32_t*, const int32_t, const int32_t* const, float, float*, float**, float* const*, const float* const*, \
   void*, const char*)
#define PP_GEN_TYPE_HOLDER_DEF(type) \
  template <>                        \
  struct holder<type>                \
  {};
#define PP_GEN_TYPE_HOLDER_STREAM_OP(type)                                                                    \
  inline PP_GEN_CLASS_NAME()::stream_type& operator<<(PP_GEN_CLASS_NAME()::stream_type& stream, holder<type>) \
  {                                                                                                           \
    return stream << PP_STRINGIFY(type);                                                                      \
  }

namespace onnc {
namespace internal {
template <typename T>
struct holder;

PP_LIST_FOR_EACH(PP_GEN_TYPE_HOLDER_DEF, PP_UNWRAP(PP_BUILTIN_TYPE_LIST))
PP_LIST_FOR_EACH(PP_GEN_TYPE_HOLDER_STREAM_OP, PP_UNWRAP(PP_BUILTIN_TYPE_LIST))

using identifier_type = PP_GEN_CLASS_NAME()::identifier_type;
using expression_type = PP_GEN_CLASS_NAME()::expression_type;
using stream_type     = PP_GEN_CLASS_NAME()::stream_type;
using tensor_type     = PP_GEN_CLASS_NAME()::TensorType;

enum class ContextMember : unsigned short
{
  input,
  weight,
  id,
  completed
};

static_assert(std::is_convertible<std::string, identifier_type>::value,
              "Should can create identifier_type from std::string");
static_assert(std::is_convertible<std::string, expression_type>::value,
              "Should can create expression_type from std::string");
static_assert(std::is_convertible<identifier_type, expression_type>::value,
              "identifier_type itself is a kind of expression_type");

inline identifier_type getTarget(const char* op)
{
  std::ostringstream stream;
  if (op != nullptr) {
    stream << "ONNC_RUNTIME_" << StringRef{op}.lower() << "_float";
  }
  return stream.str();
}

inline identifier_type getIdentifier()
{
  static unsigned serialNumber = 0;

  std::ostringstream stream;
  stream << "v" << ++serialNumber;
  return stream.str();
}

inline identifier_type getIdentifier(identifier_type context, ContextMember member)
{
  std::ostringstream stream;
  stream << "(";
  switch (member) {
  case ContextMember::input:
    stream << context << "->input";
    break;
  case ContextMember::weight:
    stream << context << "->weight";
    break;
  case ContextMember::id:
    stream << context << "->id";
    break;
  case ContextMember::completed:
    stream << context << "->completed";
    break;
  default:
    assert(false && "meet unknown context member");
  }
  stream << ")";

  return stream.str();
}

template <typename T>
inline identifier_type defineVar(stream_type& stream, Indent indent, const T& value)
{
  const auto id = getIdentifier();
  stream << indent << holder<T>{} << " " << id << " = " << value << ";\n";
  return id;
}

template <typename T>
inline identifier_type defineVarByExpr(stream_type& stream, Indent indent, const expression_type& expr)
{
  const auto id = getIdentifier();
  stream << indent << holder<T>{} << " " << id << " = " << expr << ";\n";
  return id;
}

template <typename T, typename InputRange>
inline identifier_type defineArray(stream_type& stream, Indent indent, const InputRange& range)
{
  const auto array = getIdentifier();
  stream << indent << holder<T>{} << " " << array << "[] = { ";
  bool isFirst = true;
  for (const auto& element : range) {
    if (!isFirst) {
      stream << ", ";
    }
    stream << element;
    isFirst = false;
  }
  stream << " };\n";
  return array;
}

inline std::size_t getInputIndex(const CLangMeta& meta, const Tensor& tensor)
{
  std::size_t index = 0;
  for (const auto& entry : meta.packedInputMemoryBlocks) {
    const auto* const value = entry.first;
    if (value == &tensor) {
      return index;
    }
    ++index;
  }

  assert(false && "cannot find such tensor in meta");
  return static_cast<std::size_t>(-1);
}

inline std::size_t getWeightIndex(const CLangMeta& meta, const Tensor& tensor)
{
  std::size_t index = 0;
  for (const auto& entry : meta.packedWeightMemoryBlocks) {
    const auto* const value = entry.first;
    if (value == &tensor) {
      return index;
    }
    ++index;
  }

  assert(false && "cannot find such tensor in meta");
  return static_cast<std::size_t>(-1);
}

inline CLangMemoryBlock::address_type getInternalOffset(const CLangMeta& meta, const Tensor& tensor)
{
  for (const auto& entry : meta.packedInternalMemoryBlocks) {
    const auto* const value = entry.first;
    if (value == &tensor) {
      const auto& memory = entry.second;
      return memory.offset;
    }
  }

  assert(false && "cannot find such tensor in meta");
  return static_cast<CLangMemoryBlock::address_type>(-1);
}

template <typename T>
inline expression_type toExpr(const T& value)
{
  return std::to_string(value);
}

inline expression_type toExpr(const char* str)
{
  assert(str != nullptr);

  std::ostringstream stream;
  stream << "\"" << str << "\"" << std::endl;
  return stream.str();
}

inline expression_type toExpr(const Tensor* tensor)
{
  assert(false && "not implemented yet");
  return "";
}

template <typename T>
inline expression_type castExpr(const expression_type& expr)
{
  std::ostringstream stream;
  stream << "((" << holder<T>{} << ")(" << expr << "))";
  return stream.str();
}

inline identifier_type defineDimensionArray(stream_type& stream, Indent indent, const Tensor& tensor)
{
  return defineArray<const std::int32_t>(stream, indent, tensor.getDimensions());
}

template <tensor_type>
inline identifier_type defineDimensionArrays(stream_type& stream, Indent indent, const ComputeOperator& op,
                                             std::size_t first);

template <>
inline identifier_type defineDimensionArrays<tensor_type::inputs>(stream_type& stream, Indent indent,
                                                                  const ComputeOperator& op, std::size_t first)
{
  if (op.getNumOfInputs() <= first) {
    return identifier_type{};
  }

  std::vector<identifier_type> tensors;
  for (std::size_t idx = 0; idx < op.getNumOfInputs(); ++idx) {
    if (const auto* const tensor = dynamic_cast<const Tensor*>(op.getInput(idx))) {
      tensors.emplace_back(defineDimensionArray(stream, indent, *tensor));
    }
  }

  return defineArray<const std::int32_t* const>(stream, indent, tensors);
}

template <>
inline identifier_type defineDimensionArrays<tensor_type::outputs>(stream_type& stream, Indent indent,
                                                                   const ComputeOperator& op, std::size_t first)
{
  if (op.getNumOfOutputs() <= first) {
    return identifier_type{};
  }

  std::vector<identifier_type> tensors;
  for (std::size_t idx = 0; idx < op.getNumOfOutputs(); ++idx) {
    if (const auto* const tensor = dynamic_cast<const Tensor*>(op.getOutput(idx))) {
      tensors.emplace_back(defineDimensionArray(stream, indent, *tensor));
    }
  }

  return defineArray<const std::int32_t* const>(stream, indent, tensors);
}

inline void invoke(stream_type& stream, Indent indent, const identifier_type& name,
                   std::initializer_list<expression_type> exprs)
{
  stream << indent << name << "(";
  bool isFirst = true;
  for (const auto& expr : exprs) {
    if (!isFirst) {
      stream << ", ";
    }
    stream << expr;
    isFirst = false;
  }
  stream << ");\n";
}

template <tensor_type>
inline identifier_type defineDimensionNumberArray(stream_type& stream, Indent indent, const ComputeOperator& op,
                                                  std::size_t first);

template <>
inline identifier_type defineDimensionNumberArray<tensor_type::inputs>(stream_type& stream, Indent indent,
                                                                       const ComputeOperator& op, std::size_t first)
{
  if (op.getNumOfInputs() <= first) {
    return identifier_type{};
  }

  std::vector<Tensor::Size> sizes;
  for (std::size_t idx = 0; idx < op.getNumOfInputs(); ++idx) {
    if (const auto* const tensor = dynamic_cast<const Tensor*>(op.getInput(idx))) {
      sizes.emplace_back(tensor->getNumOfDimensions());
    }
  }

  return defineArray<const std::int32_t>(stream, indent, sizes);
}

template <>
inline identifier_type defineDimensionNumberArray<tensor_type::outputs>(stream_type& stream, Indent indent,
                                                                        const ComputeOperator& op, std::size_t first)
{
  if (op.getNumOfOutputs() <= first) {
    return identifier_type{};
  }

  std::vector<Tensor::Size> sizes;
  for (std::size_t idx = 0; idx < op.getNumOfOutputs(); ++idx) {
    if (const auto* const tensor = dynamic_cast<const Tensor*>(op.getOutput(idx))) {
      sizes.emplace_back(tensor->getNumOfDimensions());
    }
  }

  return defineArray<const std::int32_t>(stream, indent, sizes);
}

} // namespace internal

using namespace internal;

void PP_GEN_CLASS_NAME()::visit(const Module& module)
{
  prepareMemoryTypes();
  prepareMemorySizes();

  for (const auto& computeOperator : *module.getRootComputeGraph()) {
    computeOperator.accept(*this);
  }
}

PP_GEN_VISIT_DEF(OutputOperator, op)
{
  for (std::size_t idx = 0; idx < op.getNumOfInputs(); ++idx) {
    if (const auto* value = dynamic_cast<const Tensor*>(op.getInput(idx))) {
      const auto tensor = defineTensor(indent, *value);
      const auto output = getIdentifier();
      stream << indent << "struct ONNC_RUNTIME_tensor_view " << output << " = {\n"
             << indent + 1 << ".data = " << tensor << ",\n"
             << indent + 1 << ".size = " << getMemorySize(*value) << "\n"
             << indent << "};\n";

      invoke(stream, indent, getIdentifier(context, ContextMember::completed),
             {getIdentifier(context, ContextMember::id), output});
    }
  }
}

identifier_type PP_GEN_CLASS_NAME()::defineTensor(internal::Indent indent, const Tensor& tensor)
{
  const auto getInitializer = [this](const Tensor& tensor) -> expression_type {
    std::ostringstream stream;
    switch (getMemoryType(tensor)) {
    case MemoryType::input:
      stream << "ONNC_RUNTIME_read_tensor(" << getIdentifier(context, ContextMember::input) << ", "
             << getInputIndex(meta, tensor) << ").data";
      break;
    case MemoryType::weight:
      stream << "ONNC_RUNTIME_read_tensor(" << getIdentifier(context, ContextMember::weight) << ", "
             << getWeightIndex(meta, tensor) << ").data";
      break;
    case MemoryType::internal:
      stream << castExpr<float*>(memory + " + " + toExpr(getInternalOffset(meta, tensor)));
      break;
    default:
      assert(false && "should not reach here");
      break;
    }
    return stream.str();
  };

  const auto id = getIdentifier();
  stream << indent << holder<float*>{} << " const " << id << " = " << getInitializer(tensor) << ";\n";

  return id;
}

template <>
identifier_type PP_GEN_CLASS_NAME()::defineTensors<tensor_type::inputs>(internal::Indent       indent,
                                                                        const ComputeOperator& op, std::size_t first)
{
  if (op.getNumOfInputs() <= first) {
    return identifier_type{};
  }

  std::vector<identifier_type> tensors;
  for (std::size_t idx = 0; idx < op.getNumOfInputs(); ++idx) {
    if (const auto* const tensor = dynamic_cast<const Tensor*>(op.getInput(idx))) {
      tensors.emplace_back(defineTensor(indent, *tensor));
    }
  }

  return defineArray<float*>(stream, indent, tensors);
}

template <>
identifier_type PP_GEN_CLASS_NAME()::defineTensors<tensor_type::outputs>(internal::Indent       indent,
                                                                         const ComputeOperator& op, std::size_t first)
{
  if (op.getNumOfOutputs() <= first) {
    return identifier_type{};
  }

  std::vector<identifier_type> tensors;
  for (std::size_t idx = 0; idx < op.getNumOfOutputs(); ++idx) {
    if (const auto* const tensor = dynamic_cast<const Tensor*>(op.getOutput(idx))) {
      tensors.emplace_back(defineTensor(indent, *tensor));
    }
  }

  return defineArray<float*>(stream, indent, tensors);
}

void PP_GEN_CLASS_NAME()::prepareMemoryTypes()
{
  memoryTypes.clear();

  for (const auto& entry : meta.packedInputMemoryBlocks) {
    const auto* const tensor = entry.first;
    memoryTypes.emplace(tensor, MemoryType::input);
  }

  for (const auto& entry : meta.packedWeightMemoryBlocks) {
    const auto* const tensor = entry.first;
    memoryTypes.emplace(tensor, MemoryType::weight);
  }

  for (const auto& entry : meta.packedInternalMemoryBlocks) {
    const auto* const tensor = entry.first;
    memoryTypes.emplace(tensor, MemoryType::internal);
  }
}

PP_GEN_CLASS_NAME()::MemoryType PP_GEN_CLASS_NAME()::getMemoryType(const Tensor& tensor) const
{
  const auto found = memoryTypes.find(&tensor);
  if (found == std::end(memoryTypes)) {
    return MemoryType::none;
  }

  return found->second;
}

void PP_GEN_CLASS_NAME()::prepareMemorySizes()
{
  memorySizes.clear();

  for (const auto& entry : meta.packedInternalMemoryBlocks) {
    const auto* const tensor = entry.first;
    memorySizes.emplace(tensor, entry.second.length);
  }
}

CLangMemoryBlock::size_type PP_GEN_CLASS_NAME()::getMemorySize(const Tensor& tensor) const
{
  const auto found = memorySizes.find(&tensor);
  if (found == std::end(memorySizes)) {
    return 0;
  }

  return found->second;
}

#include "internal/Visitors.inc"

} // namespace onnc
