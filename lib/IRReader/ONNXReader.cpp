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
// Non-member functions
//===----------------------------------------------------------------------===//
static inline bool
DoParse(Module& pModule, ::google::protobuf::io::ZeroCopyInputStream& pStream,
        int pTotalBytesLimit, int pWarningThreshold)
{
  ::google::protobuf::io::CodedInputStream coded_input(&pStream);

  coded_input.SetTotalBytesLimit(pTotalBytesLimit, pWarningThreshold);
  ::onnx::ModelProto model;
  if (!model.ParseFromCodedStream(&coded_input)) {
    return false;
  }
  IRBuilder builder(pModule);
  builder.update(model);
  return true;
}

//===----------------------------------------------------------------------===//
// onnx::Reader
//===----------------------------------------------------------------------===//
onnc::onnx::Reader::Reader()
  : m_TotalBytesLimit(1024LL << 20), m_WarningThreshold(512LL << 20) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

onnc::onnx::Reader::~Reader()
{
  // leave ShutdownProtobufLibrary() to CoreApplication's destructor.
}

void onnc::onnx::Reader::ShutdownProtobufLibrary()
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

  err = parse(file.handler(), pModule);
  if (!err.isGood()) {
    error(onnx_cannot_parsed) << pFileName;
    return err;
  }

  err = file.close();
  if (!err.isGood())
    return err;

  return SystemError::kSuccess;
}

SystemError onnc::onnx::Reader::parse(ConstBuffer pContent, Module& pModule)
{
  ::google::protobuf::io::ArrayInputStream input(
      (const uint8_t *)pContent.raw(), pContent.size());
  if (!DoParse(pModule, input, m_TotalBytesLimit, m_WarningThreshold))
    return SystemError::kUnknownError;
  return SystemError::kSuccess;
}

SystemError onnc::onnx::Reader::parse(int pFD, Module& pModule)
{
  ::google::protobuf::io::FileInputStream input(pFD);
  if (!DoParse(pModule, input, m_TotalBytesLimit, m_WarningThreshold))
    return SystemError::kUnknownError;
  return SystemError::kSuccess;
}

void onnc::onnx::Reader::setTotalBytesLimit(int pTotalBytesLimit, int pWarningThreshold)
{
  m_TotalBytesLimit = pTotalBytesLimit;
  m_WarningThreshold = pWarningThreshold;
}
