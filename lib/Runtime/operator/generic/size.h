#ifndef ONNC_RUNTIME_SIZE_H
#define ONNC_RUNTIME_SIZE_H
/*!
 * \brief Count elements in a tensor
 *
 * This function returns count of elements given shape of a tensor.
 */
static ONNC_INDEX_TYPE onnc_size(const ONNC_INDEX_TYPE* shape, ONNC_INDEX_TYPE order)
{
    ONNC_INDEX_TYPE result = 1;

    for (ONNC_INDEX_TYPE i = 0; i < order; ++i)
        result *= shape[i];

    return result;
}

#endif
// vim: ft=c
