//===- IndentOStreamBuf.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/IndentOStreamBuf.h>

namespace {

static unsigned int g_Level = 0;
static unsigned int g_Indent = 4;
static char g_Fill = ' ';

} // anonymous namespace

//===----------------------------------------------------------------------===//
// IndentOStreamBuf
//===----------------------------------------------------------------------===//
void onnc::indentbuf::Indent()
{
  ++g_Level;
}

void onnc::indentbuf::Deindent()
{
  if (g_Level > 0)
    --g_Level;
}

void onnc::indentbuf::SetIndent(unsigned int pIndent)
{
  g_Indent = pIndent;
}

unsigned int onnc::indentbuf::GetIndent()
{
  return (g_Indent * g_Level);
}

void onnc::indentbuf::ClearIndent()
{
  g_Level = 0;
  g_Indent = 4;
  g_Fill = ' ';
}

void onnc::indentbuf::SetFill(char pFill)
{
  g_Fill = pFill;
}

char onnc::indentbuf::GetFill()
{
  return g_Fill;
}
