#include "CLangGenWeightFilePass.h"

#include <onnc-runtime.h>

#include <onnc/IR/Module.h>

#include <fstream>
#include <iostream>
#include <vector>

namespace onnc {

CLangGenWeightFilePass::ReturnType CLangGenWeightFilePass::runOnModule(Module& module)
{
  // pack weight memory blocks together (one is right after the other)
  std::ofstream file(outputFile.native(), std::ios::binary);

  static const auto getData = [](const Tensor& tensor) {
    using result_type = const std::ofstream::char_type*;
    switch (tensor.kind()) {
    case Value::kFloat: {
      const auto& floatTensor = static_cast<const FloatTensor&>(tensor);
      return reinterpret_cast<result_type>(floatTensor.getValues().data());
    }
    case Value::kInt64: {
      const auto& int64Tensor = static_cast<const Int64Tensor&>(tensor);
      return reinterpret_cast<result_type>(int64Tensor.getValues().data());
    }
    default:
      assert(false && "unsupported tensor type");
    }
    return static_cast<result_type>(nullptr);
  };

  // 1. write tensor offset table into file (include tensor offsets)
  const auto tensor_num = meta.packedWeightMemoryBlocks.size();
  const auto table_size = sizeof(ONNC_RUNTIME_tensor_offset_table) + tensor_num * sizeof(ONNC_RUNTIME_tensor_offset);

  auto* const table = reinterpret_cast<ONNC_RUNTIME_tensor_offset_table*>(calloc(table_size, 1));

  strncpy(table->magic, ONNC_RUNTIME_TENSOR_FILE_MAGIC, sizeof(table->magic));
  table->number_of_tensors = tensor_num;

  for (std::size_t idx = 0; idx < tensor_num; ++idx) {
    const auto& from = meta.packedWeightMemoryBlocks[idx].second;
    auto&       to   = table->tensor_offsets[idx];
    to.size          = from.length;
    to.offset        = table_size + from.offset;
  }

  using stream_type = std::ofstream;
  using char_type   = stream_type::char_type;

  file.write(reinterpret_cast<const char_type*>(table), table_size);
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
