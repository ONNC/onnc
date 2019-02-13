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
Counter::Counter(json::Group group)
  : m_Group{std::move(group)} {
}

Counter::Counter(const Counter& pOther)
  : m_Group(pOther.m_Group) {
}

Counter::Counter(StringRef pName, int pValue, StringRef pDesc)
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
    assert(!exist);

    m_Group.writeEntry(g_NameKey, pName);
    m_Group.writeEntry(g_TypeKey, g_TypeValue);
    m_Group.writeEntry(g_DescKey, pDesc);
    m_Group.writeEntry(g_ValueKey, pValue);
  }
}

Counter& Counter::operator=(const Counter& pOther)
{
  m_Group = pOther.m_Group;
  return *this;
}

Counter& Counter::reset(int pNumber)
{
  m_Group.writeEntry(g_ValueKey, pNumber);
  return *this;
}

Counter& Counter::increase()
{
  if (!isValid())
    return *this;

  int number = m_Group.readEntry(g_ValueKey, g_ValueDef);
  ++number;
  m_Group.writeEntry(g_ValueKey, number);
  return *this;
}

Counter& Counter::decrease()
{
  if (!isValid())
    return *this;

  int number = m_Group.readEntry(g_ValueKey, g_ValueDef);
  --number;
  m_Group.writeEntry(g_ValueKey, number);
  return *this;
}

int Counter::value() const
{
  return m_Group.readEntry(g_ValueKey, g_ValueDef);
}

StringRef Counter::name() const
{
  return m_Group.readEntry(g_NameKey, StringRef());
}

StringRef Counter::getDescription() const
{
  return m_Group.readEntry(g_DescKey, "none");
}

void Counter::setDescription(StringRef pDesc)
{
  m_Group.writeEntry(g_DescKey, pDesc);
}

bool Counter::isValid() const
{
  int type = m_Group.readEntry(g_TypeKey, g_TypeValue + 1);

  // the group have neither "type" entry nor right type.
  return (g_TypeValue == type);
}

void Counter::print(std::ostream& pOS) const
{
  pOS << name() << "," << std::dec << value();
  if (!getDescription().empty())
      pOS << "," << getDescription();
  pOS << std::endl; 
}

Counter Counter::Get(json::Group pGroup)
{
  if (!IsCounter(pGroup))
    return Counter{};

  return Counter{std::move(pGroup)};
}

bool Counter::IsCounter(const json::Group& pGroup)
{
  int type = pGroup.readEntry(g_TypeKey, g_TypeValue + 1);

  // the group have neither "type" entry nor right type.
  return (g_TypeValue == type);
}

} // namespace of onnc
