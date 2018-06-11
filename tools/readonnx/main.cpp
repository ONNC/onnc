//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ReadONNXApp.h"
#include <onnc/ADT/Color.h>
#include <onnc/Support/Host.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Option/CommandLine.h>
#include <onnc/Config/AboutData.h>

using namespace onnc;

static AboutData g_About("readonnx",
                         "readonnx",
                         "0.1.0",
                         AboutLicense::kPrivate,
                         "onnx reader");

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

//===----------------------------------------------------------------------===//
// Main Procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char* pArgv[])
{
  ReadONNXApp onnxreader(pArgc, pArgv);

  // --help
  if (OptHelp) {
    g_About.print(outs());
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
  onnxreader.options().setInput(OptInput);

  // check output
  if (OptOutput.hasOccurrence())
    onnxreader.options().setOutput(OptOutput);

  return onnxreader.display();
}
