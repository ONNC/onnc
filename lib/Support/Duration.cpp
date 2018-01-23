//===- Duration.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Duration.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Duration
//===----------------------------------------------------------------------===//
Duration::Duration()
  : m_Second(0), m_MicroSecond(0), m_NanoSecond(0) {
}

Duration::Duration(unsigned int pSeconds, unsigned int pMSeconds, unsigned int pNSeconds)
{
  m_Second = pSeconds + (pMSeconds/1000000) + (pNSeconds/1000000000);
  m_MicroSecond = (pMSeconds % 1000000) + (pNSeconds/1000);
  m_NanoSecond = pNSeconds % 1000;
}

Duration::Duration(const Duration& pOther)
  : m_Second(pOther.m_Second),
    m_MicroSecond(pOther.m_MicroSecond),
    m_NanoSecond(pOther.m_NanoSecond) {
}

Duration& Duration::operator=(const Duration& pOther)
{
  m_Second = pOther.m_Second;
  m_MicroSecond = pOther.m_MicroSecond;
  m_NanoSecond = pOther.m_NanoSecond;
  return *this;
}

Duration& Duration::operator=(const struct timespec& pSpec)
{
  m_Second = pSpec.tv_sec;
  m_MicroSecond = 0;
  m_NanoSecond = pSpec.tv_nsec;
  return *this;
}

Duration& Duration::operator=(const struct timeval& pVal)
{
  m_Second = pVal.tv_sec;
  m_MicroSecond = pVal.tv_usec;
  m_NanoSecond = 0;
  return *this;
}

Duration& Duration::operator+=(const Duration& pOther)
{
  m_Second += pOther.m_Second;
  m_MicroSecond += pOther.m_MicroSecond;
  m_NanoSecond += pOther.m_NanoSecond;

  // carry on
  m_Second = m_Second + (m_MicroSecond/1000000) + (m_NanoSecond/1000000000);
  m_MicroSecond = (m_MicroSecond % 1000000) + (m_NanoSecond/1000);
  m_NanoSecond = (m_NanoSecond % 1000);
  return *this;
}

Duration& Duration::operator-=(const Duration& pOther)
{
  if (pOther >= *this) {
    m_Second = m_MicroSecond = m_NanoSecond = 0;
    return *this;
  }

  if (pOther.m_NanoSecond > m_NanoSecond) {
    // carry
    if (m_MicroSecond > 1)
      m_MicroSecond -= 1;
    else {
      m_Second -= 1;
      m_MicroSecond += 1000000;
      m_MicroSecond -= 1;
    }
    m_NanoSecond += 1000;
  }
  m_NanoSecond -= pOther.m_NanoSecond;

  if (pOther.m_MicroSecond > m_MicroSecond) {
    // carry
    m_Second -= 1;
    m_MicroSecond += 1000000;
  }
  m_MicroSecond -= pOther.m_MicroSecond;

  m_Second -= pOther.m_Second;
  return *this;
}

Duration::Duration(const struct timespec& pT)
  : m_Second(pT.tv_sec), m_MicroSecond(0), m_NanoSecond(pT.tv_nsec) {
}

Duration::Duration(const struct timeval& pT)
  : m_Second(pT.tv_sec), m_MicroSecond(pT.tv_usec), m_NanoSecond(0) {
}

bool Duration::isZero() const
{
  return ((0 == m_Second ) && (0 == m_MicroSecond) && (0 == m_NanoSecond));
}

Duration Duration::Seconds(unsigned int pValue)
{
  return Duration(pValue, 0, 0);
}

Duration Duration::MicroSeconds(unsigned int pValue)
{
  return Duration(0, pValue, 0);
}

Duration Duration::NanoSeconds(unsigned int pValue)
{
  return Duration(0, 0, pValue);
}

struct timespec Duration::toSpec() const
{
  struct timespec result;
  result.tv_sec = m_Second + (m_MicroSecond/1000000) + (m_NanoSecond/1000000000);
  result.tv_nsec = ((m_MicroSecond%1000000) * 1000) + (m_NanoSecond%1000000000);
  return result;
}

struct timeval Duration::toVal() const
{
  struct timeval result;
  result.tv_sec = m_Second + (m_MicroSecond/1000000) + (m_NanoSecond/1000000000);
  result.tv_usec = (m_MicroSecond%1000000) + ((m_NanoSecond%1000000000)/1000);
  return result;
}

void Duration::print(std::ostream& pOS) const
{
  pOS << m_Second << "s, " << m_MicroSecond << "us, " << m_NanoSecond << "ns";
}

bool onnc::operator==(const Duration& pX, const Duration& pY)
{
  return ((pX.m_Second == pY.m_Second) &&
          (pX.m_MicroSecond == pY.m_MicroSecond) &&
          (pX.m_NanoSecond == pY.m_NanoSecond));
}

bool onnc::operator<(const Duration& pX, const Duration& pY)
{
  if (pX.m_Second < pY.m_Second)
    return true;
  if (pX.m_Second == pY.m_Second) {
    if (pX.m_MicroSecond < pY.m_MicroSecond)
      return true;
    if (pX.m_MicroSecond == pY.m_MicroSecond) {
      if (pX.m_NanoSecond < pY.m_NanoSecond)
        return true;
    }
  }
  return false;
}
