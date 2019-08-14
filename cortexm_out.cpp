#include <stdint.h>
#include <stdio.h>
#include "arm_math.h"
#include "cortexm_weight.h"
#include "arm_nnfunctions.h"
#ifdef _RTE_
#include "RTE_Components.h"
#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif
#endif

#include "cortexm_input_per_proc.h"
#include "cortexm_out.h"
#include "matmul.h"
#include "add.h"
#include "mbed.h"
static q7_t conv1_wt[8*1*5*5] = CONV1_WT;
static q7_t conv1_bias[8] = CONV1_BIAS;

static q7_t add1_wt[8] = ADD1;
static int input_dims1[4] = INPUT_DIMS1;
static int add_dims1[3] = ADD_DIMS1;

static q7_t conv2_wt[16*8*5*5] = CONV2_WT;
static q7_t conv2_bias[16] = CONV2_BIAS;

static q7_t add2_wt[16] = ADD2;
static int input_dims2[4] = INPUT_DIMS2;
static int add_dims2[3] = ADD_DIMS2;

static q7_t matmul1_wt[2560] = MATMUL_WEIGHT1;

static int shape1_wt[2] = SHAPE1;
static int shape2_wt[2] = SHAPE2;

static q7_t add3_wt[10] = ADD3;
static int input_dims3[2] = INPUT_DIMS3;
static int add_dims3[2] = ADD_DIMS3;

q7_t output_data[10];
q7_t col_buffer[2*5*5*32*2];
q7_t scratch_buffer[8*28*28];
q7_t scratch_buffer2[8*28*28];

int vanilla_main(bool input_pre_proc,int* image_data){
  #ifdef RTE_Compiler_EventRecorder
    EventRecorderInitialize (EventRecordAll, 1);
  #endif

  q7_t *img_buffer1 = scratch_buffer;
  q7_t *img_buffer2 = scratch_buffer2;

  if(input_pre_proc){
    per_processing(image_data , img_buffer2);
  }else{
    for(int loop = 0 ; loop<784 ; loop++ ){
      img_buffer2[loop] = image_data[loop];
    }
  }

  arm_convolve_HWC_q7_basic( img_buffer2,28,1,conv1_wt,8,5,0,1,conv1_bias,0,RIGHT_SHIFT1,img_buffer1,28,(q15_t *)col_buffer,NULL );

  MatAdd(img_buffer1,input_dims1,add1_wt,add_dims1,img_buffer2,4,RIGHT_SHIFT2,RIGHT_SHIFT3);

  arm_relu_q7( img_buffer2,8 * 28 * 28 );

  arm_maxpool_q7_HWC( img_buffer2,28,8,2,0,2,14,NULL,img_buffer1 );

  arm_convolve_HWC_q7_basic( img_buffer1,14,8,conv2_wt,16,5,0,1,conv2_bias,0,RIGHT_SHIFT4,img_buffer2,14,(q15_t *)col_buffer,NULL );

  MatAdd(img_buffer2,input_dims2,add2_wt,add_dims2,img_buffer1,4,RIGHT_SHIFT5,RIGHT_SHIFT6);

  arm_relu_q7( img_buffer1,16 * 14 * 14 );

  arm_maxpool_q7_HWC( img_buffer1,14,16,3,0,3,4,NULL,img_buffer2 );

  matmul(img_buffer2,shape1_wt,matmul1_wt,shape2_wt,img_buffer1,RIGHT_SHIFT7);

  MatAdd(img_buffer1,input_dims3,add3_wt,add_dims3,img_buffer2,2,RIGHT_SHIFT8,RIGHT_SHIFT9);

  int return_type = 0 , type_value = 0;
  for(int i = 0; i < 10 ; i++){
    if(type_value < img_buffer2[i]){
      type_value = img_buffer2[i];
      return_type = i;
    }
  }

  return return_type;
}
