#include <string>
#include <sstream>

#include <onnx/onnx_pb.h>

// XXX: no used
std::string convertSnakeCaseToCamelCase(const std::string &str,
                                        bool initial_to_upper = false) {
  // TODO: preserve memory
  std::ostringstream r;
  bool to_upper = initial_to_upper;
  for (int i = 0; i < str.length(); ++i) {
    if (str[i] == '_') {
      to_upper = true;
    } else if (to_upper) {
      to_upper = false;
      r << std::toupper(str[i]);
    } else {
      r << str[i];
    }
  }
  return r.str();
}

std::string ConvertTypeToString(const onnx::TypeProto type) {
  std::ostringstream r;
  r << "tensor<";
  const onnx::TensorShapeProto &tensor_shape = type.tensor_type().shape();
  for (int i = 0; i < tensor_shape.dim_size(); i++) {
    const onnx::TensorShapeProto_Dimension &dim = tensor_shape.dim(i);
    if (i != 0) {
      r << ", ";
    }
    if (dim.has_dim_value()) {
      r << dim.dim_value();
    } else {
      r << dim.dim_param();
    }
  }
  r << '>';
  return r.str();
}
