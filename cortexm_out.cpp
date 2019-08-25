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
#include "cortexm_out.h"
#include "matmul.h"
#include "add.h"
#if defined(ONNC_PC_SIM)
#else
	#include "mbed.h"
#endif
static q7_t conv1_wt[1*1*3*3] = CONV1_WT;
static q7_t conv1_bias[1] = CONV1_BIAS;

q7_t output_data[10];
q7_t col_buffer[2*5*5*32*2];
q7_t scratch_buffer[1*6*6];
q7_t scratch_buffer2[1*6*6];

q7_t* cortexm_main(int* image_data){
  #ifdef RTE_Compiler_EventRecorder
    EventRecorderInitialize (EventRecordAll, 1);
  #endif

  q7_t *img_buffer1 = scratch_buffer;
  q7_t *img_buffer2 = scratch_buffer2;

  for(int loop = 0 ; loop<36 ; loop++ ){
      img_buffer2[loop] = image_data[loop];
    }
  arm_convolve_HWC_q7_basic( img_buffer2,6,1,conv1_wt,1,3,0,1,conv1_bias,0,RIGHT_SHIFT1,img_buffer1,4,(q15_t *)col_buffer,NULL );

 return img_buffer1;
}
