#include <stdint.h>
#include <stdio.h>
#include "arm_math.h"
#include "cortexm_weight.h"
#include "math.h"
void pre_processing(int *image_data , q7_t* img_buffer){
    for(int i = 0 ; i < 28*28 ; i ++){
  img_buffer[i] = (image_data[i]>>1);
  }

}
