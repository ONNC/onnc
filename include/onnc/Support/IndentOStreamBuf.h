//===- BasicIndentOStreamBuf.h --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_INDENT_OSTREAM_BUFFER_H
#define ONNC_SUPPORT_INDENT_OSTREAM_BUFFER_H
#include <streambuf>
#include <string>

namespace onnc {

template<typename CharT, typename Traits>
class BasicIndentOStreamBuf : public std::basic_streambuf<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef typename traits_type::int_type    int_type;
  typedef typename traits_type::pos_type    pos_type;
  typedef typename traits_type::off_type    off_type;

  typedef std::basic_streambuf<char_type, traits_type> streambuf_type;

public:
  /// Typical constructor. If we can get the destined streambuf, then use this
  /// constructor
  explicit BasicIndentOStreamBuf(streambuf_type* pDestBuf,
                                 unsigned int pIndent = 4, CharT pFill = ' ');

  ~BasicIndentOStreamBuf();

protected:
  virtual int_type overflow(int_type pC = traits_type::eof());

protected:
  streambuf_type* m_pDestBuf;
  bool m_bIsAtStartOfLine;
};

namespace indentbuf {

void Indent();

void Deindent();

void SetIndent(unsigned int pIndent);

unsigned int GetIndent();

void ClearIndent();

void SetFill(char pFill);

char GetFill();

} // namespace of indentbuf
} // namespace of onnc

#include "Bits/IndentOStreamBuf.tcc"

#endif
