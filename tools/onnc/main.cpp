//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNCApp.h"
#include <onnc/ADT/Color.h>
#include <onnc/Support/Host.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Option/CommandLine.h>
#include <onnc/Config/AboutData.h>
#include <onnc/Diagnostic/StreamLog.h>

using namespace onnc;

static AboutData g_About("onnc",
                         "onnc",
                         "0.1.0",
                         AboutLicense::kPrivate,
                         "ONNC is the compiler driver");

static cl::opt<Path> OptInput("input", cl::kPositional, cl::kOptional,
    cl::kValueRequired,
    cl::desc("The input file"), cl::about(g_About));

static cl::opt<std::string> OptOutput("o", cl::kShort, cl::kOptional,
    cl::kValueRequired,
    cl::desc("The output file"),
    cl::about(g_About));

static cl::opt<std::string> OptResource("r", cl::kShort, cl::kOptional,
    cl::kValueRequired,
    cl::desc("The resource directory"),
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

static cl::opt<std::string> OptQuadruple("mquadruple", cl::kShort, cl::kOptional,
    cl::kValueRequired, cl::desc("target quadruple"), cl::about(g_About));

static cl::opt<std::string> OptMArch("march", cl::kShort, cl::kOptional,
    cl::kValueRequired, cl::desc("target architecture"), cl::about(g_About));

static cl::opt<bool>
OptNvSmall("nv_small", cl::kLong, cl::kOptional, cl::kValueDisallowed,
    cl::init(false),
    cl::desc("Compile With nv_small Config Set (NVDLA Backend)"),
    cl::about(g_About));

static cl::opt<bool>
OptNvImage("nv_image", cl::kLong, cl::kOptional, cl::kValueDisallowed,
    cl::init(false),
    cl::desc("Compile As Image Execution Mode Loadable (NVDLA Backend)"),
    cl::about(g_About));

static cl::opt<bool>
OptNvLayerFusion("nv_layer_fusion", cl::kLong, cl::kOptional, cl::kValueDisallowed,
    cl::init(false),
    cl::desc("Enable Layer Fusion During Compiling (NVDLA Backend)"),
    cl::about(g_About));

//===----------------------------------------------------------------------===//
// Main Procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char* pArgv[])
{
  ONNCApp onnc(pArgc, pArgv);

  // -verbose=level
  onnc.options().setVerbose(OptVerbose);

  // -v
  if (OptV.hasOccurrence())
    onnc.options().setVerbose(OptV.getNumOccurrence());

  // --quiet
  if (OptQuiet)
    onnc.options().setVerbose(0);

  // --help
  if (OptHelp) {
    g_About.print(outs(), ONNCConfig::kNormal < onnc.options().verbose());
    return EXIT_SUCCESS;
  }

  // check inputs
  if (!exists(OptInput)) {
    errs() << Color::MAGENTA << "Fatal" << Color::RESET
           << ": input file not found: " << OptInput << std::endl;
    return EXIT_FAILURE;
  }
  if (!is_regular(OptInput)) {
    errs() << Color::MAGENTA << "Fatal" << Color::RESET
           << ": input file is not a regular file: " << OptInput << std::endl;
    return EXIT_FAILURE;
  }
  onnc.options().setInput(OptInput);

  // check output
  if (OptOutput.hasOccurrence()) {
    outs() << Color::GREEN << "Info" << Color::RESET
           << ": set the output file at: "
           << Color::Bold(Color::GREEN) << OptOutput << Color::RESET
           << std::endl;
    onnc.options().setOutput(Path(OptOutput));
  } else {
    errs() << Color::YELLOW << "Warning" << Color::RESET
           << ": Use the default path for the output file: "
           << Color::Bold(Color::GREEN)
           << ONNCConfig::DefaultOutputName
           << Color::RESET << std::endl;
    onnc.options().setOutput(Path(ONNCConfig::DefaultOutputName));
  }

  if (OptResource.hasOccurrence()) {
    onnc.options().target().setResourceDirectory(Path{OptResource});
  }

  // Set quadruple. We shall check target instance at compilation time.
  if (!OptQuadruple.hasOccurrence() && ! OptMArch.hasOccurrence()) {
    onnc.options().setQuadruple(sys::GetHostQuadruple());
  }
  else {
    if (OptQuadruple.hasOccurrence())
      onnc.options().setQuadruple(OptQuadruple);

    if (OptMArch.hasOccurrence())
      onnc.options().setArchName(OptMArch);
  }

  // handle target options (used by NVDLA Backend for now)
  if (OptNvSmall.hasOccurrence()) {
    onnc.options().target().setNvDlaConfigSet(TargetOptions::NvDlaConfigSet::nv_small);
  }
  if (OptNvImage.hasOccurrence()) {
    onnc.options().target().setNvDlaExecutionMode(TargetOptions::NvDlaExecutionMode::image);
  }
  if (OptNvLayerFusion.hasOccurrence()) {
    onnc.options().target().enableLayerFusion();
  }

  return onnc.compile();
}
