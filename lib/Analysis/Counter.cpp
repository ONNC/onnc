//===- Counter.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/Counter.h>

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
Counter::Counter()
  : m_Group() {
}

Counter::Counter(const Counter& pOther)
  : m_Group(pOther.m_Group) {
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
  int number = m_Group.readEntry(g_ValueKey, g_ValueDef);
  ++number;
  m_Group.writeEntry(g_ValueKey, number);
  return *this;
}

Counter& Counter::decrease()
{
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

bool Counter::isActive() const
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
    return Counter();

  Counter result;
  result.m_Group = pGroup;
  return result;
}

Counter Counter::Create(json::Group& pParent, StringRef pName, int pValue,
                        StringRef pDesc)
{
  bool exist = false;
  json::Group group = pParent.addGroup(pName, &exist);
  if (exist)
    return Counter();

  group.writeEntry(g_NameKey, pName);
  group.writeEntry(g_TypeKey, g_TypeValue);
  group.writeEntry(g_DescKey, pDesc);
  group.writeEntry(g_ValueKey, pValue);

  Counter result;
  result.m_Group = group;
  return result;
}

bool Counter::IsCounter(const json::Group& pGroup)
{
  int type = pGroup.readEntry(g_TypeKey, g_TypeValue + 1);

  // the group have neither "type" entry nor right type.
  return (g_TypeValue == type);
}

} // namespace of onnc
