#include "CortexmFileGenPass.h"
#include "CodeEmitVisitor.h"

#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

#include <string>

using namespace onnc;

struct code_list* first_code ;

CortexmFileGenPass::CortexmFileGenPass(TargetBackend *pBackend, CortexmBackendMeta *pMeta)
  : m_pBackend(pBackend), m_pMeta(m_pMeta){
}

Pass::ReturnType CortexmFileGenPass::runOnModule(Module& pModule){
  FILE *file;
  file = fopen("cortexm_out.cpp","w");
  
  //create include file
  fprintf(file,"#include <stdint.h>\n\
#include <stdio.h>\n\
#include \"arm_math.h\"\n\
#include \"cortexm_weight.h\"\n\
#include \"arm_nnfunctions.h\"\n\
#ifdef _RTE_\n\
#include \"RTE_Components.h\"\n\
#ifdef RTE_Compiler_EventRecorder\n\
#include \"EventRecorder.h\"\n\
#endif\n\
#endif\n\
#include \"cortexm_out.h\"\n\
#include \"matmul.h\"\n\
#include \"add.h\"\n\
#include \"mbed.h\"\n\
");
  //create weights array declaration
  int number_of_conv_layer = 0;
  int number_of_fc_layer = 0;
  int number_of_add_layer = 0;
  int number_of_matmul_layer = 0;
  int number_of_shape = 0;
  for(struct code_list* now_node = first_code; now_node!=NULL ; now_node = now_node ->next){
    if(now_node -> layer_type == TYPE_CONV){
      number_of_conv_layer++;
      fprintf(file,"static q7_t conv%d_wt[%d*%d*%d*%d] = CONV%d_WT;\n\
static q7_t conv%d_bias[%d] = CONV%d_BIAS;\n\n",
        number_of_conv_layer,
        now_node -> output_channel,
        now_node -> input_channel,
        now_node -> kernel_size,
        now_node -> kernel_size,
        number_of_conv_layer,
        number_of_conv_layer,
        now_node -> output_channel,
        number_of_conv_layer
      );
    }else if(now_node -> layer_type == TYPE_FULLYCONNECT){
      number_of_fc_layer++;
      fprintf(file,"static q7_t ip%d_wt[IP%d_DIM * IP%d_OUT] = IP%d_WT;\n\
static q7_t ip%d_bias[IP%d_OUT] = IP%d_BIAS;\n\n",
        number_of_fc_layer,
        number_of_fc_layer,
        number_of_fc_layer,
        number_of_fc_layer,
        number_of_fc_layer,
        number_of_fc_layer,
        number_of_fc_layer
      );
    }else if(now_node -> layer_type == TYPE_ADD){
      number_of_add_layer++;
      fprintf(file,"static q7_t add%d_wt[%d] = ADD%d;\n\
static int input_dims%d[%d] = INPUT_DIMS%d;\n\
static int add_dims%d[%d] = ADD_DIMS%d;\n\n",
        number_of_add_layer,
        now_node -> input_dimention,
        number_of_add_layer,
        number_of_add_layer,
        now_node -> input_channel,
        number_of_add_layer,
        number_of_add_layer,
        now_node -> output_channel,
        number_of_add_layer
      );
    }else if(now_node -> layer_type == TYPE_MATMUL){
      number_of_matmul_layer++;
      number_of_shape++;
      fprintf(file,"static q7_t matmul%d_wt[%d] = MATMUL_WEIGHT%d;\n\n",
        number_of_matmul_layer,
        now_node -> input_dimention,
        number_of_matmul_layer
      );
      fprintf(file,"static int shape%d_wt[2] = SHAPE%d;\n\
static int shape%d_wt[2] = SHAPE%d;\n\n",
        number_of_shape,
        number_of_shape,
        number_of_shape + 1,
        number_of_shape + 1 
      );
      number_of_shape++;
    }
  }
  
  //buffer size declaration
  fprintf(file,"q7_t output_data[10];\n\
q7_t col_buffer[2*5*5*32*2];\n\
q7_t scratch_buffer[%d*%d*%d];\n\
q7_t scratch_buffer2[%d*%d*%d];\n\n",
    first_code -> output_channel,
    first_code -> input_dimention,
    first_code -> input_dimention,
    first_code -> output_channel,
    first_code -> input_dimention,
    first_code -> input_dimention
  );
  
  fprintf(file,"q7_t* cortexm_main(int* image_data){\n\
  #ifdef RTE_Compiler_EventRecorder\n\
    EventRecorderInitialize (EventRecordAll, 1);\n\
  #endif\n\n");
  
  //buffer declaration
  fprintf(file,"  q7_t *img_buffer1 = scratch_buffer;\n\
  q7_t *img_buffer2 = scratch_buffer2;\n\n");

  fprintf(file,"  for(int loop = 0 ; loop<%d ; loop++ ){\n\
      img_buffer2[loop] = image_data[loop];\n\
    }\n",first_code -> output_dimention*first_code -> output_dimention);

  //create layer function call  
  number_of_conv_layer = 0;
  number_of_fc_layer = 0;
  number_of_add_layer = 0;
  number_of_matmul_layer = 0;
  number_of_shape = 0;
  int number_of_shift = 0;
  std::string final_output_buffer;
  for(struct code_list* now_node = first_code; now_node!=NULL ; now_node = now_node ->next){
    switch (now_node -> layer_type){
      case TYPE_CONV :
        number_of_conv_layer++;
        number_of_shift++;
        if(now_node -> input_channel == 3){
          fprintf(file,"  arm_convolve_HWC_q7_RGB( %s,%d,%d,conv%d_wt,%d,%d,%d,%d,conv%d_bias,0,RIGHT_SHIFT%d,%s,%d,(q15_t *)col_buffer,NULL);\n\n",
            input_buffer(now_node -> buffer_order),
	          now_node -> input_dimention,
            now_node -> input_channel,
            number_of_conv_layer,
            now_node -> output_channel,
            now_node -> kernel_size,
            now_node -> pad,
            now_node -> stride,
            number_of_conv_layer,
            number_of_shift,//
            output_buffer(now_node -> buffer_order),
            now_node -> output_dimention
          );
        }else if( ((now_node -> input_channel)%4 == 0) && ((now_node -> output_channel)%2 == 0) ){
          fprintf(file,"  arm_convolve_HWC_q7_basic( %s,%d,%d,conv%d_wt,%d,%d,%d,%d,conv%d_bias,0,RIGHT_SHIFT%d,%s,%d,(q15_t *)col_buffer,NULL );\n\n",
            input_buffer(now_node -> buffer_order),
            now_node -> input_dimention,
            now_node -> input_channel,
            number_of_conv_layer,
            now_node -> output_channel,
            now_node -> kernel_size,
            now_node -> pad,
            now_node -> stride,
            number_of_conv_layer,
            number_of_shift,//
            output_buffer(now_node -> buffer_order),
            now_node -> output_dimention
          );
        }else{
	        fprintf(file,"  arm_convolve_HWC_q7_basic( %s,%d,%d,conv%d_wt,%d,%d,%d,%d,conv%d_bias,0,RIGHT_SHIFT%d,%s,%d,(q15_t *)col_buffer,NULL );\n\n",
            input_buffer(now_node -> buffer_order),
            now_node -> input_dimention,
            now_node -> input_channel,
            number_of_conv_layer,
            now_node -> output_channel,
            now_node -> kernel_size,
            now_node -> pad,
            now_node -> stride,
            number_of_conv_layer,
            number_of_shift,//
            output_buffer(now_node -> buffer_order),
            now_node -> output_dimention
          );
	      }
        break;
      case TYPE_MAXPOOLING :
        fprintf(file,"  arm_maxpool_q7_HWC( %s,%d,%d,%d,%d,%d,%d,NULL,%s );\n\n",
          input_buffer(now_node -> buffer_order),
          now_node -> input_dimention,
          now_node -> input_channel,
          now_node -> kernel_size,
          now_node -> pad,
          now_node -> stride,
          now_node -> output_dimention,
          output_buffer(now_node -> buffer_order)
        );
        break;
      case TYPE_RELU :
        fprintf(file,"  arm_relu_q7( %s,%d * %d * %d );\n\n",
          input_buffer(now_node -> buffer_order),
          now_node -> output_channel,
          now_node -> output_dimention,
          now_node -> output_dimention
        );
        break;
      case TYPE_SOFTMAX :
	        fprintf(file,"  arm_softmax_q7(output_data,10,output_data);\n\n");
        break;
      case TYPE_FULLYCONNECT :
      	number_of_fc_layer++;
      	fprintf(file,"  arm_fully_connected_q7_opt(%s,ipt_wt,%d * %d * %d,10,IP%d_BIASS_LSHIFT,IP%d_OUT_RSHIFT,ip%d_bias,out_data,(q15_t *)%s);\n\n",
          input_buffer(now_node -> buffer_order),
          now_node -> input_channel,
          now_node -> output_dimention,
          now_node -> output_dimention,
          number_of_fc_layer,
          number_of_fc_layer,
          number_of_fc_layer,
          output_buffer(now_node -> buffer_order)
        );
        break;
      case TYPE_ADD :
        number_of_add_layer++;
        number_of_shift++;
        fprintf(file,"  MatAdd(%s,input_dims%d,add%d_wt,add_dims%d,%s,%d,RIGHT_SHIFT%d,RIGHT_SHIFT%d);\n\n",
          input_buffer(now_node -> buffer_order),
          number_of_add_layer,
          number_of_add_layer,
          number_of_add_layer,
          output_buffer(now_node -> buffer_order),
          now_node -> input_channel,
          number_of_shift,
          number_of_shift+1
        );
        number_of_shift++;
        break;
      case TYPE_MATMUL :
        number_of_matmul_layer++;
        number_of_shape++;
        number_of_shift++;
        fprintf(file,"  matmul(%s,shape%d_wt,matmul%d_wt,shape%d_wt,%s,RIGHT_SHIFT%d);\n\n",
        input_buffer(now_node -> buffer_order),
        number_of_shape,
        number_of_matmul_layer,
        number_of_shape + 1,
        output_buffer(now_node -> buffer_order),
        number_of_shift
        );
        number_of_shape++;
        break;
      default:
	      fprintf(file," // This layer is not support\n\n");
        break;
    }
    if(now_node -> next == NULL){
      if(now_node -> layer_type == TYPE_FULLYCONNECT){
        final_output_buffer = "output_data";
      }else if(now_node -> layer_type == TYPE_RELU){
        final_output_buffer = input_buffer(now_node -> buffer_order);
      }else{
        final_output_buffer = output_buffer(now_node -> buffer_order);
      }
    }
  }
  

  fprintf(file,"  return img_buffer2;\n");
  fprintf(file,"}\n");
  return Pass::kModuleNoChanged;
}

