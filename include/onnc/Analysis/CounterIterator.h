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

#include <functional>
#include <iterator>
#include <type_traits>

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
  using value_type        = Counter;
  using reference         = value_type;
  using iterator_category = std::input_iterator_tag;

private:
  using range_type            = Statistics;
  using range_value_type      = json::Group;
  using range_const_reference = std::add_lvalue_reference<
                                  std::add_const<range_value_type>::type
                                >::type;
  using range_iterator        = json::Group::GroupIterator;

public:
  CounterIterator();

  CounterIterator(range_type& pStatistics, range_iterator pIter);

  CounterIterator& operator++();
  CounterIterator operator++(int);

  value_type operator*();

  bool operator==(const CounterIterator& pOther) const;

  bool operator!=(const CounterIterator& pOther) const {
    return !(*this == pOther);
  }

private:
  range_type*    m_pStatistics;
  range_iterator m_Iterator;

  const std::function<bool(range_const_reference)> m_Predicate;
  const std::function<value_type(range_const_reference)> m_Generator;
};

} // namespace of onnc

#endif
