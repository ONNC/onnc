#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/MemoryBuffer.h"
#include <fstream>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <iostream>
#include <onnx/common/ir_pb_converter.h>
#include <string>

using namespace llvm;

static cl::opt<std::string> InputFilename(cl::Positional,
                                          cl::desc("<input .onnx.s file>"),
                                          cl::init("-"));

static cl::opt<std::string> OutputFilename("o",
                                           cl::desc("Override output filename"),
                                           cl::value_desc("filename"));
static ExitOnError ExitOnErr;

int main(int pArgc, char *pArgv[])
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  cl::ParseCommandLineOptions(pArgc, pArgv, "onnx.s -> .onnx assembler\n");

  ::onnx::ModelProto model;
  {
    std::unique_ptr<MemoryBuffer> MB = ExitOnErr(
        errorOrToExpected(MemoryBuffer::getFileOrSTDIN(InputFilename)));
    std::string prototxt = MB.get()->getBuffer().str();
    google::protobuf::TextFormat::ParseFromString(prototxt, &model);
  }

  if (OutputFilename.empty()) {
    std::string output;
    model.SerializeToString(&output);
    std::cout << output;
  } else {
    std::fstream output(OutputFilename,
                        std::ios::out | std::ios::trunc | std::ios::binary);
    model.SerializeToOstream(&output);
  }

  google::protobuf::ShutdownProtobufLibrary();
}
