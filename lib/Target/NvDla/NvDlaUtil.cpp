//===- NvDlaUtil.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "NvDlaUtil.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/Support/Casting.h>

#include <cassert>

namespace onnc {

bool isGroupConv(const Conv& conv) { return 1 < conv.getGroup().value(); }

bool isDepthwiseConv(const Conv& conv)
{
  assert(2 <= conv.getNumOfInputs() && "Depthwise Conv should have at least 2 input tensor");

  const Tensor* const input = conv.getInput(0);
  assert(input != nullptr);

  const IntAttr::ValueType numGroups = conv.getGroup().value();

  assert(2 <= input->getNumOfDimensions() && "Depthwise Conv input tensor should have at least 2 dimensions");
  return input->dimension(1) == numGroups;
}

bool isConstant(const Tensor& tensor)
{
  const ComputeOperator* const producer = getProducer(tensor);

  return isa<Initializer>(producer);
}

bool isBroadcastable(const Tensor& fromTensor, const Tensor& toTensor)
{
  return isBroadcastable(fromTensor.getDimensions(), toTensor.getDimensions());
}

bool isBroadcastable(const Tensor& fromTensor, std::initializer_list<Tensor::Dimension> toDimensions)
{
  return isBroadcastable(fromTensor.getDimensions(), toDimensions);
}

ComputeOperator* getProducer(const Tensor& tensor) { return dynamic_cast<ComputeOperator*>(tensor.getDefine()); }

void setDimension(Tensor& tensor, Tensor::Size idx, Tensor::Dimension dimension)
{
  assert(idx < tensor.getNumOfDimensions());

  Tensor::Dimensions dimensions = tensor.getDimensions();
  dimensions[idx]               = dimension;
  tensor.setDimensions(dimensions);
}

std::size_t getNumConsumers(const Tensor& tensor) { return tensor.getUses().size(); }

ComputeOperator& getConsumer(Tensor& tensor, std::size_t idx)
{
  assert(idx < getNumConsumers(tensor));
  Value::UseList& useList = tensor.getUses();

  ComputeOperator* user = useList[idx].getUser();
  assert(user != nullptr);

  return *user;
}

void memcpy_fp32_to_uint32(float f32, uint32_t& u32)
{
  // NVDLA is using little endian.
  uint8_t* target = (uint8_t*)&u32;
  target[0]       = *(uint32_t*)&f32 & 0xFF;
  target[1]       = *(uint32_t*)&f32 >> 8 & 0xFF;
  target[2]       = *(uint32_t*)&f32 >> 16 & 0xFF;
  target[3]       = *(uint32_t*)&f32 >> 24 & 0xFF;
}

void memcpy_uint32_to_fp32(uint32_t u32, float& f32)
{
  // NVDLA is using little endian.
  uint8_t* u = (uint8_t*)&u32;
  uint8_t* f = (uint8_t*)&f32;
  f[0]       = u[0];
  f[1]       = u[1];
  f[2]       = u[2];
  f[3]       = u[3];
}

} // namespace onnc
