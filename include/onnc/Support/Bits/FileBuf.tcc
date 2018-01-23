//===- FileBuf.tcc -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_FILE_BUFFER_TCC
#define ONNC_SUPPORT_FILE_BUFFER_TCC
#include <onnc/Support/Path.h>
#include <onnc/Support/FileStatus.h>
#include <limits>

namespace {

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static inline const char*
fopen_mode(std::ios_base::openmode pMode)
{
  enum { ///< shorten names
    in     = std::ios_base::in,
    out    = std::ios_base::out,
    trunc  = std::ios_base::trunc,
    app    = std::ios_base::app,
    binary = std::ios_base::binary
  };

  switch (pMode & (in|out|trunc|app|binary)) {
    case (   out                 ): return "w";
    case (   out      |app       ): return "a";
    case (             app       ): return "a";
    case (   out|trunc           ): return "w";
    case (in                     ): return "r";
    case (in|out                 ): return "r+";
    case (in|out|trunc           ): return "w+";
    case (in|out      |app       ): return "a+";
    case (in          |app       ): return "a+";

    case (   out          |binary): return "wb";
    case (   out      |app|binary): return "ab";
    case (             app|binary): return "ab";
    case (   out|trunc    |binary): return "wb";
    case (in              |binary): return "rb";
    case (in|out          |binary): return "r+b";
    case (in|out|trunc    |binary): return "w+b";
    case (in|out      |app|binary): return "a+b";
    case (in          |app|binary): return "a+b";

    default: return NULL;
  }
  return NULL;
}

} // namespace anonymous

namespace onnc {
//===----------------------------------------------------------------------===//
// BasicFileBuf Member Functions
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits>
BasicFileBuf<CharT, Traits>::BasicFileBuf()
  : std::basic_streambuf<CharT, Traits>(),
    m_Mode(std::ios_base::openmode(0)), m_CFile(NULL), m_bIsFileCreated(false),
    m_UngetBuf(traits_type::eof()) {
}

template<typename CharT, typename Traits>
BasicFileBuf<CharT, Traits>::~BasicFileBuf()
{
  this->close();
}

template<typename CharT, typename Traits>
bool BasicFileBuf<CharT, Traits>::is_open() const
{
  return (NULL != m_CFile);
}

template<typename CharT, typename Traits>
typename BasicFileBuf<CharT, Traits>::file_type*
BasicFileBuf<CharT, Traits>::open(const Path& pFileName,
                                   std::ios_base::openmode pMode)
{
  const char* c_mode = fopen_mode(pMode);
  if (NULL != c_mode &&
      !this->is_open() &&
      (m_CFile = fopen(pFileName.c_str(), c_mode))) {
    m_bIsFileCreated = true;
  }
  return m_CFile;
}

template<typename CharT, typename Traits>
typename BasicFileBuf<CharT, Traits>::file_type*
BasicFileBuf<CharT, Traits>::associate(int pFD,
                                        std::ios_base::openmode pMode)
{
  const char* c_mode = fopen_mode(pMode);
  if (NULL != c_mode &&
      !this->is_open() &&
      (m_CFile = fdopen(pFD, c_mode))) {
    char* buffer = NULL;
    m_bIsFileCreated = true;
    if (0 == pFD) {
      setvbuf(m_CFile, buffer, _IONBF, 0);
    }
  }
  return m_CFile;
}

template<typename CharT, typename Traits>
typename BasicFileBuf<CharT, Traits>::file_type*
BasicFileBuf<CharT, Traits>::associate(file_type* pFile,
                                        std::ios_base::openmode pMode)
{
  if (!this->is_open() && NULL != pFile) {
    int local_err;
    errno = 0;
    do
      local_err = this->sync();
    while (local_err && errno == EINTR);
    if (!local_err) {
      m_CFile = pFile;
      m_bIsFileCreated = false;
      return m_CFile;
    }
  }
  return m_CFile;
}

template<typename CharT, typename Traits>
typename BasicFileBuf<CharT, Traits>::file_type*
BasicFileBuf<CharT, Traits>::close()
{
  if (this->is_open()) {
    int local_err = 0;
    if (m_bIsFileCreated) {
      errno = 0;
      do
        local_err = fclose(m_CFile);
      while (local_err && errno == EINTR);
    }
    m_CFile = 0;
    if (!local_err)
      return m_CFile;
  }
  return m_CFile;
}

template<typename CharT, typename Traits>
int BasicFileBuf<CharT, Traits>::fd() const
{
  return fileno(m_CFile);
}

template<typename CharT, typename Traits>
typename BasicFileBuf<CharT, Traits>::int_type
BasicFileBuf<CharT, Traits>::pbackfail(int_type pC)
{
  int_type ret;
  const int_type ceof = traits_type::eof();

  // Check if the unget or putback was requested
  if (traits_type::eq_int_type(pC, ceof)) {
    if (!traits_type::eq_int_type(m_UngetBuf, ceof))
      ret = this->syncungetc(m_UngetBuf);
    else // buffer invalid, fail.
      ret = ceof;
  }
  else // putback
    ret = this->syncungetc(pC);

  // The buffered character is no longer valid, discard it.
  m_UngetBuf = ceof;
  return ret;
}

template<typename CharT, typename Traits>
typename BasicFileBuf<CharT, Traits>::int_type
BasicFileBuf<CharT, Traits>::overflow(int_type pC)
{
  int_type ret;
  if (traits_type::eq_int_type(pC, traits_type::eof())) {
    if (std::fflush(m_CFile))
      ret = traits_type::eof();
    else
      ret = traits_type::not_eof(pC);
  }
  else
    ret = this->syncputc(pC);
  return ret;
}

template<typename CharT, typename Traits> std::streampos
BasicFileBuf<CharT, Traits>::seekoff(std::streamoff pOff,
                                     std::ios_base::seekdir pDir,
                                     std::ios_base::openmode pMode)
{
  std::streampos ret(std::streamoff(-1));
  int whence;
  if (pDir == std::ios_base::beg)
    whence = SEEK_SET;
  else if (pDir == std::ios_base::cur)
    whence = SEEK_CUR;
  else
    whence = SEEK_END;
  if (!fseek(m_CFile, pOff, whence))
    ret = std::streampos(std::ftell(m_CFile));
  return ret;
}

//===----------------------------------------------------------------------===//
// Partial specification members 
//===----------------------------------------------------------------------===//
template<>
inline BasicFileBuf<char>::int_type BasicFileBuf<char>::syncgetc()
{
  return std::getc(m_CFile);
}

template<>
inline BasicFileBuf<char>::int_type
BasicFileBuf<char>::syncungetc(int_type pC)
{
  return std::ungetc(pC, m_CFile);
}

template<>
inline BasicFileBuf<char>::int_type
BasicFileBuf<char>::syncputc(int_type pC)
{
  return std::putc(pC, m_CFile);
}

template<>
inline std::streamsize
BasicFileBuf<char>::xsgetn(char* pS, std::streamsize pN)
{
  std::streamsize ret = std::fread(pS, 1, pN, m_CFile);
  if (ret > 0)
    m_UngetBuf = traits_type::to_int_type(pS[ret - 1]);
  else
    m_UngetBuf = traits_type::eof();
  return ret;
}

template<>
inline std::streamsize
BasicFileBuf<char>::xsputn(const char* pS, std::streamsize pN)
{
  return std::fwrite(pS, 1, pN, m_CFile);
}

} // namespace of onnc

#endif
