//===- BasicIndentOStreamBuf.h --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_INDENT_OSTREAM_H
#define ONNC_SUPPORT_INDENT_OSTREAM_H
#include <onnc/Support/IOSFwd.h>
#include <onnc/Support/IndentOStreamBuf.h>
#include <ostream>

namespace onnc {

template<typename CharT, typename Traits>
class BasicIndentOStream : public std::basic_ostream<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  // Non-standard types:
  typedef std::basic_ostream<CharT, Traits> ostream_type;
  typedef BasicIndentOStreamBuf<CharT, Traits> streambuf_type;

private:
  typedef ostream_type BaseStream;

public:
  explicit BasicIndentOStream(ostream_type& pOS,
                              unsigned int pIndent = 4,
                              char_type pFill = ' ')
    : BaseStream(), m_Buf(pOS.rdbuf(), pIndent, pFill) {
    this->init(&m_Buf);
  }

  template<typename C, typename T>
  friend ostream_type& indent(ostream_type& pOS);

  template<typename C, typename T>
  friend ostream_type& unindent(ostream_type& pOS);

private:
  streambuf_type m_Buf;
};

} // namespace of onnc

namespace std {

template<typename CharT, typename Traits>
basic_ostream<CharT, Traits>& indent(basic_ostream<CharT, Traits>& pOS)
{
  onnc::indentbuf::Indent();
  return pOS;
}

template<typename CharT, typename Traits>
basic_ostream<CharT, Traits>& unindent(basic_ostream<CharT, Traits>& pOS)
{
  onnc::indentbuf::Deindent();
  return pOS;
}

} // namespace of std

#endif
