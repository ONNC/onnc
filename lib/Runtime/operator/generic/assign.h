#ifndef ONNC_ASSIGN

#include "size.h"
#include "strides.h"
#include <string.h>
/*!
 * \brief Tensor assignment
 *
 * This function performs `y .= x` where `x` is broadcastable to `y`.
 *
 * For example, if `y` is a 3x4 matrix and `x = [a b c d]`, resulting `y`
 * would be
 *
 *     [a  b  c  d;
 *      a  b  c  d;
 *      a  b  c  d]
 */
#define ONNC_ASSIGN(SCALAR, y, yshape, yorder, x, xshape, xorder) do { \
    typedef SCALAR Scalar;                                             \
    typedef ONNC_INDEX_TYPE Index;                                     \
                                                                       \
    Scalar* restrict _y = y;                                           \
    const Scalar* restrict _x = x;                                     \
    const Index* restrict _yshape = yshape;                            \
    const Index* restrict _xshape = xshape;                            \
    Index _xorder = xorder;                                            \
    Index _diff = (Index)(yorder) - _xorder;                           \
    Index _size = onnc_size(_yshape + _diff, _xorder);                 \
    Index _count = onnc_size(_yshape, _diff);                          \
    Index _strides[_xorder];                                           \
    Index _index[_xorder];                                             \
                                                                       \
    onnc_strides(_strides, _xshape, _xorder);                          \
                                                                       \
    for (Index _i = 0; _i < _xorder; ++_i)                             \
        _index[_i] = 0;                                                \
                                                                       \
    for (Index _i = 0; _i < _size; ++_i) {                             \
        _y[_i] = _x[onnc_idot(_index, _strides, _xorder)];             \
        onnc_increment(_index, _yshape + _diff, _xorder);              \
    }                                                                  \
                                                                       \
    for (Index _i = 1; _i < _count; ++_i)                              \
        memcpy(_y + _i * _size, _y, _size * sizeof(Scalar));           \
} while (0)

#endif
// vim: ft=c
