// Auto-generated
#include <iostream>
#include <sstream>
#include <unordered_map>

#include <fcntl.h>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <onnx/onnx_pb.h>

#include "oper.h"
#include "reader_helper.h"

using namespace std;

onnc::tensor::Operator *ConvertGraph(const onnx::GraphProto& graph) {
  cout << "graph " << graph.name() << " (";

  for (int i = 0; i < graph.input_size(); i++) {
    if (i != 0) { cout << ", "; }
    cout << '%' << graph.input(i).name();
  }
  cout << ") {" << endl;

  for (int i = 0; i < graph.initializer_size(); i++) {
    const onnx::TensorProto &initializer = graph.initializer(i);
    cout << "  initialize input %" << initializer.name() << " = data tensor<";
    for (int j = 0; j < initializer.dims_size(); j++) {
      if (j != 0) { cout << ", "; }
      cout << initializer.dims(j);
    }
    cout << '>' << endl;
  }

  for (int i = 0; i < graph.node_size(); i++) {
    const onnx::NodeProto &node = graph.node(i);
    cout << "  ";
    for (int j = 0; j < node.output_size(); j++) {
      if (j != 0) { cout << ", "; }
      cout << '%' << node.output(j);
    }
    cout << " = ";
    cout << node.op_type();
    if (node.attribute_size() != 0) {
        cout << '<';
    }
    for (int j = 0; j < node.attribute_size(); j++) {
      const onnx::AttributeProto &attribute = node.attribute(j);
      if (j != 0) { cout << ", "; }
      switch (attribute.type()) {
        case onnx::AttributeProto::FLOAT:
          cout << attribute.f();
          break;
        case onnx::AttributeProto::INT:
          cout << attribute.i();
          break;
        case onnx::AttributeProto::STRING:
          cout << attribute.s();
          break;
        case onnx::AttributeProto::TENSOR:
        case onnx::AttributeProto::GRAPH:
          cout << attribute.name();
          break;
        case onnx::AttributeProto::FLOATS:
          cout << '[';
          for (int k = 0; k < attribute.floats_size(); k++) {
            if (k != 0) { cout << ", "; }
            cout << attribute.floats(k);
          }
          cout << ']';
          break;
        case onnx::AttributeProto::INTS:
          cout << '[';
          for (int k = 0; k < attribute.ints_size(); k++) {
            if (k != 0) { cout << ", "; }
            cout << attribute.ints(k);
          }
          cout << ']';
          break;
        case onnx::AttributeProto::STRINGS:
          cout << '[';
          for (int k = 0; k < attribute.strings_size(); k++) {
            if (k != 0) { cout << ", "; }
            cout << attribute.strings(k);
          }
          cout << ']';
          break;
        case onnx::AttributeProto::TENSORS:
        case onnx::AttributeProto::GRAPHS:
          cout << attribute.name();
          break;
        default:
          cout << "unknown";
      }
    }
    if (node.attribute_size() != 0) {
        cout << '>';
    }
    cout << '(';
    for (int j = 0; j < node.input_size(); j++) {
      if (j != 0) { cout << ", "; }
      cout << '%' << node.input(j);
    }
    cout << ')' << endl;
  }

    cout << "  return ";
  // unordered_map<std::string, std::string> output_type;
  for (int i = 0; i < graph.output_size(); i++) {
   if (i != 0) { cout << ", "; }
   cout << ConvertTypeToString(graph.output(i).type()) << " %" << graph.output(i).name();
  }
  cout << endl;
  cout << '}' << endl;
}
onnc::tensor::Operator *ConvertModel(const onnx::ModelProto &model) {
  if (model.has_ir_version()) { cout << "model.ir_version: " << model.ir_version() << endl; }
  // opset_import
  if (model.has_producer_name()) { cout << "model.producer_name: " << model.producer_name() << endl; }
  if (model.has_producer_version()) { cout << "model.producer_version: " << model.producer_version() << endl; }
  if (model.has_domain()) { cout << "model.domain: " << model.domain() << endl; }
  if (model.has_model_version()) { cout << "model.model_version: " << model.model_version() << endl; }
  if (model.has_doc_string()) { cout << "model.doc_string: " << model.doc_string() << endl; }
  // metadata_props
  if (model.has_graph()) {
    return ConvertGraph(model.graph());
  } else {
    return nullptr;
  }
}

int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " ONNX_FILE" << endl;
    return -1;
  }

  onnx::ModelProto model;

  {
    int fd = open(argv[1], O_RDONLY);
    ::google::protobuf::io::CodedInputStream coded_stream(
      new ::google::protobuf::io::FileInputStream(fd));
    coded_stream.SetTotalBytesLimit(1024LL << 20, 512LL << 20);
    if (!model.ParseFromCodedStream(&coded_stream)) {
      cerr << "Failed to parse onnx file." << endl;
      return -1;
    }
  }

  onnc::tensor::Operator* net;

  net = ConvertModel(model);


  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

