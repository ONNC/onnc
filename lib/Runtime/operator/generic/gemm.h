#ifndef ONNC_GEMM
/*!
 * \brief Matrix multiplication
 *
 * This function performs `y += opa(a) * opb(b)` where `a` and `b` are matrices.
 * If `op` is nonzero, its operand is viewed as transposed.
 */
#define ONNC_GEMM(SCALAR, y, a, b, alpha, rows, cols, depth, opa, opb) do { \
    typedef SCALAR Scalar;                                                  \
    typedef ONNC_INDEX_TYPE Index;                                          \
                                                                            \
    Scalar* restrict _y = y;                                                \
    const Scalar* restrict _a = a;                                          \
    const Scalar* restrict _b = b;                                          \
    Scalar _alpha = alpha;                                                  \
    Index _rows = rows;                                                     \
    Index _cols = cols;                                                     \
    Index _depth = depth;                                                   \
    int _opa = !!(opa);                                                     \
                                                                            \
    Index _istride = _opa ? 1 : _depth;                                     \
    Index _kstride = _opa ? _rows : 1;                                      \
                                                                            \
    if (opb)                                                                \
        for (Index _i = 0; _i < _rows; ++_i)                                \
            for (Index _j = 0; _j < _cols; ++_j)                            \
                for (Index _k = 0; _k < _depth; ++_k)                       \
                    _y[_i * _cols + _j] += _alpha * _a[_i * _istride + _k * _kstride] * _b[_j * _depth + _k]; \
    else                                                                    \
        for (Index _i = 0; _i < _rows; ++_i)                                \
            for (Index _k = 0; _k < _depth; ++_k)                           \
                for (Index _j = 0; _j < _cols; ++_j)                        \
                    _y[_i * _cols + _j] += _alpha * _a[_i * _istride + _k * _kstride] * _b[_k * _cols + _j]; \
} while (0)

#endif
// vim: ft=c
