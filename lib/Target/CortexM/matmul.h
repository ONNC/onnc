#ifndef MATMUL_H
#define MATMUL_H
#include "arm_math.h"
void matmul(q7_t* input_1,int input_1_dim[2],q7_t* input_2,int input_2_dim[2],q7_t* output,int right_shift);//dim[0] is high , dim[1] is weight

#endif
