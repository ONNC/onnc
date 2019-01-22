//===- CounterIterator.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/CounterIterator.h>

namespace onnc {

//===----------------------------------------------------------------------===//
// CounterIterator
//===----------------------------------------------------------------------===//
CounterIterator::CounterIterator()
  : m_pStatistics(nullptr), m_Iterator() {
}

CounterIterator::CounterIterator(Statistics& pStatistics,
                                 json::Group::GroupIterator pIter)
  : m_pStatistics(&pStatistics), m_Iterator(pIter) {
}

CounterIterator& CounterIterator::next()
{
  if (nullptr == m_pStatistics)
    return *this;

  do {
    m_Iterator.next();
  } while (!Counter::IsCounter(m_Iterator.group()));
  return *this;
}

Counter CounterIterator::counter()
{
  return Counter::Get(m_Iterator.group());
}

bool CounterIterator::operator==(const CounterIterator& pOther) const
{
  return (m_pStatistics == pOther.m_pStatistics &&
          m_Iterator == pOther.m_Iterator);
}

} // namespace of onnc
