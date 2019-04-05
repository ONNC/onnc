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
      assert(false && "unsupported tensor type");
    }
    return static_cast<result_type>(nullptr);
  };

  // 1. write tensor offset table into file (include tensor offsets)
  const char *magic = meta.weight_extension.c_str();
  file.write(reinterpret_cast<const char *>(&magic), sizeof(int8_t) * 8);
  const int64_t tensor_num = meta.packedWeightMemoryBlocks.size();
  file.write(reinterpret_cast<const char *>(&tensor_num), sizeof(int64_t));
  
  int64_t meta_offset = 
    sizeof(magic) + 
    sizeof(tensor_num) +
    sizeof(int64_t) * 2 * tensor_num;

  for (const auto& entry : meta.packedWeightMemoryBlocks) {
    const int64_t offset = entry.second.offset + meta_offset;
    const int64_t length = entry.second.length;

    file.write(reinterpret_cast<const char *>(&offset), sizeof(int64_t));
    file.write(reinterpret_cast<const char *>(&length), sizeof(int64_t));
  }

  // 2. write tensor data into file
  for (const auto& entry : meta.packedWeightMemoryBlocks) {
    const auto* const tensor      = entry.first;
    const auto&       memoryBlock = entry.second;

    file.write(getData(*tensor), memoryBlock.length);
  }

  file.close();
  outs() << "[Clang] created model weight file: " << outputFile.native() << std::endl;

  return kModuleNoChanged;
}

} // namespace onnc
