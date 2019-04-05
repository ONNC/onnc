#include <fstream>
#include <iostream>
#include <vector>

#include <onnc/IR/Module.h>

#include "CLangGenWeightFilePass.h"


namespace onnc {

CLangGenWeightFilePass::ReturnType CLangGenWeightFilePass::runOnModule(Module& module)
{
  // pack weight memory blocks together (one is right after the other)
  std::ofstream file(outputFile.native(), std::ios::binary);
  // 1. write tensor offset table into file (include tensor offsets)
  //    no implement yet
  // 2. write tensor data into file
  static const auto getData = [](const Tensor& tensor) {
    using result_type = const std::ofstream::char_type*;
    switch (tensor.kind()) {
    case Value::kFloat:
      {
        const auto& floatTensor = static_cast<const FloatTensor&>(tensor);
        return reinterpret_cast<result_type>(floatTensor.getValues().data());
      }
    case Value::kInt64:
      {
        const auto& int64Tensor = static_cast<const Int64Tensor&>(tensor);
        return reinterpret_cast<result_type>(int64Tensor.getValues().data());
      }
    default:
      assert(false && "unsupport tensor type");
    }
    return static_cast<result_type>(nullptr);
  };

  for (const auto& entry : meta.packedWeightMemoryBlocks) {
    const auto* const tensor      = entry.first;
    const auto&       memoryBlock = entry.second;

    file.write(getData(*tensor), memoryBlock.length);
  }

  return kModuleNoChanged;
}

} // namespace onnc
