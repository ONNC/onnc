//===- Counter.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_COUNTER_H
#define ONNC_ANALYSIS_COUNTER_H
#include <onnc/ADT/StringRef.h>
#include <onnc/Analysis/Counter.h>
#include <onnc/Analysis/GlobalStatistics.h>
#include <onnc/JSON/Group.h>

#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace onnc {

/** \class Counter
 *  \brief Counter provides an interface for statistical counting.
 *
 *  A counter object is an abstraction of json::Group object. It defines a
 *  format of json::Group to represent a counter when doing statistics.
 *  A counter has four entries:
 *  - "name" : [string],
 *  - "type" : [a magic integer],
 *  - "description" : [string],
 *  - "value" : [integer]
 *  Name should be a unique identity of a counter. Type is a fixed magic number.
 *  description is a string, and value keeps the value of a counter.
 *
 *  Users can use @ref Counter::Create function to create an empty counter. Or
 *  use @ref Counter::Get to get the counter object from a well-settle group.
 *
 *  In case users want to claim a Counter object and then posphone assignment of
 *  value, we also provide default constructor and assign operator for this case.
 *  @ref Counter::isValid return true if a Counter object already has well-settled
 *  group.
 */
class Counter final
{
public:
  using value_type = int;

public:
  friend class CounterIterator;

  /// Default constructor. This create an empty group object without any value.
  /// In this case, the counter object is invalid and users can not increase/
  /// decrease its value.
  Counter() = delete;

  /// Constructor. Users can get or create counter be give a name. If there
  /// is already a counter with the same name, just get the existing counter.
  /// otherwise create a new counter with that name. Users can also provide
  /// a default description and value for the counter. The parameter allowPrint
  /// is a flag to decide whether this counter is allowed to be printed by
  /// @ref global::stats()
  Counter(StringRef pName, StringRef pDesc, value_type pValue = 0, bool allowPrint = true);

  /// Constructor. Users can get or create counter be give a name. If there
  /// is already a counter with the same name, just get the existing counter.
  /// otherwise create a new counter with that name. Users can also provide
  /// a default value for the counter. The description of this counter is
  /// same as its name. The parameter allowPrint is a flag to decide whether
  /// this counter is allowed to be printed by @ref global::stats()
  Counter(StringRef pName, value_type pValue = 0, bool allowPrint = true);

  /// Copy constructor. Copy the value of the counter from the others. Since
  /// a Counter object is just an abstract interface of a json::Group object,
  /// this copy shall not create new json::Group object; the copy operator
  /// just create a new representation of the original json::Group object.
  Counter(const Counter& pOther) = default;
  Counter(Counter&& pOther) = default;

public:
  /// Assignment. Since a Counter object is just an abstract interface of a
  /// json::Group object, this assignment shall not create new json::Group object;
  /// the assignment operator just create a new representation of the original
  /// json::Group object.
  Counter& operator=(const Counter& pOther) = default;
  Counter& operator=(Counter&& pOther) = default;

  ~Counter() = default;

  /// update counter value by given @ref pNumber.
  Counter& operator=(value_type pNumber);

  /// The identity of the counter.
  StringRef name() const;

  /// The description of the counter.
  StringRef desc() const;

  /// Set description.
  Counter& desc(StringRef pDesc);

  /// The value of the counter.
  value_type value() const;

  bool isAllowPrint() const;

  /// Reset counter value to the default value
  Counter& reset();

  /// Casting operator. Cast to the value.
  operator value_type() const;

  Counter& operator++();

  /// The postfix form of ++.
  Counter& operator++(int);

  Counter& operator--();

  /// The postfix form of --.
  Counter& operator--(int);

  Counter& operator+=(value_type number);

  Counter& operator-=(value_type number);

private:
  /// Conversion constructor. This create a counter with an associated group object.
  /// The @ref CounterIterator can create Counter objects by invoking this constructor
  explicit Counter(json::Group&& group);

private:
  json::Group m_Group;
};

/// @retval true pGroup is a counter group.
bool isCounter(const json::Group& pGroup);

inline std::ostream& operator<<(std::ostream& stream, const Counter& counter)
{
  return stream << counter.value();
}

namespace view {
  struct counter_view_adaptor_tag {};
  static constexpr counter_view_adaptor_tag counter;
}

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

  CounterIterator(const range_type& stats, range_iterator iter = range_iterator{});

  CounterIterator& operator++();
  CounterIterator operator++(int);

  value_type operator*();

  bool operator==(const CounterIterator& pOther) const;

  bool operator!=(const CounterIterator& pOther) const {
    return !(*this == pOther);
  }

private:
  static Counter toCounter(json::Group group) {
    return Counter{std::move(group)};
  }

private:
  const range_type* m_pStatistics;
  range_iterator    m_Iterator;

  const std::function<bool(range_const_reference)> m_Predicate;
  const std::function<value_type(range_const_reference)> m_Generator;
};

template <typename Iterator>
class IteratorRange
{
public:
  using iterator = Iterator;

public:
  IteratorRange() = delete;
  IteratorRange(iterator first, iterator last)
    : m_First{first}
    , m_Last{last}
  { }

  IteratorRange(const IteratorRange&) = default;
  IteratorRange(IteratorRange&&) = default;

  ~IteratorRange() = default;

  IteratorRange& operator=(const IteratorRange&) = default;
  IteratorRange& operator=(IteratorRange&&) = default;

  iterator begin() { return static_cast<const IteratorRange*>(this)->begin(); }
  iterator begin() const { return m_First; }
  iterator end() { return static_cast<const IteratorRange*>(this)->end(); }
  iterator end() const { return m_Last; }

private:
  const iterator m_First;
  const iterator m_Last;
};

IteratorRange<CounterIterator> operator|(const Statistics& stats, view::counter_view_adaptor_tag);

} // namespace onnc

namespace std {
  template <>
  struct iterator_traits<onnc::CounterIterator>
  {
    using value_type        = onnc::CounterIterator::value_type;
    using reference         = value_type;
    using difference_type   = std::ptrdiff_t;
    using pointer           = std::add_pointer<value_type>::type;
    using iterator_category = onnc::CounterIterator::iterator_category;
  };
} // namespace std

#endif
