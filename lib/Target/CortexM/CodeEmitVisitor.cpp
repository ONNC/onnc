//===- CodeEmitVisitor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CodeEmitVisitor.h"

#include <onnc/ADT/Color.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/LRN.h>
#include <onnc/IR/Compute/MatMul.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;
using namespace onnc::cortexm;

CodeEmitVisitor::CodeEmitVisitor(CortexmBackendMeta& Meta) noexcept
  : m_pMeta{Meta}
{}

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

  const Tensor* input_x = pConv.getInput(0);

  const Tensor* input_w        = pConv.getInput(1);
  int           input_w_dim[4] = {1, 1, 1, 1};
  for (int loop = 0; loop < input_w->getNumOfDimensions(); loop++) {
    input_w_dim[loop] = input_w->dimension(loop);
  }

  const Tensor* input_b   = NULL;
  int32_t       input_b_d = 0;
  if (pConv.getNumOfInputs() > 2) {
    input_b   = pConv.getInput(2);
    input_b_d = input_b->dimension(0);
  }
  // get output dimension
  const Tensor* output = pConv.getOutput(0);

  // save weight
  float* weight = packWeight_or_Bias(pConv, input_w, input_w_dim[0], group,
                                     (input_w_dim[0] * input_w_dim[1] * input_w_dim[2] * input_w_dim[3]));
  float* weight_HWC =
    (float*)malloc(sizeof(float) * (input_w_dim[0] * input_w_dim[1] * input_w_dim[2] * input_w_dim[3]));

  CHW_to_HWC(weight, input_w_dim, weight_HWC);
  float* bias = (float*)malloc(sizeof(float) * input_w_dim[0]);
  if (pConv.getNumOfInputs() > 2) {
    bias = packWeight_or_Bias(pConv, input_b, input_b_d, group, (input_b_d * ELEMENT_SIZE + 31) & ~(31));
  } else {
    for (int index = 0; index < input_w_dim[0]; index++) {
      bias[index] = 0;
    }
  }

	CortexmBackendMeta::Weight weightNode;
	weightNode.weight_value = weight_HWC;
	weightNode.weight_size  = input_w_dim[0] * input_w_dim[1] * input_w_dim[2] * input_w_dim[3];
	weightNode.bias_value   = bias;
	if (pConv.getNumOfInputs() > 2) {
		weightNode.bias_size = (input_w_dim[0] * ELEMENT_SIZE + 31) & ~(31);
		weightNode.have_bias = true;
	} else {
		weightNode.bias_size = input_w_dim[0];
		weightNode.have_bias = false;
	}
  m_pMeta.m_weightList.emplace_back(weightNode);

  float       auto_pad        = 0;
  std::string auto_pad_string = pConv.getAutoPad();
  if (strcmp(auto_pad_string.c_str(), "SAME_UPPER") == 0) {
    auto_pad = 0.5;

  } else if (strcmp(auto_pad_string.c_str(), "SAME_LOWER") == 0) {
    auto_pad = (-0.5);
  }

  if (pConv.getStrides().at(0) != pConv.getStrides().at(1)) {
    errs() << "In Conv, your x-stride and y-stride are different, we just use "
              "your x-stride.\n";
  }

	CortexmBackendMeta::Layer layerNode;
  layerNode.layer_type       = TYPE_CONV;
  layerNode.batch_size       = input_x->dimension(0);
  layerNode.input_dimension  = input_x->dimension(2);
  layerNode.input_channel    = input_x->dimension(1);
  layerNode.output_channel   = output->dimension(1);
	layerNode.kernel_size			= pConv.getKernelShape().at(0);
	layerNode.stride           = pConv.getStrides().at(0);
	layerNode.buffer_order     = buffer_order;
	layerNode.output_dimension = output->dimension(2);

	if (strcmp(auto_pad_string.c_str(), "VALID") == 0) {
		layerNode.pad  = 0;
		layerNode.pads = NULL;
	} else if (strcmp(auto_pad_string.c_str(), "NOTSET") == 0) {
		layerNode.pad     = pConv.getPads().at(0);
		layerNode.pads[0] = pConv.getPads().at(0);
		layerNode.pads[1] = pConv.getPads().at(1);
		layerNode.pads[2] = pConv.getPads().at(2);
		layerNode.pads[3] = pConv.getPads().at(3);
	} else {
		layerNode.pad  = get_padding(input_x->dimension(2), output->dimension(2), pConv.getKernelShape().at(0),
				pConv.getStrides().at(0), auto_pad);
		layerNode.pads = NULL;
	}

  m_pMeta.m_layerList.emplace_back(layerNode);

  pConv.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order + 1) & 1;
}

