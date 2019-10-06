//===- Counter.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/Counter.h>

#include <cassert>
#include <string>
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
static const char* g_DefaultValueKey = "default-value";
static const int   g_DefaultValueDef = (unsigned(-1) - 3);
static const char* g_ValueKey = "value";
static const int   g_ValueDef = (unsigned(-1) - 4);
static const char* g_AllowPrintKey = "is-print";
static const bool  g_AllowPrintDef = true;

//===----------------------------------------------------------------------===//
// Counter
//===----------------------------------------------------------------------===//
Counter::Counter(StringRef pName, StringRef pDesc, value_type pValue, bool allowPrint)
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

    m_Group.writeEntry(g_NameKey        , pName      );
    m_Group.writeEntry(g_TypeKey        , g_TypeValue);
    m_Group.writeEntry(g_DescKey        , pDesc      );
    m_Group.writeEntry(g_DefaultValueKey, pValue     );
    m_Group.writeEntry(g_ValueKey       , pValue     );
    m_Group.writeEntry(g_AllowPrintKey  , allowPrint );
  }
}

Counter::Counter(StringRef pName, value_type pValue, bool allowPrint)
  : Counter{pName, pName, pValue, allowPrint}
{ }

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

Counter::value_type Counter::value() const
{
  return m_Group.readEntry(g_ValueKey, g_ValueDef);
}

bool Counter::isAllowPrint() const
{
  return m_Group.readEntry(g_AllowPrintKey, g_AllowPrintDef);
}

Counter& Counter::reset()
{
  (*this) = m_Group.readEntry(g_DefaultValueKey, g_DefaultValueDef);

  return *this;
}

Counter::operator value_type() const
{
  return value();
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
  const auto original = static_cast<value_type>(*this);
  return (*this) = (original + number);
}

Counter& Counter::operator-=(value_type number)
{
  static_assert(std::is_signed<value_type>::value, "only accept signed value");
  return (*this) += (-number);
}

Counter::Counter(json::Group&& group)
  : m_Group{std::move(group)} {
  assert(isCounter(m_Group) && "create object by non-counter group");
}

bool isCounter(const json::Group& pGroup)
{
  int type = pGroup.readEntry(g_TypeKey, g_TypeValue + 1);

  // the group have neither "type" entry nor right type.
  return (g_TypeValue == type);
}

namespace internal {
} // namespace internal

//===----------------------------------------------------------------------===//
// CounterIterator
//===----------------------------------------------------------------------===//
CounterIterator::CounterIterator()
  : m_pStatistics{nullptr}
  , m_Iterator{}
  , m_Predicate{isCounter}
  , m_Generator{toCounter}
{ }

CounterIterator::CounterIterator(const range_type& stats, range_iterator iter)
  : m_pStatistics{&stats}
  , m_Iterator{iter}
  , m_Predicate{isCounter}
  , m_Generator{toCounter}
{ }

CounterIterator& CounterIterator::operator++()
{
  const range_iterator sentinel{};
  if (m_pStatistics == nullptr || m_Iterator == sentinel) {
    return *this;
  }

  do {
    m_Iterator.next();
  } while (m_Iterator != sentinel && !m_Predicate(m_Iterator.group()));

  return *this;
}

CounterIterator CounterIterator::operator++(int)
{
   auto prev = (*this);
   ++(*this);
   return prev;
}

CounterIterator::value_type CounterIterator::operator*()
{
  return m_Generator(m_Iterator.group());
}

bool CounterIterator::operator==(const CounterIterator& pOther) const
{
  return (m_pStatistics == pOther.m_pStatistics &&
          m_Iterator == pOther.m_Iterator);
}

IteratorRange<CounterIterator> operator|(const Statistics& stats, view::counter_view_adaptor_tag)
{
  return IteratorRange<CounterIterator>{
    CounterIterator{stats, stats.getCounterGroup().gBegin()},
    CounterIterator{stats}
  };
}

} // namespace of onnc
