//===- Expansion.inc ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <wordexp.h>

namespace onnc {

class ExpansionImpl
{
public:
  ExpansionImpl(StringRef pWord);

  ~ExpansionImpl();

  unsigned int size() const { return m_Expansion.we_wordc; }

  void expand(StringList& pWords);

  Expansion::ErrorType error() const;

private:
  wordexp_t m_Expansion;
  int m_Error;
};

//===----------------------------------------------------------------------===//
// Implementation
//===----------------------------------------------------------------------===//
ExpansionImpl::ExpansionImpl(StringRef pWord)
{
  m_Error = wordexp(pWord.c_str(), &m_Expansion, 0);
}

ExpansionImpl::~ExpansionImpl()
{
  wordfree(&m_Expansion);
}

void ExpansionImpl::expand(StringList& pWords)
{
  for (unsigned int i=0; i < size(); ++i) {
    StringRef word(m_Expansion.we_wordv[i]);
    pWords.push_back(word);
  }
}

Expansion::ErrorType ExpansionImpl::error() const
{
  switch (m_Error) {
    case 0:
      return Expansion::kSuccess;
    case WRDE_BADCHAR:
      return Expansion::kBadChar;
    case WRDE_NOSPACE:
      return Expansion::kNoSpace;
    case WRDE_SYNTAX:
      return Expansion::kSyntaxErr;
    case WRDE_BADVAL:
    case WRDE_CMDSUB:
    default:
      return Expansion::kUnknown;
  }
}

} // namespace of onnc
