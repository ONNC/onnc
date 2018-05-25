#ifndef ONNC_TARGET_TG_ONNX_IR_PRINTER_H
#define ONNC_TARGET_TG_ONNX_IR_PRINTER_H

#include <string>
#include <sstream>
#include <memory>
#include <onnx/onnx_pb.h>
#include "onnx/common/ir_pb_converter.h"

// FIXME Printer can a a ModulePass

namespace ONNXIRPrinter {

inline std::string
convertSnakeCaseToCamelCase(const std::string &str, bool initial_to_upper = false)
{
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

inline std::string ConvertTypeToString(const onnx::TypeProto type)
{
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

inline void dumpModelProto(const onnx::ModelProto &model)
{

  // dump model info
  if (model.has_ir_version()) {
    std::cout << "model.ir_version: " << model.ir_version() << std::endl;
  }
  if (model.has_producer_name()) {
    std::cout << "model.producer_name: " << model.producer_name() << std::endl;
  }
  if (model.has_producer_version()) {
    std::cout << "model.producer_version: " << model.producer_version()
              << std::endl;
  }
  if (model.has_domain()) {
    std::cout << "model.domain: " << model.domain() << std::endl;
  }
  if (model.has_model_version()) {
    std::cout << "model.model_version: " << model.model_version() << std::endl;
  }
  if (model.has_doc_string()) {
    std::cout << "model.doc_string: " << model.doc_string() << std::endl;
  }

  if (!model.has_graph()) {
    return;
  }

  // dump graph
  const onnx::GraphProto &graph = model.graph();

  std::cout << "graph " << graph.name() << " (";

  for (int i = 0; i < graph.input_size(); i++) {
    if (i != 0) { std::cout << ", "; }
    const onnx::TypeProto &type = graph.input(i).type();
    if (type.has_tensor_type()) {
      const onnx::TypeProto::Tensor &tensor_type = type.tensor_type();
      std::cout << TensorProto_DataType_Name(tensor_type.elem_type()) << " tensor <";
      const onnx::TensorShapeProto &shape = tensor_type.shape();
      for (int j = 0; j < shape.dim_size(); j++) {
        if (j != 0) { std::cout << ", "; }
        if (shape.dim(j).has_dim_value()) {
          std::cout << shape.dim(j).dim_value();
        } else {
          std::cout << shape.dim(j).dim_param();
        }
      }
      std::cout << "> ";
    }
    std::cout << '%' << graph.input(i).name() << std::endl;
  }
  std::cout << ") {" << std::endl;

  for (int i = 0; i < graph.initializer_size(); i++) {
    const onnx::TensorProto &initializer = graph.initializer(i);
    std::cout << "  initialize input %" << initializer.name() << std::endl;
  }

  for (int i = 0; i < graph.node_size(); i++) {
    const onnx::NodeProto &node = graph.node(i);
    std::cout << "  ";
    for (int j = 0; j < node.output_size(); j++) {
      if (j != 0) {
        std::cout << ", ";
      }
      std::cout << '%' << node.output(j);
    }
    std::cout << " = ";
    std::cout << node.op_type();
    if (node.attribute_size() != 0) {
      std::cout << '<';
    }
    for (int j = 0; j < node.attribute_size(); j++) {
      const onnx::AttributeProto &attribute = node.attribute(j);
      if (j != 0) {
        std::cout << ", ";
      }
      std::cout << attribute.name() << ":";
      switch (attribute.type()) {
      case onnx::AttributeProto::FLOAT:
        std::cout << attribute.f();
        break;
      case onnx::AttributeProto::INT:
        std::cout << attribute.i();
        break;
      case onnx::AttributeProto::STRING:
        std::cout << attribute.s();
        break;
      case onnx::AttributeProto::TENSOR:
      case onnx::AttributeProto::GRAPH:
        std::cout << attribute.name();
        break;
      case onnx::AttributeProto::FLOATS:
        std::cout << '[';
        for (int k = 0; k < attribute.floats_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.floats(k);
        }
        std::cout << ']';
        break;
      case onnx::AttributeProto::INTS:
        std::cout << '[';
        for (int k = 0; k < attribute.ints_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.ints(k);
        }
        std::cout << ']';
        break;
      case onnx::AttributeProto::STRINGS:
        std::cout << '[';
        for (int k = 0; k < attribute.strings_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.strings(k);
        }
        std::cout << ']';
        break;
      case onnx::AttributeProto::TENSORS:
      case onnx::AttributeProto::GRAPHS:
        std::cout << attribute.name();
        break;
      default:
        std::cout << "unknown";
      }
    }
    if (node.attribute_size() != 0) {
      std::cout << '>';
    }
    std::cout << '(';
    for (int j = 0; j < node.input_size(); j++) {
      if (j != 0) {
        std::cout << ", ";
      }
      std::cout << '%' << node.input(j);
    }
    std::cout << ')' << std::endl;
  }

  std::cout << "  return ";
  // unordered_map<std::string, std::string> output_type;
  for (int i = 0; i < graph.output_size(); i++) {
    if (i != 0) {
      std::cout << ", ";
    }
    std::cout << ConvertTypeToString(graph.output(i).type()) << " %"
              << graph.output(i).name();
  }
  std::cout << std::endl;
  std::cout << '}' << std::endl;
}

inline void dumpGraph(const std::shared_ptr<onnx::Graph>& g)
{
  onnx::ModelProto model;
  onnx::ExportModelProto(&model, g);
  dumpModelProto(model);
}

} // ONNXIRPrinter namespace

#endif
