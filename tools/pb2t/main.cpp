#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#define restrict
#include <onnc/Runtime/onnc-runtime.h>
#undef restrict

#include <onnc/Config/ONNX.h>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "usage: " << argv[0] << "input.pb output.tensor" << std::endl;
    return EXIT_FAILURE;
  }

  onnc::xTensorProto reader;
	{
	std::ifstream input{argv[1], std::ios::binary};
    if (!input) {
      std::cerr << "Failed to open file: " << argv[1] << std::endl;
      return EXIT_FAILURE;
    }
  reader.ParseFromIstream(&input);
	}

  assert(reader.has_data_type() && reader.data_type() == onnc::xTensorProto::FLOAT);

  const auto& input  = reader.raw_data();
  const auto  length = input.length();

  std::ofstream output{argv[2], std::ios::binary};
  const std::size_t tableSize = sizeof(ONNC_RUNTIME_tensor_offset_table) + sizeof(ONNC_RUNTIME_tensor_offset);
  auto * const table = reinterpret_cast<ONNC_RUNTIME_tensor_offset_table*>(std::calloc(tableSize, 1));
  std::strncpy(table->magic, ONNC_RUNTIME_TENSOR_FILE_MAGIC, sizeof(table->magic));
  table->number_of_tensors = 1;
  table->tensor_offsets[0].offset = tableSize;
  table->tensor_offsets[0].size   = length;

  output.write(reinterpret_cast<const std::ofstream::char_type*>(table), tableSize);
  std::free(table);
  output.write(reinterpret_cast<const std::ofstream::char_type*>(input.data()), length);
}