void CodeEmitVisitor::visit(MaxPool& pMaxPool)
{
  pMaxPool.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const MaxPool& pMaxPool)
{
  const Tensor* input_x = pMaxPool.getInput(0);

  const Tensor* input_w   = NULL;
  int32_t       input_w_d = 0;
  if (pMaxPool.getNumOfInputs() > 1) {
    input_w   = pMaxPool.getInput(1);
    input_w_d = input_w->getNumOfDimensions();
  }

  const Tensor* input_b   = NULL;
  int32_t       input_b_d = 0;
  if (pMaxPool.getNumOfInputs() > 2) {
    input_b   = pMaxPool.getInput(2);
    input_b_d = input_b->getNumOfDimensions();
  }

  const Tensor* output_y = pMaxPool.getOutput(0);

  float       auto_pad        = 0;
  std::string auto_pad_string = pMaxPool.getAutoPad();
  if (strcmp(auto_pad_string.c_str(), "\"SAME_UPPER\"") != 0) {
    auto_pad = 0.5;
  } else if (strcmp(auto_pad_string.c_str(), "\"SAME_LOWER\"") != 0) {
    auto_pad = (-0.5);
  }

	CortexmBackendMeta::Layer layerNode;
  layerNode.layer_type       = TYPE_MAXPOOLING;
  layerNode.input_dimension  = input_x->dimension(2);
  layerNode.input_channel    = input_x->dimension(1);
	layerNode.kernel_size			= pMaxPool.getKernelShape().at(0);
	layerNode.stride						= pMaxPool.getStrides().at(0);
	layerNode.buffer_order     = buffer_order;
	layerNode.output_dimension = output_y->dimension(2);
	if (m_pMeta.m_layerList.empty()) {
		layerNode.output_channel   = output_y->dimension(1);
	}

	bool isFirstLayer = m_pMeta.m_layerList.empty();
	if ((isFirstLayer && strcmp(auto_pad_string.c_str(), "VALID") == 0) || (!isFirstLayer && strcmp(auto_pad_string.c_str(), "\"VALID\"") != 0)) {
		layerNode.pad  = 0;
		layerNode.pads = NULL;
	} else if ((isFirstLayer && strcmp(auto_pad_string.c_str(), "NOTSET") == 0) || (!isFirstLayer && strcmp(auto_pad_string.c_str(), "\"NOTSET\"") != 0)) {
		layerNode.pad     = pMaxPool.getPads().at(0);
		layerNode.pads[0] = pMaxPool.getPads().at(0);
		layerNode.pads[1] = pMaxPool.getPads().at(1);
		layerNode.pads[2] = pMaxPool.getPads().at(2);
		layerNode.pads[3] = pMaxPool.getPads().at(3);
	}
  m_pMeta.m_layerList.emplace_back(layerNode);

  pMaxPool.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order + 1) & 1;
}

