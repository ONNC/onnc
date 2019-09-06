//===- NvDlaUtil.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_UTIL_H_INCLUDED
#define TARGET_NVDLA_NVDLA_UTIL_H_INCLUDED

#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Range.h>

#include <initializer_list>
#include <iterator>
#include <type_traits>

namespace onnc {

ComputeOperator* getProducer(const Tensor& tensor);
std::size_t      getNumConsumers(const Tensor& tensor);
ComputeOperator& getConsumer(Tensor& tensor, std::size_t idx);

bool isGroupConv(const Conv& conv);
bool isDepthwiseConv(const Conv& conv);
bool isConstant(const Tensor& tensor);

template <typename FromSizedRange, typename ToSizedRange>
bool isBroadcastable(const FromSizedRange& fromRange, const ToSizedRange& toRange)
{
  if (size(toRange) < size(fromRange)) {
    return false;
  }

  using std::begin;
  using std::end;

  // check for general broadcast rule
  auto target = std::prev(end(toRange));
  for (auto source = std::prev(end(fromRange)); source != std::prev(begin(fromRange)); --source, --target) {
    if (!(*source == 1 || *source == *target)) {
      return false;
    }
  }

  return true;
}

bool isBroadcastable(const Tensor& fromTensor, const Tensor& toTensor);
bool isBroadcastable(const Tensor& fromTensor, std::initializer_list<Tensor::Dimension> toDimensions);

template <typename Operator,
          typename = typename std::enable_if<std::is_base_of<ComputeOperator, Operator>::value>::type>
bool isFirstOperator(const Operator& op)
{
  const auto* const input = dynamic_cast<const Tensor*>(op.getInput(0));
  if (input == nullptr) {
    return false;
  }

  return isa<InputOperator>(getProducer(*input));
}

template <typename Operator,
          typename = typename std::enable_if<std::is_base_of<ComputeOperator, Operator>::value>::type>
bool isLastOperator(const Operator& op)
{
  const auto* output = dynamic_cast<const Tensor*>(op.getOutput(0));
  if (output == nullptr) {
    return false;
  }

  const Tensor::UseList& useList = output->getUses();
  if (useList.size() != 1) {
    return false;
  }

  using std::begin;
  return isa<OutputOperator>(begin(useList)->getUser());
}

void setDimension(Tensor& tensor, Tensor::Size idx, Tensor::Dimension dimension);

void memcpy_fp32_to_uint32(float f32, uint32_t& u32);
void memcpy_uint32_to_fp32(uint32_t u32, float& f32);

} // namespace onnc

#endif
