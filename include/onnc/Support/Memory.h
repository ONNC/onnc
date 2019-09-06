//===- Memory.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_MEMORY_H
#define ONNC_SUPPORT_MEMORY_H
#include <memory>
#include <type_traits>
#include <utility>

#if __cplusplus < 201402L
namespace std {

template <
  typename T,
  typename... Args,
  typename = typename std::enable_if<
    !std::is_array<T>::value
  >::type
>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <
  typename T,
  typename = typename std::enable_if<
    std::is_array<T>::value
  >::type
>
std::unique_ptr<T> make_unique(std::size_t size) {
  using element_type = typename std::remove_all_extents<T>::type;
  return std::unique_ptr<T>(new element_type[size]);
}

} // namespace of std

#endif
#endif
