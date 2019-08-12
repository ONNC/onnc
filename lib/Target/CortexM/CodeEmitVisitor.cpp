//===- CodeEmitVisitor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/IOStream.h>
#include "CodeEmitVisitor.h"
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/LRN.h>
#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/Compute/MatMul.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>

using namespace onnc;
using namespace onnc::cortexm;

using namespace CortexM_loadable;
using namespace CortexM_loadable::loadable;

extern struct code_list* first_code ;
extern struct weight_list* first_weight ;
extern struct add_list* first_add ;
extern struct matmul_list* first_matmul;
extern struct shape_list* first_shape;
extern bool gen_error;

void CodeEmitVisitor::visit(Initializer& pInitializer)
{
  pInitializer.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(const Initializer& pInitializer)
{
  pInitializer.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(InputOperator& pInputOperator)
{
  pInputOperator.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(const InputOperator& pInputOperator)
{
  pInputOperator.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(OutputOperator& pOutputOperator)
{
  pOutputOperator.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(const OutputOperator& pOutputOperator)
{
  pOutputOperator.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(Conv& pConv)
{
  pConv.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(const Conv& pConv)
{
  int group = pConv.getGroup().value();

  const Tensor *input_x = pConv.getInput(0);

  const Tensor *input_w = pConv.getInput(1);
  int input_w_dim[4] = {1,1,1,1};
  for(int loop = 0 ; loop < input_w->getNumOfDimensions() ; loop++){
    input_w_dim[loop] = input_w -> dimension(loop);
  }  

  const Tensor *input_b = NULL;
  int32_t input_b_d = 0;
  if (pConv.getNumOfInputs() > 2) {
    input_b = pConv.getInput(2);
    input_b_d = input_b->dimension(0);
  }
  // get output dimension
  const Tensor *output = pConv.getOutput(0);
  
  // save weight
  float *weight = packWeight_or_Bias(pConv , input_w , input_w_dim[0] , group , (input_w_dim[0] * input_w_dim[1] * input_w_dim[2] * input_w_dim[3]) );
  float *weight_HWC = (float*)malloc(sizeof(float) * (input_w_dim[0] * input_w_dim[1] * input_w_dim[2] * input_w_dim[3]));

  CHW_to_HWC(weight,input_w_dim,weight_HWC);
  float *bias = (float*)malloc(sizeof(float) * input_w_dim[0]);
  if(pConv.getNumOfInputs() > 2){
  	bias = packWeight_or_Bias(pConv , input_b , input_b_d , group , (input_b_d * ELEMENT_SIZE + 31) & ~(31));
	}else{
    for(int index = 0 ; index < input_w_dim[0] ; index++){
      bias[index] = 0;
    }
  }
  
  if(first == 0){
    first_weight = save_weight;
    save_weight -> weight_value = weight_HWC;
    save_weight -> weight_size = input_w_dim[0] * input_w_dim[1] * input_w_dim[2] * input_w_dim[3];
		save_weight -> bias_value = bias;
    if(pConv.getNumOfInputs() > 2){
		  save_weight -> bias_size = (input_w_dim[0] * ELEMENT_SIZE + 31)& ~(31);
      save_weight -> have_bias = true;
    }else{
      save_weight -> bias_size = input_w_dim[0];
      save_weight -> have_bias = false;
    }
    save_weight -> next = NULL;
  }else{
    struct weight_list* new_weight = (weight_list*)malloc(sizeof(weight_list));
    new_weight -> weight_value = weight_HWC;
    new_weight -> weight_size = input_w_dim[0] * input_w_dim[1] * input_w_dim[2] * input_w_dim[3];
		new_weight -> bias_value = bias;
		if(pConv.getNumOfInputs() > 2){
		  new_weight -> bias_size = (input_w_dim[0] * ELEMENT_SIZE + 31)& ~(31);
      new_weight -> have_bias = true;
    }else{
      new_weight -> bias_size = input_w_dim[0];
      new_weight -> have_bias = false;
    }
    new_weight -> next = NULL;
    save_weight -> next = new_weight;
    save_weight = new_weight;
  }
  
  float auto_pad = 0;
  std::string auto_pad_string = pConv.getAutoPad();
  if( strcmp(auto_pad_string.c_str(),"\"SAME_UPPER\"") != 0 ){
    auto_pad = 0.5;
  }else if( strcmp(auto_pad_string.c_str(),"\"SAME_LOWER\"") != 0 ){
    auto_pad = (-0.5);
  }

  if(pConv.getStrides().at(0) != pConv.getStrides().at(1)){
    *error = true;
    gen_error = error;
    //errs()<< "error = "<< (*error) <<"\n";
    //errs() << "In Conv, your x-stride and y-stride are different, we just use your x-stride.\n";
  }

  //errs() << auto_pad << "\n";
  layer_id++;
  if(first == 0){
    first_code = save_code;
    save_code -> layer_type = TYPE_CONV;
    save_code -> input_dimention = input_x->dimension(2);
    save_code -> input_channel = input_x->dimension(1);
    save_code -> output_channel = output->dimension(1);
    save_code -> kernel_size = pConv.getKernelShape().at(0);
    if(strcmp(auto_pad_string.c_str(),"\"VALID\"") != 0){
      save_code -> pad = 0;
      save_code -> pads = NULL;
    }else if(strcmp(auto_pad_string.c_str(),"\"NOTSET\"") != 0){
      save_code -> pad = pConv.getPads().at(0);
      save_code -> pads[0] = pConv.getPads().at(0);
      save_code -> pads[1] = pConv.getPads().at(1);
      save_code -> pads[2] = pConv.getPads().at(2);
      save_code -> pads[3] = pConv.getPads().at(3);
    }else{
      save_code -> pad = get_padding(input_x->dimension(2),output->dimension(2),pConv.getKernelShape().at(0),pConv.getStrides().at(0),auto_pad);
      save_code -> pads = NULL;
    }
    save_code -> stride = pConv.getStrides().at(0);
    save_code -> buffer_order = buffer_order;
    save_code -> output_dimention = output->dimension(2);
    save_code -> layer_id = layer_id;
    save_code -> next = NULL;
    first++;
  }else{
    struct code_list* new_code = (code_list*)malloc(sizeof(code_list));
    new_code -> layer_type = TYPE_CONV;
    new_code -> input_dimention = input_x->dimension(2);
    new_code -> input_channel = input_x->dimension(1);
    new_code -> output_channel = output->dimension(1);
    new_code -> kernel_size = pConv.getKernelShape().at(0);
    if(strcmp(auto_pad_string.c_str(),"\"VALID\"") != 0){
      new_code -> pad = 0;
      new_code -> pads = NULL;
    }else if(strcmp(auto_pad_string.c_str(),"\"NOTSET\"") != 0){
      new_code -> pad = pConv.getPads().at(0);
      new_code -> pads[0] = pConv.getPads().at(0);
      new_code -> pads[1] = pConv.getPads().at(1);
      new_code -> pads[2] = pConv.getPads().at(2);
      new_code -> pads[3] = pConv.getPads().at(3);
    }else{
      new_code -> pad = get_padding(input_x->dimension(2),output->dimension(2),pConv.getKernelShape().at(0),pConv.getStrides().at(0),auto_pad);
      new_code -> pads = NULL;
    }
    new_code -> stride = pConv.getStrides().at(0);
    new_code -> buffer_order = buffer_order;
    new_code -> output_dimention = output->dimension(2);
    new_code -> layer_id = layer_id;
    new_code -> next = NULL;
    save_code -> next = new_code;
    save_code = new_code;
  } 
  
  pConv.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order+1)&1;
}
void CodeEmitVisitor::visit(MaxPool& pMaxPool)
{
  pMaxPool.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const MaxPool& pMaxPool)
{
  pMaxPool.print(errs());
  errs() << "\n\n";
}
void CodeEmitVisitor::visit(Relu& pRelu)
{
  pRelu.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Relu& pRelu)
{
  pRelu.print(errs());
  errs() << "\n\n";
}
void CodeEmitVisitor::visit(AveragePool& pAveragePool)
{
  pAveragePool.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const AveragePool& pAveragePool)
{
  pAveragePool.print(errs());
  errs() << "\n\n";
}
void CodeEmitVisitor::visit(Softmax& pSoftmax)
{
  pSoftmax.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Softmax& pSoftmax)
{
  pSoftmax.print(errs());
  errs() << "\n\n";
}
void CodeEmitVisitor::visit(Gemm& pGemm)
{
  pGemm.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Gemm& pGemm)
{
  pGemm.print(errs());
  errs() << "\n\n";
}
void CodeEmitVisitor::visit(Reshape& pReshape)
{
  pReshape.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Reshape& pReshape)
{
  pReshape.print(errs());
  errs() << "\n\n";
}
void CodeEmitVisitor::visit(LRN& pLRN)
{
  pLRN.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const LRN& pLRN)
{
  pLRN.print(errs());
  errs() << "\n\n";
}
void CodeEmitVisitor::visit(Concat& pConcat)
{
  pConcat.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Concat& pConcat)
{
  pConcat.print(errs());
  errs() << "\n\n";
}
//
void CodeEmitVisitor::visit(Add& pAdd)
{
  pAdd.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Add& pAdd)
{
  pAdd.print(errs());
  errs() << "\n\n";
}
void CodeEmitVisitor::visit(MatMul& pMatMul)
{
  pMatMul.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const MatMul& pMatMul)
{  
  pMatMul.print(errs());
  errs() << "\n\n";
}

float* CodeEmitVisitor::packWeight_or_Bias(const ComputeOperator& co,const Tensor* t, int dims_0, int gidx,unsigned int size){
	std::string blob_name = "tb-" + std::to_string(m_pMeta.m_NumMlobs++);
	
	float* blob_data = new float[size];
  memset(blob_data , 0 , size );

	short* dest = (short*)blob_data;
	float* data = (float*)(static_cast<const FloatTensor*>(t)->getValues().data() );

  int group_c = gidx * dims_0;
	for(int c = 0 ; c < dims_0 ;c++){
		*(dest + c) = __gnu_f2h_ieee_vanilla(*(data + group_c + c));
	}

  return data;

}