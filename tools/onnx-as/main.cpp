//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Option/CommandLine.h>
#include <onnc/Config/AboutData.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/OFStream.h>
#include <onnc/Support/FileHandle.h>
#include <onnc/Support/FileDescriptor.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <onnx/common/ir_pb_converter.h>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Command Line Option
//===----------------------------------------------------------------------===//
static AboutData g_About("onnx-as",
                         "onnx-as",
                         "0.9",
                         AboutLicense::kPrivate,
                         "onnx.s -> .onnx assembler");

static cl::opt<std::string> InputFilename(cl::kPositional,
                                          cl::kOptional,
                                          cl::desc("<input .onnx.s file>"),
                                          cl::init("-"),
                                          cl::about(g_About));

static cl::opt<std::string> OutputFilename("o",
                                           cl::kOptional,
                                           cl::kValueRequired,
                                           cl::desc("Override output filename"),
                                           cl::value_desc("filename"),
                                           cl::about(g_About));

static cl::opt<bool> OptHelp("help", cl::kLong,
                             cl::desc("help"), cl::about(g_About));

static cl::alias HelpAliasH("h", cl::kShort, cl::trueopt(OptHelp));
static cl::alias HelpAliasQ("?", cl::kShort, cl::trueopt(OptHelp));

//===----------------------------------------------------------------------===//
// Main procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char *pArgv[])
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  cl::ParseCommandLine(pArgc, pArgv);
  if (OptHelp) {
    g_About.print(outs());
    return EXIT_SUCCESS;
  }

  ::onnx::ModelProto model;
  if (InputFilename.hasOccurrence() && InputFilename != "-") {
    // use file name
    FileHandle file;
    file.open(InputFilename, FileHandle::kReadOnly);
    ::google::protobuf::io::FileInputStream input(file.handler());
    if (!::google::protobuf::TextFormat::Parse(&input, &model)) {
      file.close();
      google::protobuf::ShutdownProtobufLibrary();
      return EXIT_FAILURE;
    }
  }
  else {
    // use stdin
    ::google::protobuf::io::FileInputStream input(STDIN_FILENO);
    if (!::google::protobuf::TextFormat::Parse(&input, &model)) {
      google::protobuf::ShutdownProtobufLibrary();
      return EXIT_FAILURE;
    }
  }

  if (OutputFilename.empty()) {
    std::string output;
    model.SerializeToString(&output);
    outs() << output;
  }
  else {
    OFStream output(OutputFilename,
                        std::ios::out | std::ios::trunc | std::ios::binary);
    model.SerializeToOstream(&output);
  }

  google::protobuf::ShutdownProtobufLibrary();
  return EXIT_SUCCESS;
}
