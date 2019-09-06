//===- View.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_VIEW_H_INCLUDED
#define ONNC_SUPPORT_VIEW_H_INCLUDED

#include "Iterator.h"
#include "Range.h"

namespace onnc {
namespace view {

template <typename Value>
constexpr iterator_range<integer_iterator<Value>> indices(Value from, Value to)
{
  using iterator_type = integer_iterator<Value>;
  return iterator_range<iterator_type>(iterator_type(from), iterator_type(to));
}

template <typename Value>
constexpr iterator_range<integer_iterator<Value>> indices(Value to)
{
  return indices(static_cast<Value>(0), to);
}

} // namespace view
} // namespace onnc
#endif
