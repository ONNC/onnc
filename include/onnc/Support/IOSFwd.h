//===- IOSFwd.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_IOSFWD_H
#define ONNC_SUPPORT_IOSFWD_H
#include <ios>

namespace onnc {

template<typename CharT> class BasicFile;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicOStream;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicIStream;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicFileBuf;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicOFStream;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicIFStream;

template<typename CharT, typename Traits = std::char_traits<CharT>,
         typename Alloc = std::allocator<CharT> >
  class BasicOStringBuf;

template<typename CharT, typename Traits = std::char_traits<CharT>,
         typename Alloc = std::allocator<CharT> >
  class BasicOStrStream;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicIndentOStreamBuf;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicIndentOStream;

template<typename CharT, typename Traits = std::char_traits<CharT>,
         typename Alloc = std::allocator<CharT> >
  class BasicIStrStream;

/// @name I/O Forward Declaration
/// @{
typedef BasicIStream<char>    IStream;
typedef BasicOStream<char>    OStream;
typedef BasicFileBuf<char>    FileBuf;
typedef BasicIFStream<char>   IFStream;
typedef BasicOFStream<char>   OFStream;
typedef BasicOStringBuf<char> OStringBuf;
typedef BasicOStrStream<char> OStrStream;
typedef BasicIndentOStreamBuf<char> IndentOStreamBuf;
typedef BasicIndentOStream<char> IndentOStream;
typedef BasicIStrStream<char> IStrStream;
/// @}

} // namespace of onnc

#endif
