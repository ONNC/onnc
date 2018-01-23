//===- IStrStream.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_INTPUT_STRING_STREAM_H
#define ONNC_SUPPORT_INTPUT_STRING_STREAM_H
#include <onnc/Support/IOSFwd.h>
#include <onnc/Support/IStream.h>
#include <onnc/Support/StringBuf.h>
#include <sstream> //< for std::basic_stringbuf

namespace onnc {

template<typename CharT, typename Traits, typename Alloc>
class BasicIStrStream : public BasicIStream<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef Alloc  allocator_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  // Non-standard types:
  typedef std::basic_string<CharT, Traits, Alloc> string_type;
  // XXX: temporarily use std::basic_stringbuf instead of StringBuf
  typedef std::basic_stringbuf<CharT, Traits, Alloc> stringbuf_type;
  typedef BasicIStream<CharT, Traits> istream_type;

public:
  explicit BasicIStrStream(string_type& pStr);

  ~BasicIStrStream() { }

  stringbuf_type* rdbuf() const {
    return const_cast<stringbuf_type*>(&m_StringBuf);
  }

  /// str - get a copy of output stream
  string_type str() const { return m_StringBuf.str(); }

private:
  stringbuf_type m_StringBuf;
};

} // namespace of onnc

#include <onnc/Support/Bits/IStrStream.tcc>

#endif
