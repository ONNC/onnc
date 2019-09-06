//===- Range.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_RANGE_H_INCLUDED
#define ONNC_SUPPORT_RANGE_H_INCLUDED

#include <utility>

namespace onnc {

template <typename Iterator>
class iterator_range
{
public:
  using iterator = Iterator;

public:
  iterator_range() = delete;
  iterator_range(iterator first, iterator last)
    : first_(first)
    , last_(last)
  {}

  iterator_range(const iterator_range&) = default;
  iterator_range(iterator_range&&)      = default;
  ~iterator_range()                     = default;

  iterator_range& operator=(const iterator_range&) = default;
  iterator_range& operator=(iterator_range&&) = default;

  iterator begin() const { return first_; }

  iterator end() const { return last_; }

  auto size() const -> decltype(std::declval<iterator>() - std::declval<iterator>()) { return last_ - first_; }

private:
  iterator first_;
  iterator last_;
};

template <typename Iterator>
inline constexpr iterator_range<Iterator> make_iterator_range(Iterator first, Iterator last)
{
  return iterator_range<Iterator>(first, last);
}

template <typename SizedRange>
inline constexpr auto size(const SizedRange& range) -> decltype(std::declval<SizedRange>().size())
{
  return range.size();
}

template <typename Range>
inline constexpr auto empty(const Range& range) -> decltype(std::declval<Range>().empty())
{
  return range.empty();
}

template <typename Range, typename... Args>
inline constexpr Range asRange(Args&&... args)
{
  return {std::forward<Args>(args)...};
}

template <typename Range>
inline constexpr auto front(Range& range) -> decltype(*std::begin(std::declval<Range>()))
{
  using std::begin;

  return *begin(range);
}

} // namespace onnc

#endif
