//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNX2TGApp.h"
#include <onnc/Option/CommandLine.h>
#include <onnc/Support/Path.h>

using namespace onnc;

static const char* HelpManual =
  "Usage:\n"
  "\tonnx2tg [input] -o [output]\n"
  "\n"
  "General Options:\n"
  "\t-o <path>        Output file path\n"
  "\n"
  "\t-h | -? | --help Show this manual\n"
  "onnx2tg version 0.1.0\n";

static cl::opt<Path> OptInput("input", cl::kPositional, cl::kRequired,
                              cl::kValueRequired,
                              cl::desc("The input file"), cl::help(HelpManual));

static cl::opt<Path> OptOutput("o", cl::kShort, cl::kOptional,
                               cl::kValueRequired, cl::desc("The output file"),
                               cl::help(HelpManual));

static cl::opt<bool> OptHelp("help", cl::kLong, cl::kOptional,
                             cl::kValueDisallowed, cl::init(false),
                             cl::desc("Show this manual."));

static cl::alias HelpAliasH("h", cl::kShort, cl::trueopt(OptHelp));
static cl::alias HelpAliasQ("?", cl::kShort, cl::trueopt(OptHelp));

//===----------------------------------------------------------------------===//
// Main procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char* pArgv[])
{
  ONNX2TG onnx2tg(pArgc, pArgv);
  return onnx2tg.compile();
}
