#ifndef VANILLA_WEIGHT_PACK_H
#define VANILLA_WEIGHT_PACK_H

#ifdef __cplusplus
extern "C" {
#endif

void CHW_to_HWC(float* input_data, int dims[4], float* output_data);
void CHW_to_HWC_mat(float* input_data, int dims[4], float* output_data);

int get_padding(unsigned int input_dim, unsigned int output_dim, unsigned int kernel_size, unsigned int stride,
                float auto_pad);

#ifdef __cplusplus
}
#endif

#endif
