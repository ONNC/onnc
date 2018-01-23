//===- ErrorCode.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_ERROR_CODE_H
#define ONNC_SUPPORT_ERROR_CODE_H
#include <onnc/Support/DataTypes.h>
#include <ostream>
#include <errno.h>

namespace onnc {

/** \class onnc::ScopedErrno
 *  \brief Save the global errno and restore it at destruction.
 */
class ScopedErrno
{
public:
  ScopedErrno() : m_Save(errno) { }

  ~ScopedErrno() { errno = m_Save; }

private:
  int m_Save;
};

/** \class onnc::SystemError
 *  \brief represents the system error.
 */
class SystemError
{
public:
  enum Code {
    kSuccess =                         0,
#ifdef EAFNOSUPPORT
    kAddressFamilyNotSupported =       EAFNOSUPPORT,
#endif
#ifdef EADDRINUSE
    kAddressInUse =                    EADDRINUSE,
#endif
    kAddressNotAvailable =             EADDRNOTAVAIL,
    kAlreadyConnected =                EISCONN,
    kArgumentListTooLong =             E2BIG,
    kArgumentOutOfDomain =             EDOM,
    kBadAddress =                      EFAULT,
    kBadFileDescriptor =               EBADF,
    kBadMessage =                      EBADMSG,
    kBrokenPipe =                      EPIPE,
    kConnectionAborted =               ECONNABORTED,
    kConnectionAlreadyInProgress =     EALREADY,
    kConnectionRefused =               ECONNREFUSED,
    kConnectionReset =                 ECONNRESET,
    kCrossDeviceLink =                 EXDEV,
    kDestinationAddressRequired =      EDESTADDRREQ,
    kDeviceOrResourceBusy =            EBUSY,
    kDirectoryNotEmpty =               ENOTEMPTY,
    kExecutableFormatError =           ENOEXEC,
    kFileExists =                      EEXIST,
    kFileTooLarge =                    EFBIG,
    kFilenameTooLong =                 ENAMETOOLONG,
    kFunctionNotSupported =            ENOSYS,
    kHostUnreachable =                 EHOSTUNREACH,
    kIdentifierRemoved =               EIDRM,
    kIllegalByteSequence =             EILSEQ,
    kInappropriateIoControlOperation = ENOTTY,
    kInterrupted =                     EINTR,
    kInvalidArgument =                 EINVAL,
    kInvalidSeek =                     ESPIPE,
    kIoError =                         EIO,
    kIsADirectory =                    EISDIR,
    kMessageSize =                     EMSGSIZE,
    kNetworkDown =                     ENETDOWN,
    kNetworkReset =                    ENETRESET,
    kNetworkUnreachable =              ENETUNREACH,
    kNoBufferSpace =                   ENOBUFS,
    kNoChildProcess =                  ECHILD,
    kNoLink =                          ENOLINK,
    kNoLockAvailable =                 ENOLCK,
    kNoMessageAvailable =              ENODATA,
    kNoMessage =                       ENOMSG,
    kNoProtocolOption =                ENOPROTOOPT,
    kNoSpaceOnDevice =                 ENOSPC,
    kNoStreamResources =               ENOSR,
    kNoSuchDeviceOrAddress =           ENXIO,
    kNoSuchDevice =                    ENODEV,
    kNoSuchFileOrDirectory =           ENOENT,
    kNoSuchProcess =                   ESRCH,
    kNotADirectory =                   ENOTDIR,
    kNotASocket =                      ENOTSOCK,
    kNotAStream =                      ENOSTR,
    kNotConnected =                    ENOTCONN,
    kNotEnoughMemory =                 ENOMEM,
    kNotSupported =                    ENOTSUP,
    kOperationCanceled =               ECANCELED,
    kOperationInProgress =             EINPROGRESS,
    kOperationNotPermitted =           EPERM,
    kOperationNotSupported =           EOPNOTSUPP,
    kOperationWouldBlock =             EWOULDBLOCK,
    kOwnerDead =                       EOWNERDEAD,
    kPermissionDenied =                EACCES,
    kProtocolError =                   EPROTO,
    kProtocolNotSupported =            EPROTONOSUPPORT,
    kReadOnlyFileSystem =              EROFS,
    kResourceDeadlockWouldOccur =      EDEADLK,
    kResourceUnavailableTryAgain =     EAGAIN,
    kResultOutOfRange =                ERANGE,
    kStateNotRecoverable =             ENOTRECOVERABLE,
    kStreamTimeout =                   ETIME,
    kTextFileBusy =                    ETXTBSY,
    kTimedOut =                        ETIMEDOUT,
    kTooManyFilesOpenInSystem =        ENFILE,
    kTooManyFilesOpen =                EMFILE,
    kTooManyLinks =                    EMLINK,
    kTooManySymbolicLinkLevels =       ELOOP,
    kValueTooLarge =                   EOVERFLOW,
    kWrongProtocolType =               EPROTOTYPE,
    kNotStartedYet,
    kAddrNotParsable,
    kNotARegular,
    kUnknownError
  };

public:
  SystemError()
    : m_Code(kNotStartedYet) {
  }

  SystemError(const SystemError& pErr)
    : m_Code(pErr.m_Code) {
  }

  SystemError(Code pCode)
    : m_Code(pCode) {
  }

  SystemError(int pCode)
    : m_Code(pCode) {
  }

  virtual ~SystemError() { }

  SystemError& operator=(const SystemError& pOther) {
    m_Code = pOther.m_Code;
    return *this;
  }

  SystemError& operator=(int pCode) {
    m_Code = pCode;
    return *this;
  }

  explicit operator int() const { return m_Code; }

  int code() const { return m_Code; }

  bool isGood() const { return (m_Code == kSuccess); }

  virtual void print(std::ostream& pOS) const;

  const char* msg() const;

protected:
  int m_Code;
};

inline bool operator==(const SystemError& pX, const SystemError& pY)
{
  return (pX.code() == pY.code());
}

inline bool operator!=(const SystemError& pX, const SystemError& pY)
{
  return (pX.code() != pY.code());
}

} // namespace of onnc

namespace std {
inline ostream& operator<<(ostream& pOS, const onnc::SystemError& pX)
{
  pX.print(pOS);
  return pOS;
}

} // namespace of std

#endif
