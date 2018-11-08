//===- Perf.h -------------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_PERF_H
#define ONNC_SUPPORT_PERF_H
#include <onnc/Support/DataTypes.h>
#include <onnc/Support/PerfEvent.h>
#include <string>

namespace onnc {

//===----------------------------------------------------------------------===//
// Perf
//===----------------------------------------------------------------------===//
class Perf
{
public:
  typedef uint64_t Interval;

public:
  Perf();
  Perf(enum PerfEvent pEvent);
  ~Perf();

  bool isActive() const { return m_bIsActive; }
  Interval interval() const { return m_Interval; }
  Interval eventType() const { return m_EventType; }

  void start();
  void stop();

  static std::string unit();

private:
  Interval m_Interval, m_EventType;
  bool m_bIsActive;
};

} // namespace of onnc

#endif
