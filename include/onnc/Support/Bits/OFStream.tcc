//===- OFStream.tcc -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_OUTPUT_FILE_STREAM_TCC
#define ONNC_SUPPORT_OUTPUT_FILE_STREAM_TCC
#include <onnc/Config/Config.h>
#if defined(HAVE_UNISTD_H)
# include <unistd.h>
#endif

namespace onnc {
//===----------------------------------------------------------------------===//
// BasicOFStream Member Functions
//===----------------------------------------------------------------------===//
// BasicOFStream is designed as the manager of error state flags. It just like
// a decorator of BasicFileBuf.
template<typename CharT, typename Traits>
BasicOFStream<CharT, Traits>::BasicOFStream()
  : ostream_type(), m_FileBuf() {
  this->init(&m_FileBuf);
}

template<typename CharT, typename Traits>
BasicOFStream<CharT, Traits>::BasicOFStream(const Path& pFileName,
                                            std::ios_base::openmode pMode)
  : ostream_type(), m_FileBuf() {
  this->init(&m_FileBuf);
  this->open(pFileName, pMode);
}

template<typename CharT, typename Traits>
BasicOFStream<CharT, Traits>::BasicOFStream(int pFD,
                                            std::ios_base::openmode pMode)
  : ostream_type(), m_FileBuf() {
  this->init(&m_FileBuf);
  this->open(pFD, pMode);
}

template<typename CharT, typename Traits>
BasicOFStream<CharT, Traits>::BasicOFStream(file_type* pCFile,
                                            std::ios_base::openmode pMode)
  : ostream_type(), m_FileBuf() {
  this->init(&m_FileBuf);
  this->open(pCFile, pMode);
}

template<typename CharT, typename Traits>
BasicOFStream<CharT, Traits>::~BasicOFStream()
{
  // The file is close by filebuf.
}

template<typename CharT, typename Traits> void
BasicOFStream<CharT, Traits>::open(const Path& pFileName,
                                   std::ios_base::openmode pMode)
{
  if (!m_FileBuf.open(pFileName, pMode | std::ios_base::out))
    this->setstate(std::ios_base::failbit);
  else
    this->clear(); ///< clear error state flags
}

template<typename CharT, typename Traits> void
BasicOFStream<CharT, Traits>::open(int pFD,
                                   std::ios_base::openmode pMode)
{
  if (!m_FileBuf.associate(pFD, pMode | std::ios_base::out))
    this->setstate(std::ios_base::failbit);
  else
    this->clear(); ///< clear error state flags
}

template<typename CharT, typename Traits> void
BasicOFStream<CharT, Traits>::open(file_type* pCFile,
                                   std::ios_base::openmode pMode)
{
  if (!m_FileBuf.associate(pCFile, pMode | std::ios_base::out))
    this->setstate(std::ios_base::failbit);
  else
    this->clear(); ///< clear error state flags
}

template<typename CharT, typename Traits> void
BasicOFStream<CharT, Traits>::close()
{
  if (!m_FileBuf.close())
    this->setstate(std::ios_base::failbit);
}

template<typename CharT, typename Traits>
bool BasicOFStream<CharT, Traits>::isDisplayed() const
{
#if defined(HAVE_UNISTD_H)
  return isatty(m_FileBuf.fd());
#else
  return false;
#endif
}

} // namespace of onnc

#endif

