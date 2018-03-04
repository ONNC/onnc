//===-- ONNXReader.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IRReader/ONNXReader.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// onnx::Reader
//===----------------------------------------------------------------------===//
onnx::Reader::Reader()
{
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

onnx::Reader::~Reader()
{
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();
}

SystemError onnx::Reader::parse(const Path& pFileName, Module& pModule)
{
}

SystemError onnx::Reader::parse(StringRef pContent, Module& pModule)
{
}
