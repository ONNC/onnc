//===- Span.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_SPAN_H_INCLUDED
#define ONNC_SUPPORT_SPAN_H_INCLUDED

#include <onnc/Support/Range.h>

#include <array>
#include <type_traits>

namespace onnc {

template <typename T>
class span
{
public:
  using element_type    = T;
  using value_type      = typename std::remove_cv<element_type>::type;
  using index_type      = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer         = element_type*;
  using const_pointer   = const element_type*;
  using reference       = element_type&;
  using const_reference = const element_type&;
  using iterator        = pointer;
  using const_iterator  = pointer;

  constexpr span()
    : ptr_(nullptr)
    , size_(0)
  {}

  constexpr span(pointer first, pointer last)
    : ptr_(first)
    , size_(last - first)
  {}

  template <std::size_t N>
  constexpr span(element_type (&arr)[N]) noexcept
    : ptr_(arr)
    , size_(N)
  {}

  template <std::size_t N>
  constexpr span(std::array<value_type, N>& arr) noexcept
    : ptr_(arr.data())
    , size_(N)
  {}

  template <typename Container>
  constexpr span(const Container& container)
    : ptr_(container.data())
    , size_(container.size())
  {}

  constexpr iterator       begin() const noexcept { return ptr_; }
  constexpr const_iterator cbegin() const noexcept { return begin(); }

  constexpr iterator       end() const noexcept { return begin() + size(); }
  constexpr const_iterator cend() const noexcept { return end(); }

  constexpr reference front() const { return *begin(); }
  constexpr reference back() const { return *(--end()); }

  constexpr reference operator[](index_type idx) const { return *(begin() + idx); }
  constexpr pointer   data() const noexcept { return ptr_; }

  constexpr index_type size() const noexcept { return size_; }

private:
  pointer    ptr_;
  index_type size_;
};

} // namespace onnc

#endif
