//===- Flags.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_FLAGS_H
#define ONNC_ADT_FLAGS_H

namespace onnc {

/** \class Flags
 *  \brief Flags wrap enum types as a class.
 *
 *  Flags emulates enum class in C++11. It is useful at function overloading
 *  by enum values.
 */
template<typename Enum>
class Flags
{
public:
  typedef Enum enum_type;

public:
  Flags(const Flags& pOther)
  : m_Data(pOther.m_Data) {}

  Flags(Enum pFlag)
  : m_Data(pFlag) {}

  Flags(int pFlag = 0x0)
  : m_Data(pFlag) {}

  operator int () const
  { return m_Data; }

  bool operator! () const
  { return (m_Data == 0x0); }

  Flags operator& (int pMask ) const
  { return Flags(m_Data & pMask); }

  Flags operator& (unsigned int pMask ) const
  { return Flags(m_Data & pMask); }

  Flags operator& (Enum pMask ) const
  { return Flags(m_Data & pMask); }

  Flags& operator&= (int pMask ) {
    m_Data &= pMask;
    return *this;
  }

  Flags& operator&= (unsigned int pMask ) {
    m_Data &= pMask;
    return *this;
  }

  Flags& operator=(Flags pOther) {
    m_Data = pOther.m_Data;
    return *this;
  }

  Flags operator^ (Flags pOther) const
  { return Flags(m_Data^pOther.m_Data); }

  Flags operator^ (Enum pOther) const
  { return Flags(m_Data^pOther); }

  Flags& operator^= (Flags pOther) {
    m_Data ^= pOther.m_Data;
    return *this;
  } 

  Flags& operator^= (Enum pOther) {
    m_Data ^= pOther;
    return *this;
  }

  Flags operator| (Flags pOther) const
  { return Flags(m_Data | pOther.m_Data); }

  Flags operator| (Enum pOther ) const
  { return Flags(m_Data | pOther); }

  Flags& operator|= (Flags pOther) {
    m_Data |= pOther.m_Data;
    return *this;
  }

  Flags& operator|= (Enum pOther) {
    m_Data |= pOther;
    return *this;
  }

  Flags operator~ () const
  { return Flags(~m_Data); }

private:
  int m_Data;
};

} // namespace of onnc

#endif

