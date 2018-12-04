//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// onnx2tg will generate assembly, weight and runtime file.

// If filename is “-” or omitted, onnx2tg reads from standard input. Otherwise,
// it will from filename. Inputs can be in onnx model(.onnx).

// If the -o option is omitted, then onnx2tg will send its output except weight
// to standard output if the input is from standard input. If the -o option
// specifies “-“, then the output except weight will also be sent to standard
// output.

// If no -o option is specified and an input file other than “-” is specified,
// then onnx2tg creates the output filename by taking the input filename,
// removing existing .onnx extension, and adding a .s(assembly),
// .weight.bin(weight) and .rt.json(runtime info) suffix for different outputs.

#include "ONNX2TGApp.h"
#include <iostream>
#include <onnc/ADT/Color.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Option/CommandLine.h>
#include <onnc/Support/Debug.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Path.h>

#ifdef BMONNC_EXIST
#include <bmnetc/foo.h>
#endif

#ifdef NDEBUG
#define ENABLE_DEBUG()
#endif

using namespace onnc;

static AboutData
    g_About("onnx2tg", "onnc2tg", "0.1.0", AboutLicense::kPrivate,
            "The onnx2tg command compiles ONNX models into "
            "assembly(.s), weight(.weight.bin) and runtime files(.rt.json)");

static cl::opt<std::string> InputFilename("input", cl::kPositional,
                                          cl::kOptional, cl::kValueRequired,
                                          cl::desc("input onnx model"),
                                          cl::init("-"), cl::about(g_About));

static cl::opt<std::string> OutputFilename(
    "o", cl::kShort, cl::kOptional, cl::kValueRequired,
    cl::desc("output file basename for .s, .weight.bin and .rt.json"),
    cl::about(g_About));

static cl::opt<std::string>
    march("march", cl::kShort, cl::kOptional, cl::kValueRequired,
          cl::desc("The march of Bimain TG [bm1680|bm1880]"),
          cl::init("bm1880"), cl::about(g_About));

static cl::opt<bool> printModuleBeforeISel("print-module-before-isel",
                                           cl::kShort, cl::kOptional,
                                           cl::kValueDisallowed,
                                           cl::init(false), cl::about(g_About));

static cl::opt<bool>
    IgnoreCalibrationStep("ignore-calibration-step", cl::kShort, cl::kOptional,
                          cl::kValueDisallowed, cl::init(false),
                          cl::desc("ignore ctable"), cl::about(g_About));

static cl::opt<bool> AddDummyCTable("add-dummy-ctable", cl::kShort,
                                    cl::kOptional, cl::kValueDisallowed,
                                    cl::init(false),
                                    cl::desc("add dummy ctable if not found"),
                                    cl::about(g_About));
static cl::opt<bool> AddDummyWeight("add-dummy-weight", cl::kShort,
                                    cl::kOptional, cl::kValueDisallowed,
                                    cl::init(false),
                                    cl::desc("add dummy weight if not found"),
                                    cl::about(g_About));

static cl::opt<bool> OptHelp("help", cl::kLong, cl::kOptional,
                             cl::kValueDisallowed, cl::init(false),
                             cl::desc("Show this manual."), cl::about(g_About));

static cl::alias HelpAliasH("h", cl::kShort, cl::trueopt(OptHelp),
                            cl::about(g_About));
static cl::alias HelpAliasQ("?", cl::kShort, cl::trueopt(OptHelp),
                            cl::about(g_About));

static cl::opt<std::string> OutputOptOnnx("opt-onnx", cl::kOptional,
                                          cl::kValueRequired,
                                          cl::desc("Ouput the optimized onnx"),
                                          cl::about(g_About));

//===----------------------------------------------------------------------===//
// Main procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char *pArgv[])
{
  ENABLE_DEBUG();
  InitializeAnalysisPassOptions();
  ONNX2TG onnx2tg(pArgc, pArgv);
  if (OptHelp) {
    g_About.print(std::cout, true);
    exit(0);
  }

  onnx2tg.options().setInput(InputFilename);
  std::string OFN = OutputFilename;
  if (OutputFilename.empty()) {
    StringRef IFN = InputFilename;
    if (IFN.endswith(".onnx"))
      OFN = IFN.drop_back(5);
    else
      OFN = IFN;
  }

  // set up output
  onnx2tg.options().setOutput(OFN);
  onnx2tg.options().setMarch(march);
  onnx2tg.options().target().printBeforeTensorSel(printModuleBeforeISel);
  onnx2tg.options().target().ignoreCalibrationStep(IgnoreCalibrationStep);
  onnx2tg.options().target().useDummyCTable(AddDummyCTable);
  onnx2tg.options().target().useDummyWeight(AddDummyWeight);
  onnx2tg.options().target().optOnnxModel(OutputOptOnnx);

#ifdef BMONNC_EXIST
  foo();
#endif

  return onnx2tg.compile();
}
