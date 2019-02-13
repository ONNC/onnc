//===- Counter.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/Counter.h>
#include <onnc/Analysis/GlobalStatistics.h>

#include <cassert>
#include <utility>

namespace onnc {

/// A counter is a JSON object with this format:
/// [string of name] : {
///   "name" : [string],
///   "type" : [constant (unsigned(-1) - 2],
///   "description" : [string]
///   "value" : [integer]
/// }
static const char* g_NameKey = "name";
static const char* g_TypeKey = "type";
static const int   g_TypeValue = (unsigned(-1) - 2);
static const char* g_DescKey = "description";
static const char* g_ValueKey = "value";
static const int   g_ValueDef = (unsigned(-1) - 1);

//===----------------------------------------------------------------------===//
// Counter
//===----------------------------------------------------------------------===//
Counter::Counter(StringRef pName, value_type pValue, StringRef pDesc)
{
  auto counterGroup = global::stats().getCounterGroup();
  if (counterGroup.hasGroup(pName)) {
    auto selfGroup = counterGroup.group(pName);
    if (!isCounter(selfGroup)) {
      assert(false && "attempt to create counter by existing non-counter group");
      return;
    }

    m_Group = selfGroup;
  } else {
    bool exist = false;
    m_Group = counterGroup.addGroup(pName, &exist);
    assert(!exist && "group should be created by only one thread");

    m_Group.writeEntry(g_NameKey, pName);
    m_Group.writeEntry(g_TypeKey, g_TypeValue);
    m_Group.writeEntry(g_DescKey, pDesc);
    m_Group.writeEntry(g_ValueKey, pValue);
  }
}

Counter& Counter::operator=(value_type pNumber)
{
  m_Group.writeEntry(g_ValueKey, pNumber);
  return *this;
}

StringRef Counter::name() const
{
  return m_Group.readEntry(g_NameKey, StringRef());
}

StringRef Counter::desc() const
{
  return m_Group.readEntry(g_DescKey, "");
}

Counter& Counter::desc(StringRef pDesc)
{
  m_Group.writeEntry(g_DescKey, pDesc);
  return *this;
}

Counter::operator value_type() const
{
  return m_Group.readEntry(g_ValueKey, g_ValueDef);
}

Counter& Counter::operator++()
{
  return (*this) += 1;
}

Counter& Counter::operator++(int)
{
  return ++(*this);
}

Counter& Counter::operator--()
{
  return (*this) -= 1;
}

Counter& Counter::operator--(int)
{
  return --(*this);
}

Counter& Counter::operator+=(value_type number)
{
  if (!isValid(*this)) {
    assert(false && "update an invalid counter");
    return *this;
  }

  const auto original = static_cast<value_type>(*this);
  return (*this) = (original + number);
}

Counter& Counter::operator-=(value_type number)
{
  static_assert(std::is_signed<value_type>::value);
  return (*this) += (-number);
}

Counter::Counter(json::Group group)
  : m_Group{std::move(group)} {
  assert(isCounter(m_Group) && "create object by non-counter group");
}

bool isValid(const Counter& counter)
{
  int type = counter.m_Group.readEntry(g_TypeKey, g_TypeValue + 1);

  // the group have neither "type" entry nor right type.
  return (g_TypeValue == type);
}

bool isCounter(const json::Group& pGroup)
{
  int type = pGroup.readEntry(g_TypeKey, g_TypeValue + 1);

  // the group have neither "type" entry nor right type.
  return (g_TypeValue == type);
}

std::ostream& operator<<(std::ostream& stream, const Counter& counter)
{
  return stream << "counter("
                << "name=\"" << counter.name() << "\""
                << ",value=" << std::dec << static_cast<Counter::value_type>(counter)
                << ",desc=\"" << counter.desc() << "\""
                << ")"
                ;
}

} // namespace of onnc
