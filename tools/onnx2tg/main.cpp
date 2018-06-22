//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNX2TGApp.h"
#include <iostream>
#include <onnc/ADT/Color.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Option/CommandLine.h>
#include <onnc/Support/Debug.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Path.h>

using namespace onnc;

static const char *HelpManual = "Usage:\n"
                                "\tonnx2tg [input] -o [output]\n"
                                "\n"
                                "General Options:\n"
                                "\t-o <path>        Output file path\n"
                                "\n"
                                "\t-march [bm1680|bm1880]        \n"
                                "\n"
                                "\t-print-module-before-isel      \n"
                                "\n"
                                "\t-print-machineinstrs           \n"
                                "\n"
                                "\t-ignore-calibration-step       \n"
                                "\n"
                                "\t-h | -? | --help Show this manual\n"
                                "onnx2tg version 0.1.0\n";

static cl::opt<std::string> OptInput("input", cl::kPositional, cl::kOptional,
                                     cl::kValueRequired,
                                     cl::desc("The input file"),
                                     cl::help(HelpManual), cl::init("-"));

static cl::opt<std::string> OptOutput("o", cl::kShort, cl::kOptional,
                                      cl::kValueRequired,
                                      cl::desc("The output file"),
                                      cl::help(HelpManual),
                                      cl::init("cmdbuf.bin"));

static cl::opt<std::string> march("march", cl::kShort, cl::kOptional,
                                  cl::kValueRequired,
                                  cl::desc("The march of TG [bm1680|bm1880]"),
                                  cl::help(HelpManual), cl::init("bm1880"));

static cl::opt<bool>
    PrintMachineCode("print-machineinstrs", cl::kShort, cl::kOptional,
                     cl::kValueDisallowed, cl::init(false),
                     cl::desc("Print generated machine code for debugging)"));

static cl::opt<bool> printModuleBeforeISel("print-module-before-isel",
                                           cl::kShort, cl::kOptional,
                                           cl::kValueDisallowed,
                                           cl::init(false));

static cl::opt<bool> IgnoreCalibrationStep("ignore-calibration-step",
                                           cl::kShort, cl::kOptional,
                                           cl::kValueDisallowed,
                                           cl::init(false),
                                           cl::desc("ignore ctable)"));

static cl::opt<bool> OptHelp("help", cl::kLong, cl::kOptional,
                             cl::kValueDisallowed, cl::init(false),
                             cl::desc("Show this manual."));

static cl::alias HelpAliasH("h", cl::kShort, cl::trueopt(OptHelp));
static cl::alias HelpAliasQ("?", cl::kShort, cl::trueopt(OptHelp));

//===----------------------------------------------------------------------===//
// Main procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char *pArgv[])
{
  ENABLE_DEBUG();
  InitializeAnalysisPassOptions();
  ONNX2TG onnx2tg(pArgc, pArgv);
  if (OptHelp) {
    std::cout << HelpManual;
    exit(0);
  }

  onnx2tg.options().setInput(OptInput);

  // set up output
  if (OptOutput.hasOccurrence())
    onnx2tg.options().setOutput(OptOutput);

  onnx2tg.options().setMarch(march);
  onnx2tg.options().setPrintModuleBeforeISel(printModuleBeforeISel);
  onnx2tg.options().setPrintMachineCode(PrintMachineCode);
  onnx2tg.options().setIgnoreCalibrationStep(IgnoreCalibrationStep);

  return onnx2tg.compile();
}
