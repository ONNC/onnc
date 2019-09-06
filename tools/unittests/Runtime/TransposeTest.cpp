#define restrict __restrict__
extern "C" {
#include <onnc/Runtime/operator/transpose.h>
}
#undef restrict

#include "valarray.hpp"
#include <skypat/skypat.h>
#include <algorithm>
#include <functional>
#include <numeric>
#include <array>

/* FIXME We pass `permutation` by value because `ONNC_RUNTIME_transpose_float`
 * takes (non-const) `int32_t*` as permutation list.  Discuss if changing to
 * `const int32_t*` is possible.
 */
static void test_permutedims(
    const std::valarray<float>& tensor,
    std::valarray<std::int32_t> permutation,
    const std::valarray<std::int32_t>& shape,
    const std::valarray<std::size_t>& strides)
{
    using std::size_t;
    using std::int32_t;

    const std::valarray<size_t> permute = onnc::valarray::make<size_t>(permutation);
    const std::valarray<int32_t> permuted_shape = shape[permute];
    const std::valarray<size_t> permuted_strides = strides[permute];
    const std::gslice transpose = { 0, onnc::valarray::make<size_t>(permuted_shape), permuted_strides };
    const std::valarray<float> reference = tensor[transpose];

    std::valarray<float> candidate(tensor.size());

    ONNC_RUNTIME_transpose_float(nullptr,
        &tensor[0], shape.size(), &shape[0],
        &candidate[0], permuted_shape.size(), &permuted_shape[0],
        &permutation[0], permutation.size());

    EXPECT_TRUE(onnc::valarray::equal(reference, candidate));
}

template<typename... Args>
static void test(Args... args)
{
    using std::size_t;
    using std::int32_t;

    const size_t order = sizeof...(Args);
    std::array<int32_t, order> shape = { static_cast<int32_t>(args)... };
    std::valarray<size_t> strides = onnc::valarray::strides(shape);
    size_t size = std::accumulate(shape.cbegin(), shape.cend(), static_cast<size_t>(1), std::multiplies<size_t>());

    const std::gslice identity = { 0, onnc::valarray::make<size_t>(shape), strides };
    const std::valarray<float> tensor = onnc::valarray::range<float>(size);
    const std::valarray<float> candidate = tensor[identity];

    EXPECT_TRUE(onnc::valarray::equal(tensor, candidate));

    std::valarray<int32_t> permutation = onnc::valarray::range<int32_t>(order);

    while (std::next_permutation(std::begin(permutation), std::end(permutation)))
        test_permutedims(tensor, permutation, onnc::valarray::make<int32_t>(shape), strides);
}

SKYPAT_F(Operator_Transpose, non_broadcast)
{
    test(4);
    test(2, 5);
    test(3, 1);
    test(1, 2, 3);
    test(4, 2, 3);
    test(2, 3, 1, 2);
    test(3, 1, 3, 3, 7);
}
