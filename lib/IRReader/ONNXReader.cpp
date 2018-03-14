//===-- ONNXReader.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/Support/FileHandle.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnx/common/ir_pb_converter.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// onnx::Reader
//===----------------------------------------------------------------------===//
onnc::onnx::Reader::Reader()
{
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

onnc::onnx::Reader::~Reader()
{
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();
}

bool onnc::onnx::Reader::parse(const Path& pFileName, Module& pModule)
{
  FileHandle file;
  SystemError err = file.open(pFileName, FileHandle::kReadOnly);
  if (!err.isGood()) {
    // TODO: show the error message
    return false;
  }


  { // protobuf should be destroyed before the file being closed.
    ::google::protobuf::io::FileInputStream raw_input(file.handler());
    ::google::protobuf::io::CodedInputStream coded_input(&raw_input);
    coded_input.SetTotalBytesLimit(1024LL << 20, 512LL << 20);
    ::onnx::ModelProto model;
    if (!model.ParseFromCodedStream(&coded_input)) {
      error(onnx_cannot_parsed) << pFileName;
      return false;
    }
    pModule.delegateGraph(::onnx::ImportModelProto(model));
  }

  err = file.close();
  return err.isGood();
}

bool onnc::onnx::Reader::parse(ConstBuffer pContent, Module& pModule)
{
  {
    ::google::protobuf::io::CodedInputStream
        coded_input((const uint8_t *)pContent.raw(), pContent.size());

    /**
    onnx::ModelProto model;
    if (!model.ParseFromCodedStream(&coded_input)) {
      // TODO: show the error message
      return false;
    }
    **/
  }
  return true;
}
