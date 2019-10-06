//===- Iterator.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_ITERATOR_H_INCLUDED
#define ONNC_SUPPORT_ITERATOR_H_INCLUDED

namespace onnc {

template <typename Integer>
struct integer_iterator
{
public:
  using value_type = Integer;

public:
  integer_iterator()
    : value_()
  {}

  explicit integer_iterator(value_type value)
    : value_(value)
  {}

  integer_iterator(const integer_iterator&) = default;
  integer_iterator(integer_iterator&&)      = default;
  ~integer_iterator()                       = default;

  integer_iterator& operator=(const integer_iterator&) = default;
  integer_iterator& operator=(integer_iterator&&) = default;

  value_type operator*() const { return value_; }

  integer_iterator& operator++()
  {
    ++value_;
    return *this;
  }

  integer_iterator operator++(int)
  {
    integer_iterator copy(*this);
    ++(*this);
    return copy;
  }

private:
  Integer value_;
};

template <typename Integer>
bool operator==(integer_iterator<Integer> lhs, integer_iterator<Integer> rhs)
{
  return *lhs == *rhs;
}

template <typename Integer>
bool operator!=(integer_iterator<Integer> lhs, integer_iterator<Integer> rhs)
{
  return !(lhs == rhs);
}

template <typename Integer>
typename integer_iterator<Integer>::value_type operator-(integer_iterator<Integer> lhs, integer_iterator<Integer> rhs)
{
  return *lhs - *rhs;
}

} // namespace onnc

#endif
