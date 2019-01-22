//===- CounterIterator.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_COUNTER_ITERATOR_H
#define ONNC_ANALYSIS_COUNTER_ITERATOR_H
#include <onnc/JSON/Group.h>
#include <onnc/Analysis/Counter.h>

namespace onnc {

class Statistics;

/** \class CounterIterator
 *  \brief CounterIterator traverse counters in a Statistics object.
 *
 *  @see Statistics
 */
class CounterIterator
{
public:
  CounterIterator();

  CounterIterator(Statistics& pStatistics, json::Group::GroupIterator pIter);

  CounterIterator& next();

  Counter counter();

  bool operator==(const CounterIterator& pOther) const;

  bool operator!=(const CounterIterator& pOther) const {
    return !(*this == pOther);
  }

private:
  Statistics* m_pStatistics;
  json::Group::GroupIterator m_Iterator;
};

} // namespace of onnc

#endif
