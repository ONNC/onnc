#include "CortexmWeightFileGenPass.h"

#include "CodeEmitVisitor.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

#include <stdio.h>

using namespace onnc;

namespace onnc {
namespace internal {

void writeRightShiftValue(FILE* weightFilePtr, const CortexmBackendMeta& pMeta)
{
  unsigned int num_of_shift = 0;
  for (const auto& shiftNode : pMeta.m_shiftList) {
    fprintf(weightFilePtr, "#define RIGHT_SHIFT%d %d\n", ++num_of_shift, shiftNode.shift_value);
  }
}

void writeAddValue(FILE* weightFilePtr, const CortexmBackendMeta& pMeta)
{
  unsigned int num_of_add = 0;
  for (const auto& addNode : pMeta.m_addList) {
    num_of_add++;
    fprintf(weightFilePtr, "#define ADD%d {", num_of_add);
    for (int index = 0; index < addNode.add_value.size(); index++) {
      if (index != (addNode.add_value.size() - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(addNode.add_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(addNode.add_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");

    fprintf(weightFilePtr, "#define INPUT_DIMS%d {", num_of_add);
    for (int index = 0; index < addNode.input_dims.size(); index++) {
      if (index != (addNode.input_dims.size() - 1)) {
        fprintf(weightFilePtr, "%d,", addNode.input_dims[index]);
      } else {
        fprintf(weightFilePtr, "%d", addNode.input_dims[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");

    fprintf(weightFilePtr, "#define ADD_DIMS%d {", num_of_add);
    for (int index = 0; index < addNode.add_dims.size(); index++) {
      if (index != (addNode.add_dims.size() - 1)) {
        fprintf(weightFilePtr, "%d,", addNode.add_dims[index]);
      } else {
        fprintf(weightFilePtr, "%d", addNode.add_dims[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

void writeMatMulValue(FILE* weightFilePtr, const CortexmBackendMeta& pMeta)
{
  unsigned int num_of_matmul = 0;
  for (const auto& matmulNode : pMeta.m_matmulList) {
    num_of_matmul++;
    fprintf(weightFilePtr, "#define MATMUL_WEIGHT%d {", num_of_matmul);
    for (int index = 0; index < matmulNode.matmul_value.size(); index++) {
      if (index != (matmulNode.matmul_value.size() - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(matmulNode.matmul_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(matmulNode.matmul_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

void writeShapeValue(FILE* weightFilePtr, const CortexmBackendMeta& pMeta)
{
  unsigned int num_of_shape = 0;
  for (const auto& shapeNode : pMeta.m_shapeList) {
    num_of_shape++;
    fprintf(weightFilePtr, "#define SHAPE%d {", num_of_shape);
    for (int index = 0; index < 2; index++) {
      if (index != 1) {
        fprintf(weightFilePtr, "%d,", shapeNode.shape_value[index]);
      } else {
        fprintf(weightFilePtr, "%d", shapeNode.shape_value[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

void writeConvValue(FILE* weightFilePtr, const CortexmBackendMeta& pMeta)
{
  unsigned int num_of_weight = 0;
  for (const auto& weightNode : pMeta.m_weightList) {
    num_of_weight++;

    // Weight
    fprintf(weightFilePtr, "#define CONV%d_WT {", num_of_weight);
    for (int index = 0; index < weightNode.weight_value.size(); index++) {
      if (index != (weightNode.weight_value.size() - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(weightNode.weight_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(weightNode.weight_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");

    // Bias
    fprintf(weightFilePtr, "#define CONV%d_BIAS {", num_of_weight);
    for (int index = 0; index < weightNode.bias_value.size(); index++) {
			if (index != (weightNode.bias_value.size() - 1)) {
				fprintf(weightFilePtr, "%d,", (int)(weightNode.bias_value[index]));
			} else {
				fprintf(weightFilePtr, "%d", (int)(weightNode.bias_value[index]));
			}
    }
    fprintf(weightFilePtr, "}\n");
  }
}

} // namespace internal
} // namespace onnc

CortexmWeightFileGenPass::CortexmWeightFileGenPass(const CortexmBackendMeta& pMeta)
  : m_pMeta(pMeta)
{}

Pass::ReturnType CortexmWeightFileGenPass::runOnModule(Module& pModule)
{
  generateWeightFile();
  return Pass::kModuleNoChanged;
}

void CortexmWeightFileGenPass::generateWeightFile()
{
  FILE* weightFilePtr = fopen("cortexm_weight.h", "w");

  internal::writeRightShiftValue(weightFilePtr, m_pMeta);
  internal::writeAddValue(weightFilePtr, m_pMeta);
  internal::writeMatMulValue(weightFilePtr, m_pMeta);
  internal::writeShapeValue(weightFilePtr, m_pMeta);
  internal::writeConvValue(weightFilePtr, m_pMeta);

  fclose(weightFilePtr);
}
