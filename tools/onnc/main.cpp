//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNCApp.h"
#include <onnc/Option/CommandLine.h>

using namespace onnc;

static const char* HelpManual =
  "Usage:\n"
  "\tonnc [input] -o [output]\n"
  "\n"
  "General Options:\n"
  "\t-o     <path>        Output file path\n"
  "\t-march=<arch>        Select backend <arch>\n"
  "\n"
  "\t-h | -? | --help Show this manual\n"
  "onnc version 0.1.0\n";

static cl::opt<Path> OptInput("input", cl::kPositional, cl::kRequired,
                              cl::kValueRequired,
                              cl::desc("The input file"), cl::help(HelpManual));

static cl::opt<std::string> OptOutput("o", cl::kShort, cl::kOptional,
                                      cl::kValueRequired,
                                      cl::desc("The output file"),
                                      cl::help(HelpManual));

static cl::opt<bool> OptHelp("help", cl::kLong, cl::kOptional,
                             cl::kValueDisallowed, cl::init(false),
                             cl::desc("Show this manual."));

static cl::alias HelpAliasH("h", cl::kShort, cl::trueopt(OptHelp));
static cl::alias HelpAliasQ("?", cl::kShort, cl::trueopt(OptHelp));

static cl::opt<std::string> OptQuadruple("mquadruple", cl::kShort, cl::kOptional,
    cl::kValueRequired, cl::desc("target quadruple"), cl::help(HelpManual));
    
static cl::opt<std::string> OptMArch("march", cl::kShort, cl::kOptional,
    cl::kValueRequired, cl::desc("target architecture"), cl::help(HelpManual));

int main(int pArgc, char* pArgv[])
{
  ONNCApp onnc(pArgc, pArgv);
  return onnc.compile();
}
