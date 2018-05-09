#include <memory>
#include <onnc/IR/ONNCModulePrinter.h>
#include <onnx/common/ir_pb_converter.h>
#include <onnx/onnx_pb.h>
#include <sstream>
#include <string>

using namespace onnc;

namespace {

std::string ConvertTypeToString(const ::onnx::TypeProto type)
{
  std::ostringstream r;
  r << "tensor<";
  const ::onnx::TensorShapeProto &tensor_shape = type.tensor_type().shape();
  for (int i = 0; i < tensor_shape.dim_size(); i++) {
    const ::onnx::TensorShapeProto_Dimension &dim = tensor_shape.dim(i);
    if (dim.has_dim_value()) {
      r << dim.dim_value();
    } else {
      r << dim.dim_param();
    }
    if (i != tensor_shape.dim_size() - 1) {
      r << ", ";
    }
  }
  r << '>';
  return r.str();
}

void dumpModelProto(const ::onnx::ModelProto &model)
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
  const ::onnx::GraphProto &graph = model.graph();

  std::cout << "graph " << graph.name() << " (";

  for (int i = 0; i < graph.input_size(); i++) {
    const ::onnx::TypeProto &type = graph.input(i).type();
    if (type.has_tensor_type()) {
      const ::onnx::TypeProto::Tensor &tensor_type = type.tensor_type();
      std::cout << TensorProto_DataType_Name(tensor_type.elem_type())
                << " tensor <";
      const ::onnx::TensorShapeProto &shape = tensor_type.shape();
      for (int j = 0; j < shape.dim_size(); j++) {
        if (j != 0) {
          std::cout << ", ";
        }
        if (shape.dim(j).has_dim_value()) {
          std::cout << shape.dim(j).dim_value();
        } else {
          std::cout << shape.dim(j).dim_param();
        }
      }
      std::cout << "> ";
    }
    std::cout << '%' << graph.input(i).name();
    if (i != graph.input_size() - 1) {
      std::cout << ", ";
    }
    std::cout << std::endl;
  }
  std::cout << ") {" << std::endl;

  for (int i = 0; i < graph.initializer_size(); i++) {
    const ::onnx::TensorProto &initializer = graph.initializer(i);
    std::cout << "  initialize input %" << initializer.name() << std::endl;
  }

  for (int i = 0; i < graph.node_size(); i++) {
    const ::onnx::NodeProto &node = graph.node(i);
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
      const ::onnx::AttributeProto &attribute = node.attribute(j);
      if (j != 0) {
        std::cout << ", ";
      }
      std::cout << attribute.name() << ":";
      switch (attribute.type()) {
      case ::onnx::AttributeProto::FLOAT:
        std::cout << attribute.f();
        break;
      case ::onnx::AttributeProto::INT:
        std::cout << attribute.i();
        break;
      case ::onnx::AttributeProto::STRING:
        std::cout << attribute.s();
        break;
      case ::onnx::AttributeProto::TENSOR:
      case ::onnx::AttributeProto::GRAPH:
        std::cout << attribute.name();
        break;
      case ::onnx::AttributeProto::FLOATS:
        std::cout << '[';
        for (int k = 0; k < attribute.floats_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.floats(k);
        }
        std::cout << ']';
        break;
      case ::onnx::AttributeProto::INTS:
        std::cout << '[';
        for (int k = 0; k < attribute.ints_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.ints(k);
        }
        std::cout << ']';
        break;
      case ::onnx::AttributeProto::STRINGS:
        std::cout << '[';
        for (int k = 0; k < attribute.strings_size(); k++) {
          if (k != 0) {
            std::cout << ", ";
          }
          std::cout << attribute.strings(k);
        }
        std::cout << ']';
        break;
      case ::onnx::AttributeProto::TENSORS:
      case ::onnx::AttributeProto::GRAPHS:
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

  // dump meata data
  for (int i = 0; i < model.metadata_props_size(); i++) {
     auto strStrEntry = model.metadata_props(i);
     std::cout << "key:" << strStrEntry.key() << std::endl;
     std::cout << "value:" << strStrEntry.value() << std::endl;
  }
}

class ONNCModulePrinter : public ModulePass
{
public:
  static char ID;
  ONNCModulePrinter() : ModulePass(ID) {}

  Pass::ReturnType runOnModule(Module &pModule) override
  {
    // FIXME because we use legacy dumpModelProto funtion
    //     translate graph/meta into ModelProto first
    auto graph = pModule.getGraph();
    ::onnx::ModelProto modelProto;
    ::onnx::ExportModelProto(&modelProto, graph);
    for (auto &metaData : pModule.getMetaData()) {
      auto *metadata_props = modelProto.add_metadata_props();
      metadata_props->set_key(metaData.first);
      metadata_props->set_value(metaData.second);
    }
    dumpModelProto(modelProto);
    return Pass::kModuleNoChanged;
  }
};

} // anonymous namespace

char ONNCModulePrinter::ID = 0;
ModulePass *onnc::createONNCModulePrinterPass()
{
  return new ONNCModulePrinter();
}
