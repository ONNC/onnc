#include "CortexmMainFileGenPass.h"

#include "CodeEmitVisitor.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

#include <cstring>
#include <string>

using namespace onnc;

CortexmMainFileGenPass::CortexmMainFileGenPass(const CortexmBackendMeta& pMeta)
  : m_pMeta(pMeta)
{}

Pass::ReturnType CortexmMainFileGenPass::runOnModule(Module& pModule)
{
  FILE* filePtr = fopen("cortexm_out.cpp", "w");

  if (!filePtr) {
    return Pass::kPassFailure;
  }

  // create include file
  fprintf(filePtr, "\
#include <stdint.h>\n\
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
#if defined(ONNC_PC_SIM)\n\
#else\n\t\
#include \"mbed.h\"\n\
#endif\n\
");
  // create weights array declaration
  int number_of_conv_layer    = 0;
  int number_of_fc_layer      = 0;
  int number_of_add_layer     = 0;
  int number_of_matmul_layer  = 0;
  int number_of_shape         = 0;
  int sum_of_operators        = 0;
  int number_of_maxpool_layer = 0;

  auto firstLayer = m_pMeta.m_layerList.front();
  for (auto layerItr = m_pMeta.m_layerList.begin(); layerItr != m_pMeta.m_layerList.end(); ++layerItr) {
    if (layerItr->layer_type == TYPE_CONV) {
      number_of_conv_layer++;
      sum_of_operators++;
      fprintf(filePtr, "static q7_t conv%d_wt[%d*%d*%d*%d] = CONV%d_WT;\n\
static q7_t conv%d_bias[%d] = CONV%d_BIAS;\n\n",
              number_of_conv_layer, layerItr->output_channel, layerItr->input_channel, layerItr->kernel_size,
              layerItr->kernel_size, number_of_conv_layer, number_of_conv_layer, layerItr->output_channel,
              number_of_conv_layer);
    } else if (layerItr->layer_type == TYPE_FULLYCONNECT) {
      number_of_fc_layer++;
      fprintf(filePtr, "static q7_t ip%d_wt[IP%d_DIM * IP%d_OUT] = IP%d_WT;\n\
static q7_t ip%d_bias[IP%d_OUT] = IP%d_BIAS;\n\n",
              number_of_fc_layer, number_of_fc_layer, number_of_fc_layer, number_of_fc_layer, number_of_fc_layer,
              number_of_fc_layer, number_of_fc_layer);
    } else if (layerItr->layer_type == TYPE_ADD) {
      number_of_add_layer++;
      fprintf(filePtr, "static q7_t add%d_wt[%d] = ADD%d;\n\
static int input_dims%d[%d] = INPUT_DIMS%d;\n\
static int add_dims%d[%d] = ADD_DIMS%d;\n\n",
              number_of_add_layer, layerItr->weight_size, number_of_add_layer, number_of_add_layer,
              layerItr->input_size, number_of_add_layer, number_of_add_layer, layerItr->weight_dim_size,
              number_of_add_layer);
    } else if (layerItr->layer_type == TYPE_MATMUL) {
      number_of_matmul_layer++;
      number_of_shape++;
      fprintf(filePtr, "static q7_t matmul%d_wt[%d] = MATMUL_WEIGHT%d;\n\n", number_of_matmul_layer,
              layerItr->matmul_size, number_of_matmul_layer);
      fprintf(filePtr, "static int shape%d_wt[2] = SHAPE%d;\n\
static int shape%d_wt[2] = SHAPE%d;\n\n",
              number_of_shape, number_of_shape, number_of_shape + 1, number_of_shape + 1);
      number_of_shape++;
    }
  }

  // buffer size declaration
  fprintf(filePtr, "q7_t output_data[10];\n\
q7_t col_buffer[2*5*5*32*2];\n\
q7_t scratch_buffer[%d*%d*%d*%d];\n\
q7_t scratch_buffer2[%d*%d*%d*%d];\n\n",
          firstLayer.batch_size, firstLayer.output_channel, firstLayer.input_dimension, firstLayer.input_dimension,
          firstLayer.batch_size, firstLayer.output_channel, firstLayer.input_dimension, firstLayer.input_dimension);

  fprintf(filePtr, "q7_t* cortexm_main(int* image_data){\n\
  #ifdef RTE_Compiler_EventRecorder\n\
    EventRecorderInitialize (EventRecordAll, 1);\n\
  #endif\n\n");

  // buffer declaration
  fprintf(filePtr, "  q7_t *img_buffer1 = scratch_buffer;\n\
  q7_t *img_buffer2 = scratch_buffer2;\n\n");

  fprintf(filePtr, "  for(int loop = 0 ; loop<%d ; loop++ ){\n\
      img_buffer2[loop] = image_data[loop];\n\
}\n",
          firstLayer.batch_size * firstLayer.input_channel * firstLayer.input_dimension * firstLayer.input_dimension);
  // create layer function call
  number_of_conv_layer    = 0;
  number_of_maxpool_layer = 0;
  number_of_fc_layer      = 0;
  number_of_add_layer     = 0;
  number_of_matmul_layer  = 0;
  number_of_shape         = 0;

  int         number_of_shift = 0;
  std::string final_output_buffer;
  for (auto layerItr = m_pMeta.m_layerList.begin(); layerItr != m_pMeta.m_layerList.end(); ++layerItr) {
    switch (layerItr->layer_type) {
    case TYPE_CONV:
      number_of_conv_layer++;
      number_of_shift++;
      if (layerItr->input_channel == 3) {
        fprintf(filePtr,
                "  arm_convolve_HWC_q7_RGB( "
                "%s,%d,%d,conv%d_wt,%d,%d,%d,%d,conv%d_bias,0,RIGHT_"
                "SHIFT%d,%s,%d,(q15_t *)col_buffer,NULL);\n\n",
                input_buffer(layerItr->buffer_order), layerItr->input_dimension, layerItr->input_channel,
                number_of_conv_layer, layerItr->output_channel, layerItr->kernel_size, layerItr->pad, layerItr->stride,
                number_of_conv_layer, number_of_shift, output_buffer(layerItr->buffer_order),
                layerItr->output_dimension);
      } else if (((layerItr->input_channel) % 4 == 0) && ((layerItr->output_channel) % 2 == 0)) {
        fprintf(filePtr,
                "  arm_convolve_HWC_q7_basic( "
                "%s,%d,%d,conv%d_wt,%d,%d,%d,%d,conv%d_bias,0,RIGHT_"
                "SHIFT%d,%s,%d,(q15_t *)col_buffer,NULL );\n\n",
                input_buffer(layerItr->buffer_order), layerItr->input_dimension, layerItr->input_channel,
                number_of_conv_layer, layerItr->output_channel, layerItr->kernel_size, layerItr->pad, layerItr->stride,
                number_of_conv_layer, number_of_shift, output_buffer(layerItr->buffer_order),
                layerItr->output_dimension);
      } else {
        fprintf(filePtr,
                "  arm_convolve_HWC_q7_basic( "
                "%s,%d,%d,conv%d_wt,%d,%d,%d,%d,conv%d_bias,0,RIGHT_"
                "SHIFT%d,%s,%d,(q15_t *)col_buffer,NULL );\n\n",
                input_buffer(layerItr->buffer_order), layerItr->input_dimension, layerItr->input_channel,
                number_of_conv_layer, layerItr->output_channel, layerItr->kernel_size, layerItr->pad, layerItr->stride,
                number_of_conv_layer, number_of_shift, output_buffer(layerItr->buffer_order),
                layerItr->output_dimension);
      }
      break;
    case TYPE_MAXPOOLING:
      number_of_maxpool_layer++;
      fprintf(filePtr, "  arm_maxpool_q7_HWC( %s,%d,%d,%d,%d,%d,%d,NULL,%s );\n\n",
              input_buffer(layerItr->buffer_order), layerItr->input_dimension, layerItr->input_channel,
              layerItr->kernel_size, layerItr->pad, layerItr->stride, layerItr->output_dimension,
              output_buffer(layerItr->buffer_order));
      break;
    case TYPE_RELU:
      fprintf(filePtr, "  arm_relu_q7( %s,%d * %d * %d * %d );\n\n", input_buffer(layerItr->buffer_order),
              layerItr->batch_size, layerItr->output_channel, layerItr->output_dimension, layerItr->output_dimension);
      break;
    case TYPE_SOFTMAX:
      fprintf(filePtr, "  arm_softmax_q7(output_data,10,output_data);\n\n");
      break;
    case TYPE_FULLYCONNECT:
      number_of_fc_layer++;
      fprintf(filePtr,
              "  arm_fully_connected_q7_opt(%s,ipt_wt,%d * %d * "
              "%d,10,IP%d_BIASS_LSHIFT,IP%d_OUT_RSHIFT,ip%d_bias,out_"
              "data,(q15_t *)%s);\n\n",
              input_buffer(layerItr->buffer_order), layerItr->input_channel, layerItr->output_dimension,
              layerItr->output_dimension, number_of_fc_layer, number_of_fc_layer, number_of_fc_layer,
              output_buffer(layerItr->buffer_order));
      break;
    case TYPE_ADD:
      number_of_add_layer++;
      number_of_shift++;
      fprintf(filePtr,
              "  "
              "MatAdd(%s,input_dims%d,add%d_wt,add_dims%d,%s,%d,RIGHT_"
              "SHIFT%d,RIGHT_SHIFT%d);\n\n",
              input_buffer(layerItr->buffer_order), number_of_add_layer, number_of_add_layer, number_of_add_layer,
              output_buffer(layerItr->buffer_order), layerItr->input_size, number_of_shift, number_of_shift + 1);
      number_of_shift++;
      break;
    case TYPE_MATMUL:
      number_of_matmul_layer++;
      number_of_shape++;
      number_of_shift++;
      fprintf(filePtr,
              "  "
              "matmul(%s,shape%d_wt,matmul%d_wt,shape%d_wt,%s,RIGHT_"
              "SHIFT%d);\n\n",
              input_buffer(layerItr->buffer_order), number_of_shape, number_of_matmul_layer, number_of_shape + 1,
              output_buffer(layerItr->buffer_order), number_of_shift);
      number_of_shape++;
      break;
    default:
      fprintf(filePtr, " // This layer is not support\n\n");
      break;
    }
    if (layerItr + 1 == m_pMeta.m_layerList.end()) {
      if (layerItr->layer_type == TYPE_FULLYCONNECT) {
        final_output_buffer = "output_data";
      } else if (layerItr->layer_type == TYPE_RELU) {
        final_output_buffer = input_buffer(layerItr->buffer_order);
      } else {
        final_output_buffer = output_buffer(layerItr->buffer_order);
      }
    }
  }

  char cstr[final_output_buffer.size() + 1];
  strcpy(cstr, final_output_buffer.c_str());

  fprintf(filePtr, " return %s;\n", cstr);
  fprintf(filePtr, "}\n");
  fclose(filePtr);

  return Pass::kModuleNoChanged;
}
