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

#ifdef BMNETC_EXIST
#include <bmnetc/foo.h>
#endif

using namespace onnc;

static AboutData g_About("onnx2tg", "onnc2tg", "0.1.0", AboutLicense::kPrivate,
                         "ONNC is the compiler driver");

static cl::opt<std::string> OptInput("input", cl::kPositional, cl::kOptional,
                                     cl::kValueRequired,
                                     cl::desc("The input file"), cl::init("-"),
                                     cl::about(g_About));

static cl::opt<std::string> OptOutput("o", cl::kShort, cl::kOptional,
                                      cl::kValueRequired,
                                      cl::desc("The output file"),
                                      cl::init("cmdbuf.bin"),
                                      cl::about(g_About));

static cl::opt<std::string> march("march", cl::kShort, cl::kOptional,
                                  cl::kValueRequired,
                                  cl::desc("The march of TG [bm1680|bm1880]"),
                                  cl::init("bm1880"), cl::about(g_About));

static cl::opt<bool>
    dumpASM("S", cl::kShort, cl::kOptional, cl::kValueDisallowed,
            cl::init(false),
            cl::desc("Print ASM(generated machine code) for debugging"),
            cl::about(g_About));

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

static cl::opt<bool> OptHelp("help", cl::kLong, cl::kOptional,
                             cl::kValueDisallowed, cl::init(false),
                             cl::desc("Show this manual."), cl::about(g_About));

static cl::alias HelpAliasH("h", cl::kShort, cl::trueopt(OptHelp),
                            cl::about(g_About));
static cl::alias HelpAliasQ("?", cl::kShort, cl::trueopt(OptHelp),
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

  onnx2tg.options().setInput(OptInput);

  // set up output
  if (OptOutput.hasOccurrence())
    onnx2tg.options().setOutput(OptOutput);

  onnx2tg.options().setMarch(march);
  onnx2tg.options().setPrintModuleBeforeISel(printModuleBeforeISel);
  onnx2tg.options().setDumpASM(dumpASM);
  onnx2tg.options().setIgnoreCalibrationStep(IgnoreCalibrationStep);
  onnx2tg.options().setAddDummyCTable(AddDummyCTable);

#ifdef BMNETC_EXIST
  foo();
#endif

  return onnx2tg.compile();
}
