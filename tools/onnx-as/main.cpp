#include <fstream>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <iostream>
#include <onnx/common/ir_pb_converter.h>
#include <string>

int main(int pArgc, char *pArgv[])
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (pArgc != 2) {
    std::cerr << "usage:  " << pArgv[0] << " onnx_file\n";
    return -1;
  }

  ::onnx::ModelProto model;
  std::string fileName(pArgv[1]);
  {
    std::fstream input(fileName, std::ios::in | std::ios::binary);
    google::protobuf::io::IstreamInputStream iis(&input);
    google::protobuf::TextFormat::Parse(&iis, &model);
  }

  std::string output;
  model.SerializeToString(&output);
  std::cout << output;

  google::protobuf::ShutdownProtobufLibrary();
}
