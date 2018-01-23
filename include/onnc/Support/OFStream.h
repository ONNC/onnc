//===- OFStream.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_OFSTREAM_H
#define ONNC_SUPPORT_OFSTREAM_H
#include <onnc/Support/IOSFwd.h>
#include <onnc/Support/FileBuf.h>
#include <onnc/Support/OStream.h>

namespace onnc {

class Path;

/** \class BasicOFStream
 *  \brief Output file stream class to operate on file.
 */
template<typename CharT, typename Traits>
class BasicOFStream : public BasicOStream<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  // Non-standard types:
  typedef BasicFileBuf<char_type, traits_type> filebuf_type;
  typedef BasicOStream<char_type, traits_type> ostream_type;
  typedef typename filebuf_type::file_type    file_type;

public:
  /// Constructs an ofstream object that is not associated with any file.
  BasicOFStream();

  /// Constructs an ofstream object, initially associated with the newly
  /// constructed file object.
  BasicOFStream(const Path& pFileName,
                std::ios_base::openmode pMode = std::ios_base::out |
                                                std::ios_base::trunc);

  /// Constructs an ofstream object, initially associated with the existing
  /// file object.
  BasicOFStream(int pFD,
                std::ios_base::openmode pMode = std::ios_base::out |
                                                std::ios_base::trunc);

  /// Constructs an ofstream object, initially associated with the existing
  /// file object.
  BasicOFStream(file_type* pFile,
                std::ios_base::openmode pMode = std::ios_base::out |
                                                std::ios_base::trunc);

  /// Destruct and disassociates to the stream. If the file is created by the
  /// stream, then close it.
  ~BasicOFStream();

  /// Get the associated file buffer.
  filebuf_type* rdbuf() const { return const_cast<filebuf_type*>(&m_FileBuf); }

  /// check if the stream is currently associated to a file.
  bool is_open() const { return m_FileBuf.is_open(); }

  /// Opens the file indicated by argument \ref pFileName 
  /// @param [in] pFileName the name of file
  /// @param [in] pMode     flags describing the request of input/output mode for
  ///                       the file
  void open(const Path& pFileName,
            std::ios_base::openmode pMode = std::ios_base::out |
                                            std::ios_base::trunc);

  /// Associates this stream with the existing file descriptor \ref pFD. The mode
  /// of the stream must be compatible with the mode of the file descriptor. When
  /// this stream is closed via \ref close(), pFD is \b NOT closed.
  /// @param [in] pFD   the associated file descriptor.
  /// @param [in] pMode the mode of this stream.
  void open(int pFD,
            std::ios_base::openmode pMode = std::ios_base::out |
                                            std::ios_base::trunc);

  /// Associate this stream with the existing C stdio file \ref pFile. The mode
  /// of the stream must be compatible with the mode of C stdio file. When this
  /// stream is closed via \ref close(), pFile is \b NOT closed.
  /// @param [in] pFile the associated file descriptor.
  /// @param [in] pMode the mode of this stream.
  void open(file_type* pFile,
            std::ios_base::openmode pMode = std::ios_base::out |
                                            std::ios_base::trunc);

  /// Close the file associated with the stream and disassociate it.
  /// If the file is not created by the stream, then close() function only
  /// disassociates with it, does not close it.
  void close();

  virtual bool isDisplayed() const;

private:
  filebuf_type m_FileBuf;
};

} // namespace of onnc

#include <onnc/Support/Bits/OFStream.tcc>

#endif
