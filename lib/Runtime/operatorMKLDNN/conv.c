#include <onnc/Runtime/operatorMKLDNN/conv.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

mkldnn_engine_t engine;
mkldnn_stream_t stream;

static inline bool next_dim(int32_t ndim, int32_t * restrict dim,
                            const int32_t * restrict dim_max) {
  do {
    ndim = ndim - 1;
    dim[ndim] += 1;
    if (dim[ndim] < dim_max[ndim]) {
      return true;
    } else { // reach dimension max
      if (ndim == 0) { // all dimension done
        return false;
      }
      dim[ndim] = 0;
    }
  } while(true);
}

static inline int64_t dim_to_offset(int32_t ndim, const int32_t * restrict dim,
                                    const int32_t * restrict dim_max) {
  int64_t offset = 0;
  int64_t step = 1;
  for (int32_t i = ndim - 1; i >= 0; --i) {
    offset += dim[i] * step;
    step *= dim_max[i];
  }
  return offset;
}

// If it is outside the bounds of the input, use 0.
static inline float get_value_or_zero(int32_t ndim, const int32_t * restrict dim_max,
                                      const float * restrict value, const int32_t * restrict dim) {
  for (int32_t i = 0; i < ndim; ++i) {
    if (dim[i] < 0 || dim[i] >= dim_max[i]) {
      return 0.f;
    }
  }
  return value[dim_to_offset(ndim, dim, dim_max)];
}

void ONNC_RUNTIME_conv_2d_float(void * restrict onnc_runtime_context,
                                int32_t N, int32_t C, int32_t iH, int32_t iW,
                                const float X[restrict N][C][iH][iW],
                                int32_t M, int32_t kC, int32_t kH, int32_t kW,
                                const float W[restrict M][kC][kH][kW],
                                const float B[restrict M],
                                int32_t oN, int32_t oC, int32_t oH, int32_t oW,
                                float Y[restrict oN][oC][oH][oW],
                                int32_t auto_pad,
                                const int32_t * restrict dilations,
                                int32_t group,
                                const int32_t * restrict kernel_shape,
                                const int32_t * restrict pads,
                                const int32_t * restrict strides) {
  // TODO: auto_pad
  // TODO: type
  for (int32_t n = 0; n < oN; ++n) {
    for (int32_t c = 0; c < oC; ++c) {

      for (int32_t h = 0; h < oH; ++h) {
        for (int32_t w = 0; w < oW; ++w) {

          int32_t base_c = (c * group / M) * kC; // input channel <-group-> output channel
          int32_t base_h = h * strides[0] - pads[0];
          int32_t base_w = w * strides[1] - pads[1];

          float sum = 0.f;

          for (int32_t i = (base_h < 0 ? (-base_h) / dilations[0] : 0); i < kH; ++i) {
            int32_t input_h = base_h + i * dilations[0];
            if (input_h >= iH) { break; }
            for (int32_t j =  (base_w < 0 ? (-base_w) / dilations[1] : 0); j < kW; ++j) {
              int32_t input_w = base_w + j * dilations[1];
              if (input_w >= iW) { break; }
              for (int32_t w_channel = 0; w_channel < kC; ++w_channel) {
                int32_t input_channel = base_c + w_channel;

                float input = X[n][input_channel][input_h][input_w];
                float weight = W[c][w_channel][i][j];
                sum += input * weight;
              }
            }
          }

          if (B != NULL) {
            sum += B[c];
          }
          Y[n][c][h][w] = sum;
        }
      }
    }
  }
}

