//===- IStrStream.tcc -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_INTPUT_STRING_STREAM_TCC
#define ONNC_SUPPORT_INTPUT_STRING_STREAM_TCC

namespace onnc {

template<typename CharT, typename Traits, typename Alloc>
BasicIStrStream<CharT, Traits, Alloc>::BasicIStrStream(string_type& pStr)
  : istream_type(&m_StringBuf), m_StringBuf(pStr, std::ios_base::in) {
  this->init(&m_StringBuf);
}

} // namespace of onnc

#endif
