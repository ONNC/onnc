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
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
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

#include <onnc/ADT/Color.h>
#include <onnc/Diagnostic/MsgHandling.h>

//#include <fstream>

//#include "add.h"

using namespace onnc;
using namespace onnc::cortexm;

using namespace cortexm_loadable;
using namespace cortexm_loadable::loadable;

extern struct code_list* first_code ;
extern struct weight_list* first_weight ;
extern struct add_list* first_add ;
extern struct matmul_list* first_matmul;
extern struct shape_list* first_shape;

CodeEmitVisitor::CodeEmitVisitor(CortexmBackendMeta &Meta) noexcept 
: m_pMeta{Meta}
{ }

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
  errs() << "\n\n";
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
  if( strcmp(auto_pad_string.c_str(),"SAME_UPPER") == 0 ){
    auto_pad = 0.5;
    
  }else if( strcmp(auto_pad_string.c_str(),"SAME_LOWER") == 0 ){
    auto_pad = (-0.5);
  
  }

  if(pConv.getStrides().at(0) != pConv.getStrides().at(1)){
    errs() << "In Conv, your x-stride and y-stride are different, we just use your x-stride.\n";
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
    if(strcmp(auto_pad_string.c_str(),"VALID") == 0){
      save_code -> pad = 0;
      save_code -> pads = NULL;
    }else if(strcmp(auto_pad_string.c_str(),"NOTSET") == 0){
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
    if(strcmp(auto_pad_string.c_str(),"VALID") == 0){
      new_code -> pad = 0;
      new_code -> pads = NULL;
    }else if(strcmp(auto_pad_string.c_str(),"NOTSET") == 0){
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
  const Tensor *input_x = pMaxPool.getInput(0);

  const Tensor *input_w = NULL;
  int32_t input_w_d = 0;
  if (pMaxPool.getNumOfInputs() > 1) {
    input_w = pMaxPool.getInput(1);
    input_w_d = input_w->getNumOfDimensions();
  }

  const Tensor *input_b = NULL;
  int32_t input_b_d = 0;
  if (pMaxPool.getNumOfInputs() > 2) {
    input_b = pMaxPool.getInput(2);
    input_b_d = input_b->getNumOfDimensions();
  }

  const Tensor *output_y = pMaxPool.getOutput(0);
  
  float auto_pad = 0;
  std::string auto_pad_string = pMaxPool.getAutoPad();
  if( strcmp(auto_pad_string.c_str(),"\"SAME_UPPER\"") != 0 ){
    auto_pad = 0.5;
  }else if( strcmp(auto_pad_string.c_str(),"\"SAME_LOWER\"") != 0 ){
    auto_pad = (-0.5);
  }

  //save
  layer_id++;
  struct code_list* new_code = (code_list*)malloc(sizeof(code_list));
  new_code -> layer_type = TYPE_MAXPOOLING;
  new_code -> buffer_order = buffer_order;

  new_code -> input_dimention = input_x->dimension(2);
  new_code -> input_channel = input_x->dimension(1);
  new_code -> kernel_size = pMaxPool.getKernelShape().at(0);

  if(pMaxPool.getStrides().at(0) != pMaxPool.getStrides().at(1)){
    errs() << "In Maxpooling, your x-stride and y-stride are different, we just use your x-stride.\n";
  }

  if(strcmp(auto_pad_string.c_str(),"\"VALID\"") != 0){
    new_code -> pad = 0;
    new_code -> pads = NULL;
  }else if(strcmp(auto_pad_string.c_str(),"\"NOTSET\"") != 0){
    new_code -> pad = pMaxPool.getPads().at(0);
    new_code -> pads[0] = pMaxPool.getPads().at(0);
    new_code -> pads[1] = pMaxPool.getPads().at(1);
    new_code -> pads[2] = pMaxPool.getPads().at(2);
    new_code -> pads[3] = pMaxPool.getPads().at(3);
  }else{
    new_code -> pad = get_padding(input_x->dimension(2),output_y->dimension(2),pMaxPool.getKernelShape().at(0),pMaxPool.getStrides().at(0),auto_pad);
    new_code -> pads = NULL;
  }
  new_code -> stride = pMaxPool.getStrides().at(0);
  new_code -> output_dimention = output_y->dimension(2);
  new_code -> layer_id = layer_id;
  new_code -> next = NULL;
  save_code -> next = new_code;
  save_code = new_code;
  
  pMaxPool.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order+1)&1;
}

void CodeEmitVisitor::visit(Relu& pRelu)
{
  pRelu.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Relu& pRelu)
{
  const Tensor *input_x = pRelu.getInput(0);

  const Tensor *output = pRelu.getOutput(0);
  
  //save
  layer_id++;
  struct code_list* new_code = (code_list*)malloc(sizeof(code_list));
  new_code -> layer_type = TYPE_RELU;
  if(input_x -> getNumOfDimensions() == 4){
    new_code -> output_channel = output->dimension(1);
    new_code -> output_dimention = output->dimension(2);
  }else if(input_x -> getNumOfDimensions() == 2){
    new_code -> output_channel = output->dimension(1);
    new_code -> output_dimention = 1;
  }
  new_code -> buffer_order = buffer_order;
  new_code -> layer_id = layer_id;
  new_code -> next = NULL;
  save_code -> next = new_code;
  save_code = new_code;

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
  const Tensor *input_x = pSoftmax.getInput(0);
  int32_t input_x_d = input_x->getNumOfDimensions();
  for(int loop = 0 ; loop<input_x_d; loop++){
  }
  int32_t axis = pSoftmax.getAxis().value();
  //save
  layer_id++;
  struct code_list* new_code = (code_list*)malloc(sizeof(code_list));
  new_code -> layer_type = TYPE_SOFTMAX;
  new_code -> layer_id = layer_id;
  new_code -> next = NULL;
  save_code -> next = new_code;
  save_code = new_code;
  
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
  const Tensor *input_x = pGemm.getInput(0);
  int32_t input_x_d = input_x->getNumOfDimensions();
  for(int loop = 0 ; loop<input_x_d; loop++){
  }

  const Tensor *input_w = pGemm.getInput(1);
  int32_t input_w_d = input_w->getNumOfDimensions();
  for(int loop = 0 ; loop<input_w_d; loop++){
  }

  const Tensor *input_b = NULL;
  int32_t input_b_d = 0;
  if (pGemm.getNumOfInputs() > 2) {
    input_b = pGemm.getInput(2);
    input_b_d = input_b->getNumOfDimensions();
    for(int loop = 0 ; loop<input_b_d; loop++){
    }
  }

  float alpha = pGemm.getAlpha().value();
  
  float beta = pGemm.getBeta().value();
  
  int32_t transA = pGemm.getTransA().value();
  
  int32_t transB = pGemm.getTransB().value();

  //save
  layer_id++;
  struct code_list* new_code = (code_list*)malloc(sizeof(code_list));
  
  new_code -> layer_type = TYPE_FULLYCONNECT;
  new_code -> buffer_order = buffer_order;
  if(save_code -> layer_type == TYPE_MAXPOOLING){
    new_code -> input_channel = save_code -> input_channel;
    new_code -> output_dimention = save_code -> output_dimention;
  }else if(save_code -> layer_type == TYPE_RELU){
    new_code -> input_channel = save_code -> output_channel;
    new_code -> output_dimention = 1;
  }
  new_code -> layer_id = layer_id;
  new_code -> next = NULL;
  save_code -> next = new_code;
  save_code = new_code;

  pGemm.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order+1)&1;
}

