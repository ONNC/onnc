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
#include <onnc/IR/ONNXUtils.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// onnx::Reader
//===----------------------------------------------------------------------===//
onnc::onnx::Reader::Reader()
  : m_TotalBytesLimit(1024LL << 20), pWarningThreshold(512LL << 20) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

onnc::onnx::Reader::~Reader()
{
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();
}

SystemError onnc::onnx::Reader::parse(const Path& pFileName, Module& pModule)
{
  FileHandle file;
  SystemError err = file.open(pFileName, FileHandle::kReadOnly);
  if (!err.isGood())
    return err;


  { // protobuf should be destroyed before the file being closed.
    ::google::protobuf::io::FileInputStream raw_input(file.handler());
    ::google::protobuf::io::CodedInputStream coded_input(&raw_input);

    ::onnx::ModelProto model;
    coded_input.SetTotalBytesLimit(m_TotalBytesLimit, pWarningThreshold);
    if (!model.ParseFromCodedStream(&coded_input)) {
      error(onnx_cannot_parsed) << pFileName;
      return SystemError::kUnknownError;
    }
    IRBuilder builder(pModule);
    builder.update(model);
  }

  err = file.close();
  if (!err.isGood())
    return err;
  return SystemError::kSuccess;
}

SystemError onnc::onnx::Reader::parse(ConstBuffer pContent, Module& pModule)
{
  {
    ::google::protobuf::io::ArrayInputStream input(
        (const uint8_t *)pContent.raw(), pContent.size());
    ::google::protobuf::io::CodedInputStream coded_input(&input);

    coded_input.SetTotalBytesLimit(m_TotalBytesLimit, pWarningThreshold);
    ::onnx::ModelProto model;
    if (!model.ParseFromCodedStream(&coded_input)) {
      error(onnx_cannot_parsed);
      return SystemError::kUnknownError;
    }
    IRBuilder builder(pModule);
    builder.update(model);
  }
  return SystemError::kSuccess;
}

void onnc::onnx::Reader::setTotalBytesLimit(int pTotalBytesLimit, int pWarningThreshold)
{
  m_TotalBytesLimit = pTotalBytesLimit;
  pWarningThreshold = pWarningThreshold;
}
