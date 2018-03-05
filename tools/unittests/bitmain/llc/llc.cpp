//===-- llc.cpp - Implement the ONNC Native Code Generator for bm1680----------------===//


#include "TGBackend.h"
#include <fcntl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>
#include <onnx/onnx_pb.h>

int main(int argc, char* argv[]) {

  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    std::cerr << "usage:  " << argv[0] << " onnx_file" << std::endl;
    return -1;
  }

  onnx::ModelProto model;
  {
    int fd = open(argv[1], O_RDONLY);
    ::google::protobuf::io::CodedInputStream coded_stream(
        new ::google::protobuf::io::FileInputStream(fd));
    coded_stream.SetTotalBytesLimit(1024LL << 20, 512LL << 20);
    if (!model.ParseFromCodedStream(&coded_stream)) {
      std::cerr << "Failed to parse onnx file." << std::endl;
      return -1;
    }
  }

  TGBackend tgBackend(model);
  tgBackend.lowering().codeEmit();

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
