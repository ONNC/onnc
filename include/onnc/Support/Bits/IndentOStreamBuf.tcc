//===- IndentOStreamBuf.tcc -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_INDENT_OSTREAM_BUFFER_TCC
#define ONNC_SUPPORT_INDENT_OSTREAM_BUFFER_TCC

namespace onnc {

//===----------------------------------------------------------------------===//
// BasicIndentOStreamBuf
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits>
BasicIndentOStreamBuf<CharT, Traits>::BasicIndentOStreamBuf(
                                                       streambuf_type* pDestBuf,
                                                       unsigned int pIndent,
                                                       CharT pFill)
  : m_pDestBuf(pDestBuf),
    m_bIsAtStartOfLine(true) {
  indentbuf::SetIndent(pIndent);
  indentbuf::SetFill(pFill);
}

template<typename CharT, typename Traits>
BasicIndentOStreamBuf<CharT, Traits>::~BasicIndentOStreamBuf()
{
  m_pDestBuf = nullptr;
  m_bIsAtStartOfLine = true;
  indentbuf::ClearIndent();
}

template<typename CharT, typename Traits>
typename BasicIndentOStreamBuf<CharT, Traits>::int_type
BasicIndentOStreamBuf<CharT, Traits>::overflow(int_type pC)
{
  if (traits_type::eq_int_type(pC, traits_type::eof()))
    return m_pDestBuf->sputc(pC);

  if (m_bIsAtStartOfLine) {
    fill_n(std::ostreambuf_iterator<CharT>(m_pDestBuf),
           indentbuf::GetIndent(),
           indentbuf::GetFill());
    m_bIsAtStartOfLine = false;
  }

  if (traits_type::eq_int_type(m_pDestBuf->sputc(pC), traits_type::eof()))
    return traits_type::eof();

  if (traits_type::eq_int_type(pC, traits_type::to_char_type('\n')))
    m_bIsAtStartOfLine = true;

  return traits_type::not_eof(pC);
}

} // namespace of onnc

#endif