void CodeEmitVisitor::visit(Relu& pRelu)
{
  pRelu.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Relu& pRelu)
{
  const Tensor* input_x  = pRelu.getInput(0);
  const Tensor* output_y = pRelu.getOutput(0);

	CortexmBackendMeta::Layer layerNode;
  layerNode.layer_type       = TYPE_RELU;
  layerNode.batch_size       = input_x->dimension(0);
  layerNode.input_channel    = input_x->dimension(1);
  layerNode.input_dimension  = input_x->dimension(2);
  layerNode.output_channel   = output_y->dimension(1);
  layerNode.output_dimension = output_y->dimension(2);
  layerNode.buffer_order     = buffer_order;

	if (m_pMeta.m_layerList.size() != 0) {
		if (input_x->getNumOfDimensions() == 4) {
			layerNode.output_channel   = output_y->dimension(1);
			layerNode.output_dimension = output_y->dimension(2);
		} else if (input_x->getNumOfDimensions() == 2) {
			layerNode.output_channel   = output_y->dimension(1);
			layerNode.output_dimension = 1;
		}
	}
  m_pMeta.m_layerList.emplace_back(layerNode);

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
  const Tensor* input_x   = pSoftmax.getInput(0);
  int32_t       input_x_d = input_x->getNumOfDimensions();
  for (int loop = 0; loop < input_x_d; loop++) {
  }
  int32_t axis = pSoftmax.getAxis().value();

	CortexmBackendMeta::Layer layerNode;
  layerNode.layer_type       = TYPE_SOFTMAX;
  m_pMeta.m_layerList.emplace_back(layerNode);

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
  const Tensor* input_x   = pGemm.getInput(0);
  int32_t       input_x_d = input_x->getNumOfDimensions();
  for (int loop = 0; loop < input_x_d; loop++) {
  }

  const Tensor* input_w   = pGemm.getInput(1);
  int32_t       input_w_d = input_w->getNumOfDimensions();
  for (int loop = 0; loop < input_w_d; loop++) {
  }

  const Tensor* input_b   = NULL;
  int32_t       input_b_d = 0;
  if (pGemm.getNumOfInputs() > 2) {
    input_b   = pGemm.getInput(2);
    input_b_d = input_b->getNumOfDimensions();
    for (int loop = 0; loop < input_b_d; loop++) {
    }
  }

  float alpha = pGemm.getAlpha().value();
  float beta = pGemm.getBeta().value();
  int32_t transA = pGemm.getTransA().value();
  int32_t transB = pGemm.getTransB().value();

	CortexmBackendMeta::Layer layerNode;
  layerNode.layer_type       = TYPE_FULLYCONNECT;
	layerNode.buffer_order			= buffer_order;
	auto lastCode = m_pMeta.m_layerList.back();

	if (lastCode.layer_type == TYPE_MAXPOOLING) {
		layerNode.input_channel	= lastCode.input_channel;
		layerNode.output_dimension = lastCode.output_dimension;
	} else if (lastCode.layer_type == TYPE_RELU) {
		layerNode.input_channel	= lastCode.output_dimension;
		layerNode.output_dimension = 1;
	}
  m_pMeta.m_layerList.emplace_back(layerNode);

  pGemm.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order + 1) & 1;
}

