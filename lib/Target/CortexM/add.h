#ifndef ADD_H
#define ADD_H

#include "arm_math.h"

void MatAdd(q7_t* input,int* input_dim,q7_t* add,int* add_dim,q7_t* output,int number_of_input_dim,int right_shift,int add_shift);

#endif
