//===- IFStream.tcc -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_INPUT_FILE_STREAM_TCC
#define ONNC_SUPPORT_INPUT_FILE_STREAM_TCC

namespace onnc {
//===----------------------------------------------------------------------===//
// BasicIFStream Member Functions
//===----------------------------------------------------------------------===//
// BasicIFStream is designed as the manager of error state flags. It just like
// a decorator of BasicFileBuf.
template<typename CharT, typename Traits>
BasicIFStream<CharT, Traits>::BasicIFStream()
  : istream_type(&m_FileBuf), m_FileBuf() {
}

template<typename CharT, typename Traits>
BasicIFStream<CharT, Traits>::BasicIFStream(const Path& pFileName,
                                            std::ios_base::openmode pMode)
  : istream_type(&m_FileBuf), m_FileBuf() {
  this->init(&m_FileBuf);
  this->open(pFileName, pMode);
}

template<typename CharT, typename Traits>
BasicIFStream<CharT, Traits>::BasicIFStream(int pFD,
                                            std::ios_base::openmode pMode)
  : istream_type(&m_FileBuf), m_FileBuf() {
  this->init(&m_FileBuf);
  this->open(pFD, pMode);
}

template<typename CharT, typename Traits>
BasicIFStream<CharT, Traits>::BasicIFStream(file_type* pCFile,
                                            std::ios_base::openmode pMode)
  : istream_type(&m_FileBuf), m_FileBuf() {
  this->init(&m_FileBuf);
  this->open(pCFile, pMode);
}

template<typename CharT, typename Traits>
BasicIFStream<CharT, Traits>::~BasicIFStream()
{
  // The file is close by filebuf.
}

template<typename CharT, typename Traits> void
BasicIFStream<CharT, Traits>::open(const Path& pFileName,
                                   std::ios_base::openmode pMode)
{
  if (!m_FileBuf.open(pFileName, pMode | std::ios_base::in))
    this->setstate(std::ios_base::failbit);
  else
    this->clear(); ///< clear error state flags
}

template<typename CharT, typename Traits> void
BasicIFStream<CharT, Traits>::open(int pFD,
                                   std::ios_base::openmode pMode)
{
  if (!m_FileBuf.associate(pFD, pMode | std::ios_base::in))
    this->setstate(std::ios_base::failbit);
  else
    this->clear(); ///< clear error state flags
}

template<typename CharT, typename Traits> void
BasicIFStream<CharT, Traits>::open(file_type* pCFile,
                                   std::ios_base::openmode pMode)
{
  if (!m_FileBuf.associate(pCFile, pMode | std::ios_base::in))
    this->setstate(std::ios_base::failbit);
  else
    this->clear(); ///< clear error state flags
}

template<typename CharT, typename Traits>
typename BasicIFStream<CharT, Traits>::istream_type&
BasicIFStream<CharT, Traits>::readall(std::string& pResult)
{
  // TODO: check for better implementation
  pResult.clear();
  char buf[4096];
  while (this->read(buf, sizeof(buf)))
    pResult.append(buf, sizeof(buf));
  pResult.append(buf, this->gcount());
  return *this;
}

template<typename CharT, typename Traits> void
BasicIFStream<CharT, Traits>::close()
{
  if (!m_FileBuf.close())
    this->setstate(std::ios_base::failbit);
}

} // namespace of onnc

#endif
