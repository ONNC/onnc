//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// onnx-dis IGNORES init data in model for readability
// output can be pass to onnx-as as a new  onnx.model binary without init data
#include <onnc/Option/CommandLine.h>
#include <onnc/Config/AboutData.h>
#include <onnc/Support/IOStream.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <onnx/common/ir_pb_converter.h>
#include <fstream>
#include <string>

using namespace onnc;
using namespace std;

//===----------------------------------------------------------------------===//
// Command Line Options
//===----------------------------------------------------------------------===//
static AboutData g_About("onnx-dis",
                         "onnx-dis",
                         "0.9",
                         AboutLicense::kUnknown,
                         ".onnx -> .onnx.s disassembler");

static cl::opt<std::string> InputFilename(cl::kPositional,
                                          cl::desc("<input .onnx.s file>"),
                                          cl::init("-"),
                                          cl::about(g_About));

static cl::opt<bool> DumpWeight("dump-weight",
                                cl::desc("Dump wegiht"),
                                cl::about(g_About));

static cl::opt<bool> OptHelp("help", cl::kLong, cl::desc("help"),
                             cl::about(g_About));

static cl::alias HelpAliasH("h", cl::kShort, cl::trueopt(OptHelp));
static cl::alias HelpAliasQ("?", cl::kShort, cl::trueopt(OptHelp));

//===----------------------------------------------------------------------===//
// Procedures
//===----------------------------------------------------------------------===//
template <class T> static void dumpRawTensor(const std::string &pRaw)
{
  vector<T> data(pRaw.size() / (sizeof(T) / sizeof(char)));
  memcpy(data.data(), pRaw.data(), pRaw.size());
  for (auto &i : data) {
    if (!std::is_same<T, float>::value)
      cout << (int)i << ", ";
    else
      cout << i << ", ";
  }
}

static void dumpNodeProto(
    const ::google::protobuf::RepeatedPtrField< ::onnx::NodeProto> &pNodes,
    set<string> &pReshapeInputs)
{
  int i;
  ::google::protobuf::RepeatedPtrField<const ::onnx::NodeProto>::iterator it;

  for (it = pNodes.begin(); it != pNodes.end(); it++) {
    cout << "  node { ";

    if (it->op_type() == "Reshape") {
      for (i = 0; i < it->input_size(); i++)
        pReshapeInputs.insert(it->input(i));
    }

    cout << it->ShortDebugString();
    cout << " }\n";
  }
}

static void dumpTensorProto(
    const ::google::protobuf::RepeatedPtrField< ::onnx::TensorProto> &pTensors,
    set<string> &pReshapeInputs)

{
  ::google::protobuf::RepeatedPtrField<const ::onnx::TensorProto>::iterator it;
  set<string>::iterator itReshape;

  for (it = pTensors.begin(); it != pTensors.end(); it++) {
    if (!DumpWeight) {
      itReshape = pReshapeInputs.find(it->name());
      if (itReshape == pReshapeInputs.end())
        continue;
    }

    cout << "  initializer { \n";
    cout << it->DebugString();
    cout << " }\n";
  }
}

static void dumpGraphProto(const ::onnx::GraphProto &pGraph)
{
  int i;
  set<string> reshapeInputs;

  cout << "graph {\n";
  cout << "  name: \"" << pGraph.name() << "\"\n";
  cout << "  doc_string: \"" << pGraph.doc_string() << "\"\n";

  dumpNodeProto(pGraph.node(), reshapeInputs);

  for (i = 0; i < pGraph.input_size(); i++)
    cout << "  input { " << pGraph.input(i).ShortDebugString() << " }\n";

  for (i = 0; i < pGraph.output_size(); i++)
    cout << "  output { " << pGraph.output(i).ShortDebugString() << " }\n";

  for (i = 0; i < pGraph.value_info_size(); i++)
    cout << "  value_info { " << pGraph.value_info(i).ShortDebugString()
         << " }\n";

  dumpTensorProto(pGraph.initializer(), reshapeInputs);

  cout << "}\n";
}

static void dumpModelProto(const ::onnx::ModelProto &pModel)
{
  int i;

  cout << "ir_version: " << pModel.ir_version() << "\n";
  cout << "producer_name: \"" << pModel.producer_name() << "\"\n";
  cout << "producer_version: \"" << pModel.producer_version() << "\"\n";
  cout << "domain: \"" << pModel.domain() << "\"\n";
  cout << "model_version: " << pModel.model_version() << "\n";
  cout << "doc_string: \"" << pModel.doc_string() << "\"\n";

  if (pModel.has_graph())
    dumpGraphProto(pModel.graph());

  for (i = 0; i < pModel.opset_import_size(); ++i) {
    cout << "opset_import { " << pModel.opset_import(i).ShortDebugString()
         << " }\n";
  }
  for (i = 0; i < pModel.metadata_props_size(); ++i) {
    cout << "metadata_props { " << pModel.metadata_props(i).ShortDebugString()
         << " }\n";
  }
}

//===----------------------------------------------------------------------===//
// Main function
//===----------------------------------------------------------------------===//
int main(int pArgc, char *pArgv[])
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  cl::ParseCommandLine(pArgc, pArgv);
  if (OptHelp) {
    g_About.print(outs());
    return EXIT_SUCCESS;
  }

  onnx::ModelProto model;
  string fileName(InputFilename);

  {
    std::ifstream input(fileName, std::ifstream::binary);
    ::google::protobuf::io::IstreamInputStream inputStream(&input);
    ::google::protobuf::io::CodedInputStream codeInputStream(&inputStream);
    codeInputStream.SetTotalBytesLimit(1024LL << 20, 512LL << 20);
    if (!model.ParseFromCodedStream(&codeInputStream)) {
      std::cerr << "Failed to parse onnx::ModelProto.\n";
      return EXIT_FAILURE;
    }
  }

  dumpModelProto(model);

  google::protobuf::ShutdownProtobufLibrary();

  return EXIT_SUCCESS;
}