void ONNC_RUNTIME_conv_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_W
  ,int32_t input_W_ndim, const int32_t * restrict input_W_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict auto_pad
  ,int32_t * restrict dilations
  ,int32_t number_of_dilations
  ,int32_t group
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t * restrict strides
  ,int32_t number_of_strides
) {

  mkldnn_engine_create(&engine, mkldnn_cpu, 0);
  mkldnn_stream_create(&stream, engine, mkldnn_stream_default_flags);
  mkldnn_convolution_desc_t conv_any_desc;
  int ndims = input_X_ndim - 2;
  mkldnn_memory_desc_t src_desc, weight_desc, bias_desc, dst_desc;
  mkldnn_dim_t src_dims[5], weight_dims[6], bias_dims[1], dst_dims[5], dilates[3], padding_l[3], padding_r[3];
  weight_dims[0]=group;
  for(int i=0;i<ndims+2;i++){
    src_dims[i]=input_X_dims[i];
    weight_dims[i+1]=input_W_dims[i];
    dst_dims[i]=output_Y_dims[i];
  }
  if(input_B!=NULL)
    bias_dims[0] = input_B_dims[0];
  weight_dims[1]/=weight_dims[0];
  switch(ndims){
    case 1:
      mkldnn_memory_desc_init_by_tag(&src_desc, ndims+2, src_dims, mkldnn_f32, mkldnn_ncw);
      mkldnn_memory_desc_init_by_tag(&weight_desc, ndims+3, weight_dims, mkldnn_f32, mkldnn_goiw);
      mkldnn_memory_desc_init_by_tag(&dst_desc, ndims+2, dst_dims, mkldnn_f32, mkldnn_ncw);
      break;
    case 2:
      mkldnn_memory_desc_init_by_tag(&src_desc, ndims+2, src_dims, mkldnn_f32, mkldnn_nchw);
      mkldnn_memory_desc_init_by_tag(&weight_desc, ndims+3, weight_dims, mkldnn_f32, mkldnn_goihw);
      mkldnn_memory_desc_init_by_tag(&dst_desc, ndims+2, dst_dims, mkldnn_f32, mkldnn_nchw);
      break;
    case 3:
      mkldnn_memory_desc_init_by_tag(&src_desc, ndims+2, src_dims, mkldnn_f32, mkldnn_ncdhw);
      mkldnn_memory_desc_init_by_tag(&weight_desc, ndims+3, weight_dims, mkldnn_f32, mkldnn_goidhw);
      mkldnn_memory_desc_init_by_tag(&dst_desc, ndims+2, dst_dims, mkldnn_f32, mkldnn_ncdhw);
      break;
  }
  if(input_B!=NULL){
    mkldnn_memory_desc_init_by_strides(&bias_desc , 1, bias_dims, mkldnn_f32, NULL);
  }
  mkldnn_dim_t dilation_dim[3], strides_dim[3], padding_l_dim[3], padding_r_dim[3];
  for(int i=0;i<ndims;i++){
    dilation_dim[i] = dilations[i]-1;
    strides_dim[i] = strides[i];
    padding_l_dim[i] = pads[i];
    padding_r_dim[i] = pads[i+ndims];
  }
  if(input_B!=NULL)
    mkldnn_dilated_convolution_forward_desc_init(&conv_any_desc, mkldnn_forward,
          mkldnn_convolution_direct, &src_desc, &weight_desc,
          &bias_desc, &dst_desc, strides_dim, dilation_dim, padding_l_dim,
          padding_r_dim);
  else
    mkldnn_dilated_convolution_forward_desc_init(&conv_any_desc, mkldnn_forward,
          mkldnn_convolution_direct, &src_desc, &weight_desc,
          NULL, &dst_desc, strides_dim, dilation_dim, padding_l_dim,
          padding_r_dim);

  mkldnn_primitive_desc_t conv_pd;

  mkldnn_primitive_desc_create(
        &conv_pd, &conv_any_desc, NULL, engine, NULL);
  mkldnn_memory_t src_memory, weight_memory, dst_memory, bias_memory;
  mkldnn_memory_create(&src_memory, &src_desc, engine, (void*)input_X);
  mkldnn_memory_create(&weight_memory, &weight_desc, engine, (void*)input_W);
  mkldnn_memory_create(&bias_memory, &bias_desc, engine, (void*)input_B);
  mkldnn_memory_create(&dst_memory, &src_desc, engine, (void*)output_Y);
  mkldnn_primitive_t conv;
  mkldnn_primitive_create(&conv, conv_pd);

  args_t args;
  prepare_arg_node(&args, 4);
  set_arg(&args.args[0], MKLDNN_ARG_SRC, src_memory);
  set_arg(&args.args[1], MKLDNN_ARG_WEIGHTS, weight_memory);
  set_arg(&args.args[2], MKLDNN_ARG_BIAS, bias_memory);
  set_arg(&args.args[3], MKLDNN_ARG_DST, dst_memory);
  mkldnn_primitive_execute(conv, stream, args.nargs, args.args);
  mkldnn_stream_wait(stream);

  mkldnn_stream_destroy(stream);
  mkldnn_engine_destroy(engine);

}
