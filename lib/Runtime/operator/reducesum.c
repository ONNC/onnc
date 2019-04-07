#include <onnc/Runtime/operator/reducesum.h>

#include <stdint.h>
#include <stdbool.h>

static int32_t con(
	int32_t *meofarr,
	int32_t A_ndim, int32_t *A_dims
){
	int32_t mul = 1, res = 0 ;
	for(int32_t i = A_ndim - 1 ; i >= 0 ; --i){
		res += meofarr[i] * mul ;
		mul *= A_dims[i] ;
	}
	return res ;
}
static void Reduce(
	float *A, float *B,
	int32_t A_ndim,
	int32_t *A_dims,
	int32_t axis,
	int32_t *meofarr, int32_t idx
){
	if( idx == A_ndim ){
		meofarr[ axis ] = 0 ;
		int32_t bef_red = A_dims[ axis ] ;
		A_dims[ axis ] = 1 ;
		int32_t B_idx = con( meofarr, A_ndim, A_dims ) ;
		A_dims[ axis ] = bef_red ;
		B[ B_idx ] = 0 ;
		if(axis==2){
		}
		for(int32_t i = 0 ; i < A_dims[axis] ; ++i){
			meofarr[axis] = i ;
			B[ B_idx ] += A[ con( meofarr, A_ndim, A_dims ) ];
		}
	}
	else if( idx == axis ){
		Reduce(
			A, B,
			A_ndim,
			A_dims,
			axis,
			meofarr, idx+1
		);
	} else {
		for(int32_t i = 0 ; i < A_dims[idx] ; ++i){
			meofarr[idx] = i ;
			Reduce(
				A, B,
				A_ndim,
				A_dims,
				axis,
				meofarr, idx+1
			);
		}
	}
}

#include "internal/reducesum.inc"