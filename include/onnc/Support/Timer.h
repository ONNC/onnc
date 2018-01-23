//===- Timer.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_TIMER_H
#define ONNC_SUPPORT_TIMER_H
#include <onnc/Support/DataTypes.h>
#include <string>

namespace onnc {

/** \class onnc::Timer
 *  \brief provides time-out signal.
 */
class Timer
{
public:
  typedef uint64_t Interval;

public:
  Timer();

  ~Timer();

  bool isActive() const { return m_bIsActive; }

  Interval interval() const { return m_Interval; }

  void start();

  void stop();

  static std::string unit();

private:
  Interval m_Interval;
  bool m_bIsActive;
};

} // namespace of onnc

#endif
