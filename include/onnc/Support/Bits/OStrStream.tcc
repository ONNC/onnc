//===- OStrStream.tcc -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_OUTPUT_STRING_STREAM_TCC
#define ONNC_SUPPORT_OUTPUT_STRING_STREAM_TCC

namespace onnc {

//===----------------------------------------------------------------------===//
// BasicOStrStream
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits, typename Alloc>
BasicOStrStream<CharT, Traits, Alloc>::BasicOStrStream(string_type& pStr)
  : ostream_type(),
    m_StringBuf(pStr, std::ios_base::out|std::ios_base::ate|std::ios_base::app) {
  this->init(&m_StringBuf);
}

} // namespace of onnc

#endif
