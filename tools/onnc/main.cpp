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

using namespace onnc;

static const char* HelpManual =
  "Usage:\n"
  "\tonnc [input] -o [output]\n"
  "\n"
  "General Options:\n"
  "\t-o     <path>        the output file path\n"
  "\t-mquadruple=<quad>   target a specific backend\n"
  "\t-march=<arch>        target a specific architecture\n"
  "\t-mcpu=<cpu>          target a specific CPU type\n"
  "\n"
  "\t-h | -? | --help Show this manual\n"
  "onnc version 0.1.0\n";

static cl::list<Path> OptInput("input", cl::kPositional,
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

//===----------------------------------------------------------------------===//
// Main Procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char* pArgv[])
{
  ONNCApp onnc(pArgc, pArgv);

  // check inputs
  if (OptInput.empty()) {
    errs() << Color::RED << "Error" << Color::RESET
           << ": no input" << std::endl;
    return EXIT_FAILURE;
  }

  cl::list<Path>::iterator input, iEnd = OptInput.end();
  for (input = OptInput.begin(); input != iEnd; ++input) {
    if (!exists(*input)) {
      errs() << Color::MAGENTA << "Fatal" << Color::RESET
             << ": input file not found: " << *input << std::endl;
      return EXIT_FAILURE;
    }
    if (!is_regular(*input)) {
      errs() << Color::MAGENTA << "Fatal" << Color::RESET
             << ": input file is not a regular file: " << *input << std::endl;
      return EXIT_FAILURE;
    }
    onnc.options().addInput(*input);
  }

  // check output
  if (OptOutput.hasOccurrence())
    onnc.options().setOutput(OptOutput);
  else
    onnc.options().setOutput(ONNCConfig::DefaultOutputName);

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

  return onnc.compile();
}
