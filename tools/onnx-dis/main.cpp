// NOTE: this disassembler is more convenient than proto's DebugString()
// it will IGNORE init data in model for readability
// output can be pass to onnx-as as a new  onnx.model binary without init data
#include "llvm/Support/CommandLine.h"
#include <fstream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>
#include <onnx/common/ir_pb_converter.h>
#include <string>
using namespace llvm;

static cl::opt<std::string> InputFilename(cl::Positional,
                                          cl::desc("<input .onnx.s file>"),
                                          cl::init("-"));

static cl::opt<bool> DumpWeight("dump-weight", cl::desc("Dump wegiht"));

template <class T> static void dumpRawTensor(const std::string &raw)
{
  std::vector<T> data(raw.size() / (sizeof(T) / sizeof(char)));
  std::memcpy(data.data(), raw.data(), raw.size());
  for (auto &i : data) {
    if (!std::is_same<T, float>::value)
      std::cout << (int)i << ", ";
    else
      std::cout << i << ", ";
  }
}

static void dumpGraphProto(const ::onnx::GraphProto &pGraph)
{
  std::cout << "graph {\n";
  if (pGraph.has_name())
    std::cout << "  name: \"" << pGraph.name() << "\"\n";
  if (pGraph.has_doc_string())
    std::cout << "  doc_string: \"" << pGraph.doc_string() << "\"\n";
  for (int i = 0; i < pGraph.node_size(); ++i) {
    std::cout << "  node { " << pGraph.node(i).ShortDebugString() << " }\n";
  }
  for (int i = 0; i < pGraph.input_size(); ++i) {
    std::cout << "  input { " << pGraph.input(i).ShortDebugString() << " }\n";
  }
  for (int i = 0; i < pGraph.output_size(); ++i) {
    std::cout << "  output { " << pGraph.output(i).ShortDebugString() << " }\n";
  }
  for (int i = 0; i < pGraph.value_info_size(); ++i) {
    std::cout << "  value_info { " << pGraph.value_info(i).ShortDebugString()
              << " }\n";
  }
  std::cout << "}\n";
}

int main(int pArgc, char *pArgv[])
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  cl::ParseCommandLineOptions(pArgc, pArgv, ".onnx -> .onnx.s disassembler\n");

  ::onnx::ModelProto model;
  std::string fileName(InputFilename);
  {
    std::ifstream input(fileName, std::ifstream::binary);
    ::google::protobuf::io::IstreamInputStream inputStream(&input);
    ::google::protobuf::io::CodedInputStream codeInputStream(&inputStream);
    codeInputStream.SetTotalBytesLimit(1024LL << 20, 512LL << 20);
    if (!model.ParseFromCodedStream(&codeInputStream)) {
      std::cerr << "Failed to parse onnx::ModelProto.\n";
      return -1;
    }
  }
  if (model.has_ir_version())
    std::cout << "ir_version: " << model.ir_version() << "\n";
  if (model.has_producer_name())
    std::cout << "producer_name: \"" << model.producer_name() << "\"\n";
  if (model.has_producer_version())
    std::cout << "producer_version: \"" << model.producer_version() << "\"\n";
  if (model.has_domain())
    std::cout << "domain: \"" << model.domain() << "\"\n";
  if (model.has_model_version())
    std::cout << "model_version: " << model.model_version() << "\n";
  if (model.has_doc_string())
    std::cout << "doc_string: \"" << model.doc_string() << "\"\n";
  dumpGraphProto(model.graph());
  for (int i = 0; i < model.opset_import_size(); ++i) {
    std::cout << "opset_import { " << model.opset_import(i).ShortDebugString()
              << " }\n";
  }
  for (int i = 0; i < model.metadata_props_size(); ++i) {
    std::cout << "metadata_props { "
              << model.metadata_props(i).ShortDebugString() << " }\n";
  }

  if (model.has_graph() && DumpWeight) {
    onnx::GraphProto *graph = model.mutable_graph();
    for (int i = 0; i < graph->initializer_size(); ++i) {
      const auto &tp = graph->initializer(i);
      if (!tp.has_raw_data()) {
        tp.DebugString();
        continue;
      }
      std::cout << "tensor_pros { ";
      if (tp.has_name()) {
        std::cout << "name: " << tp.name() << " data: ";
      }
      switch (tp.data_type()) {
      case onnx::TensorProto_DataType_FLOAT:
        dumpRawTensor<float>(tp.raw_data());
        break;
      case onnx::TensorProto_DataType_INT8:
        dumpRawTensor<int8_t>(tp.raw_data());
        break;
      case onnx::TensorProto_DataType_INT16:
        dumpRawTensor<int16_t>(tp.raw_data());
        break;
      default:
        std::cout << "unsupported type!\n";
        break;
      }
      std::cout << " }\n";
    }
  }

  google::protobuf::ShutdownProtobufLibrary();
}
