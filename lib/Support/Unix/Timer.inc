//===- Timer.inc ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/ManagedStatic.h>
#include <time.h>
#include <unistd.h>
#include <cassert>

#if defined(HAVE_SYS_TIMES_H)
#include <sys/times.h>
#endif

#if defined(HAVE_SYS_TIME_H) && defined(ENABLE_GETTIMEOFDAY)
#include <sys/time.h>
#endif

namespace onnc {

//===----------------------------------------------------------------------===//
// Timer Implementation
//===----------------------------------------------------------------------===//
class TimerImpl
{
public:
  TimerImpl() {
    if (-1 == g_ClkTick) {
      g_ClkTick = sysconf(_SC_CLK_TCK);
      assert((0 < g_ClkTick) && "sysconf error");
    }
  }

   Timer::Interval ns() {
#if defined(HAVE_CLOCK_GETTIME) && defined(ENABLE_CLOCK_GETTIME)
     struct timespec ts;
     int r = clock_gettime(CLOCK_MONOTONIC, &ts);
     return r == -1 ? -1 : ts.tv_sec * 1000000000LL + ts.tv_nsec;
#elif defined(HAVE_GETTIMEOFDAY) && defined(ENABLE_GETTIMEOFDAY)
     struct timeval tv;
     int r = gettimeofday(&tv, NULL);
     return r == -1 ? -1 : tv.tv_sec * 1000000000LL + (tv.tv_usec * 1000LL);
#else
     struct tms tm;
     clock_t r = times(&tm);
     return r == -1 ? -1 : r * 1000000000LL / g_ClkTick;
#endif
  }

  void start() {
    m_Start = ns();
    assert(-1 != m_Start && "fail to get starting time");
  }

  void stop() {
    m_End = ns();
    assert(-1 != m_End && "fail to get elapsed time");
  }

  Timer::Interval clock() const {
    return (m_End - m_Start);
  }

private:
  Timer::Interval m_Start;
  Timer::Interval m_End;

  static long g_ClkTick;
};

long TimerImpl::g_ClkTick = -1;

static ManagedStatic<TimerImpl> g_Timer;

//===----------------------------------------------------------------------===//
// Timer
//===----------------------------------------------------------------------===//
Timer::Timer()
  : m_Interval(0), m_bIsActive(false) {
}

Timer::~Timer()
{
}

void Timer::start()
{
  m_bIsActive = true;
  g_Timer->start();
}

void Timer::stop()
{
  g_Timer->stop();
  m_bIsActive = false;
  m_Interval = g_Timer->clock();
}

std::string Timer::unit()
{
  return "ns";
}

} // namespace of onnc
