//===- OStrStream.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_OUT_STRING_STREAM_H
#define ONNC_SUPPORT_OUT_STRING_STREAM_H
#include <onnc/Support/IOSFwd.h>
#include <onnc/Support/OStream.h>
#include <onnc/Support/StringBuf.h>
#include <cassert>

namespace onnc {

/** \class BasicOStrStream
 *  \brief BasicOStrStream is an OStream that writes to an std::string.
 */
template<typename CharT, typename Traits, typename Alloc>
class BasicOStrStream : public BasicOStream<CharT, Traits>
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
  typedef BasicStringBuf<CharT, Traits, Alloc> stringbuf_type;
  typedef BasicOStream<CharT, Traits> ostream_type;

public:
  explicit BasicOStrStream(string_type& pStr);

  /// Destructor - does nothing
  ~BasicOStrStream() {}

  stringbuf_type* rdbuf() const {
    return const_cast<stringbuf_type*>(&m_StringBuf);
  }

  /// str - get a copy of output stream
  string_type str() const { return m_StringBuf.str(); }

  /// Clear the content of the string buffer.
  void clearbuf() { m_StringBuf.clear(); }

private:
  stringbuf_type m_StringBuf;
};

} // namespace of onnc

#include <onnc/Support/Bits/OStrStream.tcc>

#endif
