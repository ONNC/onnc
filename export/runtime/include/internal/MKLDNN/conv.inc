#include<stdio.h>
#include<stdlib.h>
void ONNC_RUNTIME_conv_float(void* restrict onnc_runtime_context, const float* restrict input_X, int32_t input_X_ndim,
        const int32_t* restrict input_X_dims, const float* restrict input_W, int32_t input_W_ndim,
        const int32_t* restrict input_W_dims, const float* restrict input_B, int32_t input_B_ndim,
        const int32_t* restrict input_B_dims, float* restrict output_Y, int32_t output_Y_ndim,
        const int32_t* restrict output_Y_dims, const char* restrict auto_pad,
        int32_t* restrict dilations, int32_t number_of_dilations, int32_t group,
        int32_t* restrict kernel_shape, int32_t number_of_kernel_shape, int32_t* restrict pads,
        int32_t number_of_pads, int32_t* restrict strides, int32_t number_of_strides)
{
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
    if(input_B!=NULL)
        mkldnn_memory_desc_init_by_strides(&bias_desc , 1, bias_dims, mkldnn_f32, NULL);
    mkldnn_dim_t dilation_dim[3], strides_dim[3], padding_l_dim[3], padding_r_dim[3];
    for(int i=0;i<ndims;i++){
        dilation_dim[i] = dilations[i]-1;
        strides_dim[i] = strides[i];
        padding_l_dim[i] = pads[i];
        padding_r_dim[i] = pads[i+ndims];
    }
    if(input_B!=NULL)
        CHECK(mkldnn_dilated_convolution_forward_desc_init(&conv_any_desc, mkldnn_forward,
                mkldnn_convolution_direct, &src_desc, &weight_desc,
                &bias_desc, &dst_desc, strides_dim, dilation_dim, padding_l_dim,
                padding_r_dim));
    else
        CHECK(mkldnn_dilated_convolution_forward_desc_init(&conv_any_desc, mkldnn_forward,
                mkldnn_convolution_direct, &src_desc, &weight_desc,
                NULL, &dst_desc, strides_dim, dilation_dim, padding_l_dim,
                padding_r_dim));

    mkldnn_primitive_desc_t conv_pd;

    CHECK(mkldnn_primitive_desc_create(
        &conv_pd, &conv_any_desc, NULL, engine, NULL));
    mkldnn_memory_t src_memory, weight_memory, dst_memory, bias_memory;
    CHECK(mkldnn_memory_create(&src_memory, &src_desc, engine, (void*)input_X));
    CHECK(mkldnn_memory_create(&weight_memory, &weight_desc, engine, (void*)input_W));
    CHECK(mkldnn_memory_create(&bias_memory, &bias_desc, engine, (void*)input_B));
    CHECK(mkldnn_memory_create(&dst_memory, &src_desc, engine, (void*)output_Y));
    mkldnn_primitive_t conv;
    CHECK(mkldnn_primitive_create(&conv, conv_pd));

    args_t args;
    prepare_arg_node(&args, 4);
    set_arg(&args.args[0], MKLDNN_ARG_SRC, src_memory);
    set_arg(&args.args[1], MKLDNN_ARG_WEIGHTS, weight_memory);
    set_arg(&args.args[2], MKLDNN_ARG_BIAS, bias_memory);
    set_arg(&args.args[3], MKLDNN_ARG_DST, dst_memory);
    CHECK(mkldnn_primitive_execute(conv, stream, args.nargs, args.args));
    CHECK(mkldnn_stream_wait(stream));
}
