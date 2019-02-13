//===- CounterIterator.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/CounterIterator.h>

namespace onnc {

namespace internal {
  Counter toCounter(const json::Group& group) {
    return Counter{group};
  }
} // namespace internal


//===----------------------------------------------------------------------===//
// CounterIterator
//===----------------------------------------------------------------------===//
CounterIterator::CounterIterator()
  : m_pStatistics{nullptr}
  , m_Iterator{}
  , m_Predicate{isCounter}
  , m_Generator{internal::toCounter}
{ }

CounterIterator::CounterIterator(Statistics& pStatistics,
                                 json::Group::GroupIterator pIter)
  : m_pStatistics{&pStatistics}
  , m_Iterator{pIter}
  , m_Predicate{isCounter}
  , m_Generator{internal::toCounter}
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

} // namespace of onnc