void CodeEmitVisitor::visit(Reshape& pReshape)
{
  pReshape.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Reshape& pReshape)
{
  const Tensor *input_x = pReshape.getInput(0);
  int32_t input_x_d = input_x->getNumOfDimensions();
  int input_x_dim[4] = {1,1,1,1};
  for(int loop = 0 ; loop < input_x_d ; loop++){
    input_x_dim[loop] = input_x -> dimension(loop);
  } 

  const Tensor *output_y = pReshape.getOutput(0);
  int32_t output_y_d = output_y->getNumOfDimensions();
  int *output_y_dim = (int*)malloc(sizeof(int)*output_y_d);
  for(int loop = 0 ; loop < output_y_d ; loop++){
    output_y_dim[loop] = output_y -> dimension(loop);
  } 

  if(output_y_dim[0] != 1){
    float *weight = packWeight_or_Bias(pReshape , input_x , input_x_dim[0] , 1 , (input_x_dim[0] * input_x_dim[1] * input_x_dim[2] * input_x_dim[3] * ELEMENT_SIZE + 31) & ~(31));
    float *weight_HWC = (float*)malloc(sizeof(float) * (input_x_dim[0] * input_x_dim[1] * input_x_dim[2] * input_x_dim[3]));
    CHW_to_HWC_mat(weight,input_x_dim,weight_HWC);
    if(matmul_first == 0){
      first_matmul = save_matmul;
      save_matmul -> matmul_value = weight_HWC;
      save_matmul -> matmul_size = (input_x_dim[0] * input_x_dim[1] * input_x_dim[2] * input_x_dim[3]);
      save_matmul -> next = NULL;
      matmul_first++;
    }else{
      struct matmul_list* new_matmul = (matmul_list*)malloc(sizeof(matmul_list));
      new_matmul -> matmul_value = weight_HWC;
      new_matmul -> matmul_size = (input_x_dim[0] * input_x_dim[1] * input_x_dim[2] * input_x_dim[3]);
      new_matmul -> next = NULL;
      save_matmul -> next = new_matmul;
      save_matmul = new_matmul;
    }
  }

  if(shape_first == 0){
    first_shape = save_shape;
    save_shape -> shape_value = output_y_dim;
    save_shape -> next = NULL;

    shape_first++;
  }else{
    struct shape_list* new_shape = (shape_list*)malloc(sizeof(shape_list));
    

    if(save_shape -> shape_value[1] == output_y_dim[0] ){
      new_shape -> shape_value = output_y_dim;
    }else{
      new_shape -> shape_value = save_shape -> shape_value;
      save_shape -> shape_value = output_y_dim;
    }

    new_shape -> next = NULL;
    save_shape -> next = new_shape;
    save_shape = new_shape;
  }

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

  const Tensor *input_x = pLRN.getInput(0);
  int32_t input_x_d = input_x->getNumOfDimensions();
  for(int loop = 0 ; loop<input_x_d; loop++){
  }

  float alpha = pLRN.getAlpha().value();
  
  float beta = pLRN.getBeta().value();
  
  float bias = pLRN.getBias().value();
  
  int32_t size = pLRN.getSize().value();

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
  const Tensor *input_x = pAdd.getInput(0);
  int32_t input_x_d = input_x->getNumOfDimensions();
  int input_x_dim[4] = {1,1,1,1};
  int *input_x_dim_ptr = (int*)malloc(sizeof(input_x_dim));
  for(int loop = 0;loop < input_x_d;loop++){
    input_x_dim[loop] = input_x -> dimension(loop);
    input_x_dim_ptr[loop] = input_x_dim[loop];
  }
  if(input_x_d == 4){
    input_x_dim_ptr[1] ^= input_x_dim_ptr[3]; 
    input_x_dim_ptr[3] ^= input_x_dim_ptr[1]; 
    input_x_dim_ptr[1] ^= input_x_dim_ptr[3]; 
  }
  
  const Tensor *input_b = pAdd.getInput(1);
  int32_t input_b_d = input_b->getNumOfDimensions();
  int input_b_dim[4] = {1,1,1,1};
  int *input_b_dim_ptr = (int*)malloc(sizeof(input_b_dim));
  for(int loop = 0;loop < input_b_d;loop++){
    input_b_dim[loop] = input_b -> dimension(loop);
    input_b_dim_ptr[loop] = input_b_dim[loop];
  }

  float *add = packWeight_or_Bias(pAdd , input_b , input_b_dim[0] , 1 , (input_b_dim[0] * ELEMENT_SIZE + 31) & ~(31));
  
  if(add_first == 0){
    first_add = save_add;
    save_add -> add_value = add;
    save_add -> add_size = (input_b_dim[0] * input_b_dim[1]);
    save_add -> input_dims = input_x_dim_ptr;
    save_add -> input_dims_size = input_x_d;
    save_add -> add_dims = input_b_dim_ptr;
    save_add -> add_dims_size = input_b_d;
    save_add -> next = NULL;
    add_first++;
  }else{
    struct add_list* new_add = (add_list*)malloc(sizeof(add_list));
    new_add -> add_value = add;
    new_add -> add_size = (input_b_dim[0] * input_b_dim[1]);
    new_add -> input_dims = input_x_dim_ptr;
    new_add -> input_dims_size = input_x_d;
    new_add -> add_dims = input_b_dim_ptr;
    new_add -> add_dims_size = input_b_d;
    new_add -> next = NULL;
    save_add -> next = new_add;
    save_add = new_add;
  } 
  layer_id++;
  if(first == 0){
    first_code = save_code;
    save_code -> layer_type = TYPE_ADD;
    save_code -> buffer_order = buffer_order;
    save_code -> input_dimention = input_x_dim[1];
    save_code -> input_channel = input_x_d;//input_dims_size
    save_code -> output_channel = input_b_d;//add_dims_size
    save_code -> layer_id = layer_id;
    save_code -> next = NULL;
    first++;
  }else{
    struct code_list* new_code = (code_list*)malloc(sizeof(code_list));
    new_code -> layer_type = TYPE_ADD;
    new_code -> buffer_order = buffer_order;
    new_code -> input_dimention = input_x_dim[1];
    new_code -> input_channel = input_x_d;//input_dims_size
    new_code -> output_channel = input_b_d;//add_dims_size
    new_code -> layer_id = layer_id;
    new_code -> next = NULL;
    save_code -> next = new_code;
    save_code = new_code;
  }

  pAdd.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order+1)&1;
}

void CodeEmitVisitor::visit(MatMul& pMatMul)
{
  pMatMul.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const MatMul& pMatMul)
{
  layer_id++;
  if(first == 0){
    first_code = save_code;
    save_code -> layer_type = TYPE_MATMUL;
    save_code -> buffer_order = buffer_order;
    save_code -> input_dimention = save_matmul -> matmul_size;
    save_code -> layer_id = layer_id;
    save_code -> next = NULL;
    first++;
  }else{
    struct code_list* new_code = (code_list*)malloc(sizeof(code_list));
    new_code -> layer_type = TYPE_MATMUL;
    new_code -> buffer_order = buffer_order;
    new_code -> input_dimention = save_matmul -> matmul_size;
    new_code -> layer_id = layer_id;
    new_code -> next = NULL;
    save_code -> next = new_code;
    save_code = new_code;
  }

  pMatMul.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order+1)&1;
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



