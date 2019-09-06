#ifndef ONNCRT_TEST_VALARRAY_HPP
#define ONNCRT_TEST_VALARRAY_HPP

#include <algorithm>
#include <iterator>
#include <valarray>
#include <cstring>

namespace onnc {
namespace valarray {

template<typename Container>
static std::valarray<std::size_t> strides(const Container& shape)
{
    std::size_t order = shape.size();
    std::valarray<std::size_t> result(order);

    if (order > 0)
        result[order - 1] = 1;

    for (std::size_t i = order - 1; i > 0; --i)
        result[i - 1] = shape[i] * result[i];

    for (std::size_t i = 0; i < order; ++i)
        result[i] *= shape[i] > 1;

    return result;
}

template<typename T>
static std::valarray<T> range(std::size_t end)
{
    std::valarray<T> result(end);

    for (std::size_t i = 0; i < end; ++i)
        result[i] = i;

    return result;
}

template<typename T, typename Container>
static std::valarray<T> make(const Container& x)
{
    std::valarray<T> result(x.size());
    std::copy(std::begin(x), std::end(x), std::begin(result));
    return result;
}

template<typename T>
static bool equal(const std::valarray<T>& x, const std::valarray<T>& y)
{
    return x.size() == y.size() && !std::memcmp(&x[0], &y[0], x.size() * sizeof(T));
}

} // namespace valarray
} // namespace onnc
#endif
