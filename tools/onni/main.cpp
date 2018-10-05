//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNIApp.h"
#include <onnc/ADT/Color.h>
#include <onnc/Support/Host.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Option/CommandLine.h>
#include <onnc/Config/AboutData.h>

using namespace onnc;

static AboutData g_About("onni",
                         "onni",
                         "0.1.0",
                         AboutLicense::kPrivate,
                         "[Experimental] ONNI is the interpreter of ONNC");

static cl::opt<Path> OptModel("model", cl::kPositional, cl::kOptional,
    cl::kValueRequired,
    cl::desc("The onnx model file"), cl::about(g_About));

static cl::opt<Path> OptInput("input", cl::kPositional, cl::kOptional,
    cl::kValueRequired,
    cl::desc("The input file"), cl::about(g_About));

static cl::opt<std::string> OptOutput("o", cl::kShort, cl::kOptional,
    cl::kValueRequired,
    cl::desc("The output file"),
    cl::about(g_About));

static cl::opt<bool> OptHelp("help", cl::kLong, cl::kOptional,
    cl::kValueDisallowed, cl::init(false),
    cl::desc("Show this manual."),
    cl::about(g_About));

static cl::alias HelpAliasH("h", cl::kShort, cl::trueopt(OptHelp));
static cl::alias HelpAliasQ("?", cl::kShort, cl::trueopt(OptHelp));

static cl::opt<unsigned int>
OptVerbose("verbose",
    cl::kLong,
    cl::kZeroOrMore,
    cl::kValueRequired,
    cl::kEqualSeparated,
    cl::desc("Set verbose level to <number> (default is 1)."),
    cl::init(1),
    cl::about(g_About));

static cl::opt<bool>
OptV("v", cl::kShort, cl::kZeroOrMore, cl::kValueDisallowed, cl::init(false),
    cl::desc("One -v increases one verbose level."),
    cl::about(g_About));

static cl::opt<bool>
OptQuiet("quiet", cl::kLong, cl::kOptional, cl::kValueDisallowed,
    cl::init(false),
    cl::desc("Set verbose level to 0."),
    cl::about(g_About));

static cl::opt<bool>
OptDryRun("dry-run", cl::kLong, cl::kOptional, cl::kValueDisallowed,
    cl::init(false),
    cl::desc("Do not do the inference, just print statistics."),
    cl::about(g_About));

// TODO: General way to enable passes
static cl::opt<bool>
OptOnnxOpt("onnx-opt", cl::kLong, cl::kOptional, cl::kValueDisallowed,
    cl::init(false),
    cl::desc("Enable onnx optimizer"),
    cl::about(g_About));

static cl::opt<std::string> OptQuadruple("mquadruple", cl::kShort, cl::kOptional,
    cl::kValueRequired, cl::desc("target quadruple"), cl::about(g_About));

static cl::opt<std::string> OptMArch("march", cl::kShort, cl::kOptional,
    cl::kValueRequired, cl::desc("target architecture"), cl::about(g_About));

//===----------------------------------------------------------------------===//
// Main Procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char* pArgv[])
{
  ONNIApp onni(pArgc, pArgv);

  // -verbose=level
  onni.options().setVerbose(OptVerbose);

  // -v
  if (OptV.hasOccurrence())
    onni.options().setVerbose(OptV.getNumOccurrence());

  // --quiet
  if (OptQuiet)
    onni.options().setVerbose(0);

  // --dry-run
  onni.options().setDryRun(OptDryRun);

  // --onnx-optimizer
  onni.options().setOnnxOpt(OptOnnxOpt);

  // --help
  if (OptHelp) {
    g_About.print(outs(), ONNIConfig::kNormal < onni.options().verbose());
    return EXIT_SUCCESS;
  }

  // check onnx model
  if (!exists(OptModel)) {
    errs() << Color::MAGENTA << "Fatal" << Color::RESET
           << ": onnx model file not found: " << OptModel << std::endl;
    return EXIT_FAILURE;
  }
  if (!is_regular(OptModel)) {
    errs() << Color::MAGENTA << "Fatal" << Color::RESET
           << ": onnx model file is not a regular file: "
           << OptModel << std::endl;
    return EXIT_FAILURE;
  }
  onni.options().setModel(OptModel);

  // set onnx input
  onni.options().setInput(OptInput);

  // check output
  if (OptOutput.hasOccurrence())
    onni.options().setOutput(OptOutput);
  else
    onni.options().setOutput(ONNIConfig::DefaultOutputName);

  // Set quadruple. We shall check target instance at compilation time.
  if (!OptQuadruple.hasOccurrence() && ! OptMArch.hasOccurrence()) {
    onni.options().setQuadruple(sys::GetHostQuadruple());
  }
  else {
    if (OptQuadruple.hasOccurrence())
      onni.options().setQuadruple(OptQuadruple);

    if (OptMArch.hasOccurrence())
      onni.options().setArchName(OptMArch);
  }

  return onni.run();
}
