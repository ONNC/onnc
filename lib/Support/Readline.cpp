//===- Readline.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Readline.h>
#include <onnc/Support/Directory.h>
#include <onnc/Support/Path.h>
#include <onnc/ADT/StringRef.h>
#include "linenoise.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static inline void completion(const char *buf, linenoiseCompletions *lc)
{
  Path current;
  StringRef line(buf);
  if (!line.empty() && Path::separator == line.back())
    current = line.c_str();
  else
    current = ".";

  Directory directory(current);
  Directory::const_iterator entry, eEnd = directory.end();
  for (entry = directory.begin(); entry != eEnd; entry.next()) {
    if (line.empty())
      linenoiseAddCompletion(lc, entry.fileInfo().path().c_str());
    else {
      Path filepath(current);
      filepath.append(entry.fileInfo().path());
      linenoiseAddCompletion(lc, filepath.c_str());
    }
  }
  directory.close();
}

//===----------------------------------------------------------------------===//
// Readline
//===----------------------------------------------------------------------===//
Readline::Readline()
  : m_Prompt(), m_bGood(false) {
  linenoiseSetCompletionCallback(completion);
}

Readline::Readline(const std::string& pPrompt)
  : m_Prompt(pPrompt), m_bGood(false) {
  linenoiseSetCompletionCallback(completion);
}

Readline::~Readline()
{
}

/// Add a string in the tab completion
Readline& Readline::complete(const std::string& pValue)
{
  // TODO
  return *this;
}

/// Read a line
Readline& Readline::read(std::istream& pIns, std::string& pLine)
{
  char* line = nullptr;
  if (nullptr != (line = linenoise(m_Prompt.c_str()))) {
    m_bGood = true;
    pLine = line;
    ::free(line);
  }
  else
    m_bGood = false;
  return *this;
}
