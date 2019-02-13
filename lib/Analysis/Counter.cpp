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
    if (!IsCounter(selfGroup)) {
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

Counter::value_type Counter::value() const
{
  return m_Group.readEntry(g_ValueKey, g_ValueDef);
}

StringRef Counter::name() const
{
  return m_Group.readEntry(g_NameKey, StringRef());
}

StringRef Counter::getDescription() const
{
  return m_Group.readEntry(g_DescKey, defaultDesc);
}

void Counter::setDescription(StringRef pDesc)
{
  m_Group.writeEntry(g_DescKey, pDesc);
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
  if (!isValid()) {
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

bool Counter::isValid() const
{
  int type = m_Group.readEntry(g_TypeKey, g_TypeValue + 1);

  // the group have neither "type" entry nor right type.
  return (g_TypeValue == type);
}

bool Counter::IsCounter(const json::Group& pGroup)
{
  int type = pGroup.readEntry(g_TypeKey, g_TypeValue + 1);

  // the group have neither "type" entry nor right type.
  return (g_TypeValue == type);
}

Counter::Counter(json::Group group)
  : m_Group{std::move(group)} {
  assert(IsCounter(m_Group) && "create object by non-counter group");
}

std::ostream& operator<<(std::ostream& stream, const Counter& counter)
{
  return stream << "counter("
                << "name=\"" << counter.name() << "\""
                << ",value=\"" << std::dec << counter.value() << "\""
                << ",desc=\"" << counter.getDescription() << "\""
                << ")"
                ;
}

} // namespace of onnc