void CodeEmitVisitor::visit(Reshape& pReshape)
{
  pReshape.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Reshape& pReshape)
{
  const Tensor* input_x = pReshape.getInput(0);
  const Tensor* data    = pReshape.getShape();

  errs() << "data:" << data->dimension(0) << "\n";

  int32_t input_x_d      = input_x->getNumOfDimensions();
  int     input_x_dim[4] = {1, 1, 1, 1};
  for (int loop = 0; loop < input_x_d; loop++) {
    input_x_dim[loop] = input_x->dimension(loop);
    errs() << "data2:" << input_x->dimension(loop) << "\n";
  }

  const Tensor* output_y     = pReshape.getOutput(0);
  int32_t       output_y_d   = output_y->getNumOfDimensions();
  int*          output_y_dim = (int*)malloc(sizeof(int) * output_y_d);
  for (int loop = 0; loop < output_y_d; loop++) {
    output_y_dim[loop] = output_y->dimension(loop);
  }

  if (output_y_dim[0] != 1) {
    float* weight = packWeight_or_Bias(
      pReshape, input_x, input_x_dim[0], 1,
      (input_x_dim[0] * input_x_dim[1] * input_x_dim[2] * input_x_dim[3] * ELEMENT_SIZE + 31) & ~(31));
    float* weight_HWC =
      (float*)malloc(sizeof(float) * (input_x_dim[0] * input_x_dim[1] * input_x_dim[2] * input_x_dim[3]));
    CHW_to_HWC_mat(weight, input_x_dim, weight_HWC);

		CortexmBackendMeta::Matmul matmulNode;
		matmulNode.matmul_value = weight_HWC;
		matmulNode.matmul_size  = (input_x_dim[0] * input_x_dim[1] * input_x_dim[2] * input_x_dim[3]);
		for (int i = 0; i < 4; i++) {
			errs() << "input:" << input_x_dim[i] << "\n";
		}
		errs() << "matmul_size:" << matmulNode.matmul_size << "\n";
		matmulNode.output_channel  = input_x_dim[0];
		matmulNode.input_channel   = input_x_dim[0];
		matmulNode.input_dimension = input_x_dim[1];
		matmulNode.batch_size      = input_x_dim[3];
		m_pMeta.m_matmulList.emplace_back(matmulNode);
  }

	CortexmBackendMeta::Shape shapeNode;
	if (m_pMeta.m_shapeList.empty()) {
		shapeNode.shape_value = output_y_dim;
	} else {
		auto lastShapeNode = m_pMeta.m_shapeList.back();
    if (lastShapeNode.shape_value[1] == output_y_dim[0]) {
      shapeNode.shape_value = output_y_dim;
    } else {
      shapeNode.shape_value  = lastShapeNode.shape_value;
      lastShapeNode.shape_value = output_y_dim;
    }
	}
	m_pMeta.m_shapeList.emplace_back(shapeNode);

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
  const Tensor* input_x   = pLRN.getInput(0);
  int32_t       input_x_d = input_x->getNumOfDimensions();
  for (int loop = 0; loop < input_x_d; loop++) {
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

void CodeEmitVisitor::visit(Add& pAdd)
{
  pAdd.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const Add& pAdd)
{
  const Tensor* input_x         = pAdd.getInput(0);
  int32_t       input_x_d       = input_x->getNumOfDimensions();
  int           input_x_dim[4]  = {1, 1, 1, 1};
  int*          input_x_dim_ptr = (int*)malloc(sizeof(input_x_dim));
  for (int loop = 0; loop < input_x_d; loop++) {
    input_x_dim[loop]     = input_x->dimension(loop);
    input_x_dim_ptr[loop] = input_x_dim[loop];
  }
  if (input_x_d == 4) {
    input_x_dim_ptr[1] ^= input_x_dim_ptr[3];
    input_x_dim_ptr[3] ^= input_x_dim_ptr[1];
    input_x_dim_ptr[1] ^= input_x_dim_ptr[3];
  }

  const Tensor* input_b         = pAdd.getInput(1);
  int32_t       input_b_d       = input_b->getNumOfDimensions();
  int           input_b_dim[4]  = {1, 1, 1, 1};
  int*          input_b_dim_ptr = (int*)malloc(sizeof(input_b_dim));
  for (int loop = 0; loop < input_b_d; loop++) {
    input_b_dim[loop]     = input_b->dimension(loop);
    input_b_dim_ptr[loop] = input_b_dim[loop];
  }

  const Tensor* output_z        = pAdd.getC();
  int32_t       output_dim_size = output_z->getNumOfDimensions();

  float* add = packWeight_or_Bias(pAdd, input_b, input_b_dim[0], 1, (input_b_dim[0] * ELEMENT_SIZE + 31) & ~(31));
  int    weight_size = 1;
  for (int i = 0; i < input_b_d; i++) {
    weight_size *= input_b_dim[i];
  }

	CortexmBackendMeta::Add addNode;
	addNode.add_value       = add;
	addNode.add_size        = (input_b_dim[0] * input_b_dim[1]);
	addNode.input_dims      = input_x_dim_ptr;
	addNode.input_dims_size = input_x_d;
	addNode.add_dims        = input_b_dim_ptr;
	addNode.add_dims_size   = input_b_d;
	m_pMeta.m_addList.emplace_back(addNode);

	bool isFirstLayer = m_pMeta.m_layerList.empty();
	CortexmBackendMeta::Layer layerNode;
  layerNode.layer_type       = TYPE_ADD;
	layerNode.buffer_order     = buffer_order;
	layerNode.batch_size				= input_x->dimension(0);
	layerNode.weight_size			= weight_size;
	layerNode.input_size			  = input_x_d;
  layerNode.input_dimension  = (input_x_d == 2 && isFirstLayer) ? input_x->dimension(1) : input_x->dimension(2);
  layerNode.input_channel    = (input_x_d == 2 && isFirstLayer) ? 1 : input_x->dimension(1); // input_x_d;//input_dims_size
	layerNode.weight_dim_size  = input_b_d;
	layerNode.output_dimension = output_dim_size;
	layerNode.output_channel   = (input_x_d == 2 && isFirstLayer) ? 1 : input_x->dimension(1); // add_dims_size
  m_pMeta.m_layerList.emplace_back(layerNode);

  pAdd.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order + 1) & 1;
}

void CodeEmitVisitor::visit(MatMul& pMatMul)
{
  pMatMul.print(errs());
  errs() << "\n\n";
}

void CodeEmitVisitor::visit(const MatMul& pMatMul)
{
  const Tensor* input_x = pMatMul.getA();
  const Tensor* input_y = pMatMul.getB();

  int x_dim_size = input_x->getNumOfDimensions();
  int input_x_dim[x_dim_size];

  int y_dim_size = input_y->getNumOfDimensions();
  int input_y_dim[y_dim_size];

  if (x_dim_size == y_dim_size) {
    for (int dim_size = 0; dim_size < x_dim_size; dim_size++) {
      input_x_dim[dim_size] = input_x->dimension(dim_size); // x [2] ={1,256} y [2] ={256,10}
      input_y_dim[dim_size] = input_y->dimension(dim_size);
    }
  } else {
    // if x_dim_size != y_dim_size
  }

	CortexmBackendMeta::Layer layerNode;
  layerNode.layer_type       = TYPE_MATMUL;
  layerNode.buffer_order     = buffer_order;
  layerNode.batch_size       = m_pMeta.m_matmulList.back().batch_size;
  layerNode.output_channel   = m_pMeta.m_matmulList.back().output_channel;
  layerNode.input_channel    = m_pMeta.m_matmulList.back().input_channel;
  layerNode.input_dimension  = m_pMeta.m_matmulList.back().input_dimension;
	layerNode.matmul_size			= m_pMeta.m_matmulList.back().matmul_size;
  m_pMeta.m_layerList.emplace_back(layerNode);

  pMatMul.print(errs());
  errs() << "\n\n";
  buffer_order = (buffer_order + 1) & 1;
}

float* CodeEmitVisitor::packWeight_or_Bias(const ComputeOperator& co, const Tensor* t, int dims_0, int gidx,
                                           unsigned int size)
{
  float* blob_data = new float[size];
  memset(blob_data, 0, size);

  short* dest = (short*)blob_data;
  float* data = (float*)(static_cast<const FloatTensor*>(t)->getValues().data());

  int group_c = gidx * dims_0;
  for (int c = 0; c < dims_0; c++) {
    *(dest + c) = __gnu_f2h_ieee_vanilla(*(data + group_c + c));
  }

  return data;
}
