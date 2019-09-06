#ifndef ONNC_TRANSPOSE

#include "size.h"
#include "strides.h"
/*!
 * \brief Permutation of index vector
 *
 * The permutation specifier is the same as described by ONNX `Transpose`
 * operator, which is an array of indices.  For example, when
 * `permutation = { 1, 0, 2 }` and `index = { a, b, c }`, this function writes
 * the permuted index `{ b, a, c }` to `result`.
 */
static void onnc_permute(
    ONNC_INDEX_TYPE* restrict result,
    const ONNC_INDEX_TYPE* restrict permutation,
    const ONNC_INDEX_TYPE* restrict index,
    ONNC_INDEX_TYPE order)
{
    for (ONNC_INDEX_TYPE i = 0; i < order; ++i)
        result[i] = index[permutation[i]];
}

/*!
 * \brief Tensor transpose
 *
 * This is generalization of matrix transpose to multidimensional space.
 * As there is more than one non-trivial transpose in 3+ ordered tensors,
 * transpose is described as permutation of indices.  See onnc_permute() for
 * details of index permutation.
 */
#define ONNC_TRANSPOSE(SCALAR, y, x, permutation, shape, order) do { \
    typedef SCALAR Scalar;                                           \
    typedef ONNC_INDEX_TYPE Index;                                   \
                                                                     \
    Scalar* restrict _y = y;                                         \
    const Scalar* restrict _x = x;                                   \
    const Index* restrict _permutation = permutation;                \
    const Index* restrict _shape = shape;                            \
    Index _order = order;                                            \
                                                                     \
    Index _size = onnc_size(_shape, _order);                         \
    Index _index[_order], _permuted[_order], _strides[_order];       \
                                                                     \
    for (Index _i = 0; _i < _order; ++_i)                            \
        _index[_i] = 0;                                              \
                                                                     \
    onnc_strides(_permuted, _shape, _order);                         \
    onnc_permute(_strides, _permutation, _permuted, _order);         \
    onnc_permute(_permuted, _permutation, _shape, _order);           \
                                                                     \
    for (Index _i = 0; _i < _size; ++_i) {                           \
        _y[_i] = _x[onnc_idot(_index, _strides, _order)];            \
        onnc_increment(_index, _permuted, _order);                   \
    }                                                                \
} while (0)

#endif
// vim: ft=c
