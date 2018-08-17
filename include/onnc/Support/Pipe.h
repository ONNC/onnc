//===- Pipe.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_PIPE_H
#define ONNC_SUPPORT_PIPE_H
#include <onnc/Support/FileDescriptor.h>
#include <onnc/Support/ErrorCode.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <unistd.h>

namespace onnc {

/** \class Pipe
 *  \brief provides abstraction for Unix pipe.
 */
class Pipe
{
public:
  enum Side {
    kRead = 0,
    kWrite,
    kBoth,
    kSize = kBoth
  };

public:
  Pipe();

  virtual ~Pipe() { }

  /// assert occurs if pSide is neither kRead nor kWrite.
  FD& operator[](Side pSide);

  /// assert occurs if pSide is neither kRead nor kWrite.
  FD operator[](Side pSide) const;

  /// template for explicit specification.
  template<Side SIDE> FD& side() { return m_FD[SIDE]; }

  template<Side SIDE> FD side() const { return m_FD[SIDE]; }

  /// template for explicit specification.
  template<Side SIDE> void close() {
    if (!isOpened())
      return;

    int ret = -1;
    do {
      ret = ::close(side<SIDE>());
    } while (-1 == ret && EINTR == errno);
  }

  /// @return The error status of this pipe.
  SystemError error() const { return m_Error; }

  /// @retval Successful to pipe operations
  bool isGood() const { return (SystemError::kSuccess == m_Error.code()); }

  bool isOpened() const { return isGood(); }

  /// Cast to boolean
  /// @retval Successful to pipe operations.
  operator bool() const { return isGood(); }

  /// Set the direction of the communication.
  /// If ROLE is kRead, the pipe is playing for reader.
  /// Otherwise, if ROLE is kWrite, it's playing for writer.
  template<Side ROLE> void play() { /** kDuplex do nothing **/ }

  /// Connect the file descriptor.
  /// Replace @ref pFD by side<SIDE>().
  /// @param[in,out] pFD The file descriptor being connected
  template<Side SIDE> void connect(int pFD) {
    ::close(pFD);
    ::dup2(m_FD[SIDE], pFD);
    ::close(m_FD[SIDE]);
  }

protected:
  FD m_FD[kSize];
  SystemError m_Error; ///< The error occurs during the life.
};

//===----------------------------------------------------------------------===//
// Template specification
//===----------------------------------------------------------------------===//
template<> FD& Pipe::side<Pipe::kBoth>();
template<> FD Pipe::side<Pipe::kBoth>() const;
template<> void Pipe::close<Pipe::kBoth>();

template<> void Pipe::play<Pipe::kRead>();
template<> void Pipe::play<Pipe::kWrite>();

template<> void Pipe::connect<Pipe::kBoth>(int pFD);

} // namespace of onnc

#endif
