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
//#include <onnx/onnx_pb.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// onnx::Reader
//===----------------------------------------------------------------------===//
onnx::Reader::Reader()
  : m_TotalBytesLimit(1024LL << 20), pWarningThreshold(512LL << 20) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

onnx::Reader::~Reader()
{
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();
}

bool onnx::Reader::parse(const Path& pFileName, Module& pModule)
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

    /**
    onnx::ModelProto model;
    coded_input.SetTotalBytesLimit(m_TotalBytesLimit, pWarningThreshold);
    if (!model.ParseFromCodedStream(&coded_input)) {
      // TODO: show the error message
      return false;
    }
    **/
  }

  err = file.close();
  return err.isGood();
}

bool onnx::Reader::parse(ConstBuffer pContent, Module& pModule)
{
  {
    ::google::protobuf::io::CodedInputStream
        coded_input((const uint8_t *)pContent.raw(), pContent.size());

    /**
    onnx::ModelProto model;
    coded_input.SetTotalBytesLimit(m_TotalBytesLimit, pWarningThreshold);
    if (!model.ParseFromCodedStream(&coded_input)) {
      // TODO: show the error message
      return false;
    }
    **/
  }
  return true;
}

void onnc::onnx::Reader::setTotalBytesLimit(int pTotalBytesLimit, int pWarningThreshold)
{
  m_TotalBytesLimit = pTotalBytesLimit;
  pWarningThreshold = pWarningThreshold;
}
