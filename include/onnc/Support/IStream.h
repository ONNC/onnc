//===- IStream.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_ISTREAM_H
#define ONNC_SUPPORT_ISTREAM_H
#include <onnc/Support/IOSFwd.h>
#include <istream>

namespace onnc {

template<typename CharT, typename Traits>
class BasicIStream : public std::basic_istream<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  // Non-standard Types:
  typedef std::basic_streambuf<CharT, Traits> streambuf_type;
  typedef std::basic_ios<CharT, Traits>       ios_type;
  typedef std::basic_istream<CharT, Traits>   istream_type;

public:
  explicit BasicIStream(streambuf_type* pSB)
    : std::basic_istream<CharT, Traits>(pSB) { }

  virtual ~BasicIStream() { }
};

} // namespace of onnc

#endif
