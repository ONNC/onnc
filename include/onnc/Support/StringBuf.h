//===-StringBuf.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_STRING_BUFFER_H
#define ONNC_SUPPORT_STRING_BUFFER_H
#include <streambuf>
#include <string>

namespace onnc {

/** \class BasicStringBuf
 *  \brief The actual work of input and output (for std::string)
 *
 *  Unlike std::basic_stringbuf, BasicStringBuf doesn't own a string for the
 *  internal buffer. Otherwise, BasicStringBuf uses the given string as the
 *  internal buffer. Every change to the BasicStringBuf also changes the
 *  content of given string directly.
 */
template<typename CharT, typename Traits, typename Alloc>
class BasicStringBuf : public std::basic_streambuf<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef Alloc  allocator_type;
  typedef typename traits_type::int_type    int_type;
  typedef typename traits_type::pos_type    pos_type;
  typedef typename traits_type::off_type    off_type;

  typedef std::basic_streambuf<char_type, traits_type> streambuf_type;
  typedef std::basic_string<char_type, Traits, Alloc>  string_type;
  typedef typename string_type::size_type              size_type;

public:
  explicit BasicStringBuf(string_type& pStr,
                          std::ios_base::openmode pMode = std::ios_base::out);

  ~BasicStringBuf() { }

  /// str - return a copy
  string_type str() const;

  /// clear - clear the content of the string buffer.
  void clear();

protected:
  void BasicStringBufInit(std::ios_base::openmode pMode);

  void doSync(char_type* pBase, size_type pO);

  /// Get sequence of characters
  /// override std::streambuf::xsgetn
  std::streamsize xsgetn(char_type* pS, std::streamsize pN);

  int_type underflow();

  /// Put sequence of characters
  /// override std::streambuf::xsputn
  std::streamsize xsputn(const char_type* pS, std::streamsize pN);

  int_type overflow(int_type pC = traits_type::eof());

protected:
  std::ios_base::openmode m_Mode;
  string_type&            m_String;
};

} // namespace of onnc

#include "Bits/StringBuf.tcc"

#endif
