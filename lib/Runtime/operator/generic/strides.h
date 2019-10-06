#ifndef ONNC_RUNTIME_STRIDES_H
#define ONNC_RUNTIME_STRIDES_H
/*!
 * \brief Inner product of vectors of indices
 *
 * This function computes data offset from a multidimensional index.
 * See onnc_strides() for details.
 */
static ONNC_INDEX_TYPE onnc_idot(
    const ONNC_INDEX_TYPE* x,
    const ONNC_INDEX_TYPE* y,
    ONNC_INDEX_TYPE count)
{
    typedef ONNC_INDEX_TYPE Index;
    Index result = 0;

    for (Index i = 0; i < count; ++i)
        result += x[i] * y[i];

    return result;
}

/*!
 * \brief Increment 0-based multidimensional index in a tensor
 *
 * For example, in a 2x1x3 tensor, an index shall iterate through
 *
 *     { 0, 0, 0 }
 *     { 0, 0, 1 }
 *     { 0, 0, 2 }
 *     { 1, 0, 0 }
 *     { 1, 0, 1 }
 *     { 1, 0, 2 }
 */
static void onnc_increment(
    ONNC_INDEX_TYPE* restrict index,
    const ONNC_INDEX_TYPE* restrict shape,
    ONNC_INDEX_TYPE order)
{
    typedef ONNC_INDEX_TYPE Index;
    Index end = order - 1;

    if (order > 0 && ++index[end] >= shape[end]) {
        index[end] = 0;
        onnc_increment(index, shape, end);
    }
}

/*!
 * \brief Strides for each axis
 *
 * This function computes strides given shape of a tensor.
 *
 * For example, given a 3x1x3x3x7 tensor, its strides on each axis are
 *
 *     { 63, 0, 21, 7, 1 }
 *
 * In this case, we can access the element at `[a, b, c, d, e]` with
 *
 *     pointer[63*a + 21*c + 7*d + e]
 *
 * The stride on each 1-element-long axis is thought to be 0 to ease arbitrary
 * broadcasting.  For example, the example array of strides still works if the
 * tensor is read as a 3xnx3x3x7 tensor.
 */
static void onnc_strides(
    ONNC_INDEX_TYPE* restrict result,
    const ONNC_INDEX_TYPE* restrict shape,
    ONNC_INDEX_TYPE order)
{
    typedef ONNC_INDEX_TYPE Index;

    if (order > 0)
        result[order - 1] = 1;

    for (Index i = order - 1; i > 0; --i)
        result[i - 1] = shape[i] * result[i];

    for (Index i = 0; i < order; ++i)
        result[i] *= shape[i] > 1;
}

#endif
// vim: ft=c
