//===- FileBuf.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_FILEBUF_H
#define ONNC_SUPPORT_FILEBUF_H
#include <onnc/Support/IOSFwd.h>
#include <onnc/Support/FileDescriptor.h>
#include <streambuf>

namespace onnc {

class Path;

/** \class BasicFileBuf
 *
 *  BasicFileBuf is a file buffer that provides interoperation with system
 *  file operations, such as file descriptor and C FILE.
 *
 *  BasicFileBuf use system's stream buffering operations and doesn't maintain
 *  buffer by itself. @see man 3 setvbuf.
 *
 *  BasicFileBuf provides both read and write functions to IStream and
 *  OStream.
 */
template<typename CharT, typename Traits>
class BasicFileBuf : public std::basic_streambuf<CharT, Traits>
{
public:
  typedef FILE file_type;

  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef typename traits_type::int_type    int_type;
  typedef typename traits_type::pos_type    pos_type;
  typedef typename traits_type::off_type    off_type;

  typedef std::basic_streambuf<char_type, traits_type> streambuf_type;

public:
  BasicFileBuf();

  virtual ~BasicFileBuf();

  bool is_open() const;

  file_type* open(const Path& pFileName, std::ios_base::openmode pMode);

  file_type* associate(int pFD, std::ios_base::openmode pMode);

  file_type* associate(file_type* pFile, std::ios_base::openmode pMode);

  file_type* close();

  FD fd() const;

  file_type* file() { return m_CFile; }

protected:
  int_type syncgetc();

  int_type syncungetc(int_type pC);

  int_type syncputc(int_type pC);

  int_type underflow() {
    int_type pC = this->syncgetc();
    return this->syncungetc(pC);
  }

  int_type uflow() {
    // Store the gotten character in case we need to unget it.
    m_UngetBuf = this->syncgetc();
    return m_UngetBuf;
  }

  int_type pbackfail(int_type pC = traits_type::eof());

  std::streamsize xsgetn(char_type* pS, std::streamsize pN);

  int_type overflow(int_type pC = traits_type::eof());

  std::streamsize xsputn(const char_type* pS, std::streamsize pN);

  int sync()
  { return std::fflush(m_CFile); }

  std::streampos seekoff(std::streamoff pOff, std::ios_base::seekdir pDir,
       std::ios_base::openmode pMode = std::ios_base::in | std::ios_base::out);

  std::streampos
  seekpos(std::streampos pPos,
          std::ios_base::openmode pMode =
          std::ios_base::in | std::ios_base::out)
  { return seekoff(std::streamoff(pPos), std::ios_base::beg, pMode); }

private:
  /// open mode
  std::ios_base::openmode m_Mode;

  /// use buffered I/O
  file_type* m_CFile;

  /// true iff we opened m_CFile, and thus we must close it ourselves.
  bool m_bIsFileCreated;

  /// Last character gotten. This is used when pbackfail is
  /// called from basic_streambuf::sungetc()
  int_type m_UngetBuf;
};

} // namespace of onnc

#include <onnc/Support/Bits/FileBuf.tcc>

#endif
