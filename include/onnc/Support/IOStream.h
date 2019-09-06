//===- IOStream.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_IOSTREAM_H
#define ONNC_SUPPORT_IOSTREAM_H
#include <onnc/Support/OStream.h>
#include <onnc/Support/TypeTraits.h>

#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace onnc {

/// outs() - This returns a reference to a OStream for standard output.
/// Use it like: outs() << "foo" << "bar";
OStream& outs();

/// errs() - This returns a reference to a OStream for standard error.
/// Use it like: errs() << "foo" << "bar";
OStream& errs();

/// ins() - This returns a reference to an IStream for standard in.
/// Use it like: ins() >> x;
IStream& ins();

namespace operators {
template <typename T, std::size_t N,
          typename = typename std::enable_if<!std::is_same<std::ostream::char_type, T>::value>::type>
inline std::ostream& operator<<(std::ostream& stream, const T (&values)[N])
{
  stream << "[";
  bool isFirst = true;
  for (const auto& value : values) {
    if (!isFirst) {
      stream << ", ";
    }

    stream << value;
    isFirst = false;
  }
  return stream << "]";
}

template <typename T, std::size_t N>
inline std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& values)
{
  stream << "[";
  bool isFirst = true;
  for (const auto& value : values) {
    if (!isFirst) {
      stream << ", ";
    }

    stream << value;
    isFirst = false;
  }
  return stream << "]";
}
} // namespace operators

std::ostream& print(std::ostream& stream);

template <typename First, typename... Rest>
inline std::ostream& print(std::ostream& stream, First&& first, Rest&&... rest)
{
  using namespace operators;

  stream << first;
  return print(stream, std::forward<Rest>(rest)...);
}

template <typename First, typename... Rest>
inline std::ostream& println(std::ostream& stream, First&& first, Rest&&... rest)
{
  print(stream, first, std::forward<Rest>(rest)...);
  return stream << '\n';
}

} // namespace onnc

#endif
