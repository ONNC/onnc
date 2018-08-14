//===- Duration.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_TIME_DURATION_H
#define ONNC_SUPPORT_TIME_DURATION_H
#include <onnc/Support/DataTypes.h>
#include <onnc/Config/Config.h>
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif
#include <time.h>
#include <ostream>

namespace onnc {

/** \class onnc::Duration
 *  \brief represents a small time in the system.
 *
 *  Duration represent a while of time. It can not be negative. That
 *  means, subtracting a bigger duration from a smaller duration returns zero
 *  duration, not negative.
 */
class Duration
{
public:
  Duration();

  Duration(unsigned int pSecond,
           unsigned int pMicroSeconds = 0,
           unsigned int pNanoSeconds = 0);

  /// construct from C struct timespec
  explicit Duration(const struct timespec& pSpec);

  /// construct from POSIX struct timeval.
  explicit Duration(const struct timeval& pVal);

  Duration(const Duration& pOther);

  Duration& operator=(const Duration& pOther);

  Duration& operator=(const struct timespec& pSpec);

  Duration& operator=(const struct timeval& pVal);

  Duration& operator+=(const Duration& pOther);

  /// @retval Zero If pOther is larger
  Duration& operator-=(const Duration& pOther);

  bool isZero() const;

  unsigned int second() const { return m_Second; }

  unsigned int microsecond() const { return m_MicroSecond; }

  unsigned int nanosecond() const { return m_NanoSecond; }

  /// Return a Duration with @ref pValue seconds.
  static Duration Seconds(unsigned int pValue);

  /// Return a Duration with @ref pValue micro seconds.
  static Duration MicroSeconds(unsigned int pValue);

  /// Return a Duration with @ref pValue nano seconds.
  static Duration NanoSeconds(unsigned int pValue);

  /// To C struct timespec
  /// struct time spec {
  ///    time_t     tv_sec;   /* seconds since Jan. 1, 1970 */
  ///    long       tv_nsec;  /* and nanoseconds */
  /// };
  struct timespec toSpec() const;

  /// To POSIX struct timeval
  /// struct timeval {
  ///   time_t       tv_sec;     /* seconds */
  ///   suseconds_t  tv_usec;    /* and microseconds */
  /// };
  struct timeval toVal() const;

  friend bool operator==(const Duration& pX, const Duration& pY);

  friend bool operator<(const Duration& pX, const Duration& pY);

  void print(std::ostream& pOS) const;

private:
  unsigned int m_Second;  //< second
  unsigned int m_MicroSecond; //< 10^-6
  unsigned int m_NanoSecond;  //< 10^-9
};

inline bool operator!=(const Duration& pX, const Duration& pY) {
  return !(pX == pY);
}

inline bool operator>(const Duration& pX, const Duration& pY) {
  if (pX == pY)
    return false;
  if (pX < pY)
    return false;
  return true;
}

inline bool operator<=(const Duration& pX, const Duration& pY) {
  if ((pX == pY) || (pX < pY))
    return true;
  return false;
}

inline bool operator>=(const Duration& pX, const Duration& pY) {
  if ((pX == pY) || (pX > pY))
    return true;
  return false;
}

inline Duration operator+(const Duration& pX, const Duration& pY) {
  Duration result(pX);
  result += pY;
  return result;
}

inline Duration operator-(const Duration& pX, const Duration& pY) {
  Duration result(pX);
  result -= pY;
  return result;
}

inline std::ostream& operator<<(std::ostream& pOS, const Duration& pD) {
  pD.print(pOS);
  return pOS;
}

/// Return the difference between two duration, i.e. the
/// absolute value of pX - pY
inline Duration GetLength(const Duration& pX, const Duration& pY) {
  if (pX > pY)
    return (pX - pY);
  return (pY - pX);
}

bool operator==(const Duration& pX, const Duration& pY);
bool operator<(const Duration& pX, const Duration& pY);

} // namespace of onnc

#endif
