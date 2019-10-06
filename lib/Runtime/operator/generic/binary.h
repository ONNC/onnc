#ifndef ONNC_BINARY

#include "size.h"
#include "strides.h"
/*!
 * \brief Inplace binary operator
 *
 * This function performs `y = f.(y, x)` where `x` is broadcastable to `y`.
 * For example, if `f = +`, then `y .+= x` is performed.
 */
#define ONNC_BINARY(SCALAR, y, yshape, yorder, x, xshape, xorder, f) do {     \
    typedef SCALAR Scalar;                                                    \
    typedef ONNC_INDEX_TYPE Index;                                            \
                                                                              \
    Scalar* restrict _y = y;                                                  \
    const Scalar* restrict _x = x;                                            \
    const Index* restrict _yshape = yshape;                                   \
    const Index* restrict _xshape = xshape;                                   \
    Index _yorder = yorder;                                                   \
    Index _xorder = xorder;                                                   \
                                                                              \
    Index _diff = _yorder - _xorder;                                          \
    Index _size = onnc_size(_yshape, _yorder);                                \
    Index _index[_yorder];                                                    \
    Index _strides[_xorder];                                                  \
                                                                              \
    onnc_strides(_strides, _xshape, _xorder);                                 \
                                                                              \
    for (Index _i = 0; _i < _yorder; ++_i)                                    \
       _index[_i] = 0;                                                        \
                                                                              \
    for (Index _i = 0; _i < _size; ++_i) {                                    \
        _y[_i] = f(_y[_i], _x[onnc_idot(_index + _diff, _strides, _xorder)]); \
        onnc_increment(_index, _yshape, _yorder);                             \
    }                                                                         \
} while (0)

#endif
// vim: ft=c
