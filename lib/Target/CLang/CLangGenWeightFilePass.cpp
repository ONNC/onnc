#include <fstream>
#include <iostream>
#include <vector>

#include <onnc/IR/Module.h>
#include <onnc/Runtime/onnc-runtime.h>

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
  uint64_t tensor_num = meta.packedWeightMemoryBlocks.size();
  int64_t table_size =
    sizeof(ONNC_RUNTIME_tensor_offset_table) +
    tensor_num * sizeof(ONNC_RUNTIME_tensor_offset);
  uint8_t magic[8] = ONNC_RUNTIME_TENSOR_FILE_MAGIC;

  auto* table = reinterpret_cast<ONNC_RUNTIME_tensor_offset_table*>(calloc(table_size, 1));
  memcpy(table->magic, magic, sizeof(table->magic));
  table->number_of_tensors = tensor_num;

  for (int i = 0; i < tensor_num; i++) {
    const auto entry = &meta.packedWeightMemoryBlocks[i];
    const uint64_t offset = entry->second.offset + table_size;
    const uint64_t size = entry->second.length;

    ONNC_RUNTIME_tensor_offset to = {
      .offset = offset,
      .size = size,
    };
    
    table->tensor_offsets[i] = to;
  }

  file.write(reinterpret_cast<const char *>(&table), table_size);
  free(table);

  // 2. write tensor data into file
  for (const auto& entry : meta.packedWeightMemoryBlocks) {
    const auto* const tensor      = entry.first;
    const auto&       memoryBlock = entry.second;

    file.write(getData(*tensor), memoryBlock.length);
  }

  outs() << "[Clang] created model weight file: " << outputFile.native() << std::endl;

  return kModuleNoChanged;
}

} // namespace onnc
