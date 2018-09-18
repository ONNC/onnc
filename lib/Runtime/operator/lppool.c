#include <onnc/Runtime/operator/lppool.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static int32_t conver(
   int32_t * restrict meofarr
  ,int32_t ndim, const int32_t * restrict dims
){
	int32_t mul = 1, res = 0;
	for(int32_t i = ndim-1 ; i>=0 ; --i){
		res += meofarr[i] * mul ;
		mul *= dims[i] ;
	}
	return res;
}
static void Enu_In(
   const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,int32_t * restrict in_meofarr, int32_t in_idx
  ,int32_t * restrict kernel_shape
  ,int32_t p
  ,float *cnt, int32_t *cnt_navail
){
	if( in_idx == input_X_ndim ){
		if( !(*cnt_navail) ){
			*cnt += powf ( fabs( input_X[ conver( in_meofarr, input_X_ndim, input_X_dims ) ] ), p ) ;
		}
	} else {
		for(int32_t i = 0 ; i < kernel_shape[ in_idx -2 ] ; ++i){
			in_meofarr[ in_idx ] += i ;
			if( in_meofarr[ in_idx ] < 0 || in_meofarr[ in_idx ] >= input_X_dims[ in_idx ] ){
				(*cnt_navail)++;
			}
			Enu_In(
				input_X,
				input_X_ndim, input_X_dims,
				in_meofarr, in_idx + 1,
				kernel_shape,
				p,
				cnt, cnt_navail
			);
			if( in_meofarr[ in_idx ] < 0 || in_meofarr[ in_idx ] >= input_X_dims[ in_idx ] ){
				(*cnt_navail)--;
			}
			in_meofarr[ in_idx ] -= i ;
		}
	}
}
static void Enu_Out(
   const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,int32_t * restrict in_meofarr
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,int32_t * restrict ou_meofarr, int32_t ou_idx
  ,int32_t * restrict kernel_shape
  ,int32_t p
  ,int32_t * restrict pads
  ,int32_t * restrict strides
){
	if( ou_idx == output_Y_ndim ){
		float cnt = 0;
		int32_t cnt_navail = 0;
		in_meofarr[0] = ou_meofarr[0] ;
		in_meofarr[1] = ou_meofarr[1] ;
		for(int32_t i = 2 ; i < input_X_ndim  ; ++i){
			in_meofarr[i] = ou_meofarr[i] * strides[ i - 2 ] - pads[ i - 2 ] ;
		}
		Enu_In(
			input_X,
			input_X_ndim, input_X_dims,
			in_meofarr, 2,
			kernel_shape,
			p,
			&cnt, &cnt_navail
		);	
		output_Y[ conver( ou_meofarr, output_Y_ndim, output_Y_dims ) ] = powf(cnt, 1.0/p) ;
	} else {
		for(int32_t i = 0 ; i < output_Y_dims[ ou_idx ] ; ++i){
			ou_meofarr[ ou_idx ] += i ;
			Enu_Out(
				input_X,
				input_X_ndim, input_X_dims,
				in_meofarr,
				output_Y,
				output_Y_ndim, output_Y_dims,
				ou_meofarr,  ou_idx + 1,
				kernel_shape,
				p,
				pads,
				strides
			);
			ou_meofarr[ ou_idx ] -= i ;
		}
	}
}
void ONNC_RUNTIME_lppool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict auto_pad
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t p
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t * restrict strides
  ,int32_t number_of_strides
) {
	int32_t ou_meofarr[output_Y_ndim];
	for(int32_t i = 2 ; i < output_Y_ndim ; ++i){
		ou_meofarr[i] = 0;
	}
	int32_t in_meofarr[input_X_ndim];
	for(int32_t i = 0 ; i < output_Y_dims[0] ; ++i){
		for(int32_t j = 0 ; j < output_Y_dims[1] ; ++j){
			ou_meofarr[0] = i;
			ou_meofarr[1] = j;
			Enu_Out(
				input_X,
				input_X_ndim, input_X_dims,
				in_meofarr,
				output_Y,
				output_Y_ndim, output_Y_dims,
				ou_meofarr,  2,
				kernel_shape,
				p,
				pads,
				strides
			);
		}
	}
}
