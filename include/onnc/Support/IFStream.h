//===- IFStream.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_IFSTREAM_H
#define ONNC_SUPPORT_IFSTREAM_H
#include <onnc/Support/FileDescriptor.h>
#include <onnc/Support/IOSFwd.h>
#include <onnc/Support/FileBuf.h>
#include <onnc/Support/IStream.h>

namespace onnc {

class Path;

/** \class BasicIFStream
 *  \brief Output file stream class to operate on file.
 */
template<typename CharT, typename Traits>
class BasicIFStream : public BasicIStream<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  // Non-standard types:
  typedef BasicFileBuf<char_type, traits_type> filebuf_type;
  typedef BasicIStream<char_type, traits_type> istream_type;
  typedef typename filebuf_type::file_type    file_type;

public:
  /// Constructs an ofstream object that is not associated with any file.
  BasicIFStream();

  /// Constructs an ofstream object, initially associated with the newly
  /// constructed file object.
  BasicIFStream(const Path& pFileName,
                std::ios_base::openmode pMode = std::ios_base::in);

  /// Constructs an ofstream object, initially associated with the existing
  /// file object.
  BasicIFStream(FD pFD,
                std::ios_base::openmode pMode = std::ios_base::in);

  /// Constructs an ofstream object, initially associated with the existing
  /// file object.
  BasicIFStream(file_type* pFile,
                std::ios_base::openmode pMode = std::ios_base::in);

  /// Destruct and disassociates to the stream. If the file is created by the
  /// stream, then close it.
  ~BasicIFStream();

  /// Get the associated file buffer.
  filebuf_type* rdbuf() const { return const_cast<filebuf_type*>(&m_FileBuf); }

  /// check if the stream is currently associated to a file.
  bool is_open() const { return m_FileBuf.is_open(); }

  /// Opens the file indicated by argument \ref pFileName 
  /// @param [in] pFileName the name of file
  /// @param [in] pMode     flags describing the request of input/output mode for
  ///                       the file
  void open(const Path& pFileName,
            std::ios_base::openmode pMode = std::ios_base::in);

  /// Associates this stream with the existing file descriptor \ref pFD. The mode
  /// of the stream must be compatible with the mode of the file descriptor. When
  /// this stream is closed via \ref close(), pFD is \b NOT closed.
  /// @param [in] pFD   the associated file descriptor.
  /// @param [in] pMode the mode of this stream.
  void open(FD pFD, std::ios_base::openmode pMode = std::ios_base::in);

  /// Associate this stream with the existing C stdio file \ref pFile. The mode
  /// of the stream must be compatible with the mode of C stdio file. When this
  /// stream is closed via \ref close(), pFile is \b NOT closed.
  /// @param [in] pFile the associated file descriptor.
  /// @param [in] pMode the mode of this stream.
  void open(file_type* pFile,
            std::ios_base::openmode pMode = std::ios_base::in);

  /// Read the entire file into \ref pResult.
  /// @param [out] pResult the file content being read
  istream_type& readall(std::string& pResult);

  /// Close the file associated with the stream and disassociate it.
  /// If the file is not created by the stream, then close() function only
  /// disassociates with it, does not close it.
  void close();

private:
  filebuf_type m_FileBuf;
};

} // namespace of onnc

#include <onnc/Support/Bits/IFStream.tcc>

#endif
