#include <fstream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>
#include <onnx/common/ir_pb_converter.h>
#include <string>

int main(int pArgc, char *pArgv[])
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (pArgc != 2) {
    std::cerr << "usage:  " << pArgv[0] << " onnx_file\n";
    return EXIT_FAILURE;
  }

  ::onnx::ModelProto model;
  std::string fileName(pArgv[1]);
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

  if (model.has_graph()) {
    onnx::GraphProto *graph = model.mutable_graph();
    graph->clear_initializer();
  }
  std::cout << model.DebugString() << "\n";

  google::protobuf::ShutdownProtobufLibrary();
}
