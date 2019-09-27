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

static void writeRightShiftValue(FILE* weightFilePtr, CortexmBackendMeta* pMeta)
{
  unsigned int num_of_shift = 0;
  for (auto shiftItr = pMeta->m_shiftList.begin(); shiftItr != pMeta->m_shiftList.end(); ++shiftItr) {
    num_of_shift++;

    fprintf(weightFilePtr, "#define RIGHT_SHIFT%d %d\n", num_of_shift, shiftItr->shift_value);
  }
}

static void writeAddValue(FILE* weightFilePtr, CortexmBackendMeta* pMeta)
{
  unsigned int num_of_add = 0;
  for (auto addItr = pMeta->m_addList.begin(); addItr != pMeta->m_addList.end(); ++addItr) {
    num_of_add++;
    fprintf(weightFilePtr, "#define ADD%d {", num_of_add);
    for (int index = 0; index < addItr->add_size; index++) {
      if (index != (addItr->add_size - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(addItr->add_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(addItr->add_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");

    fprintf(weightFilePtr, "#define INPUT_DIMS%d {", num_of_add);
    for (int index = 0; index < addItr->input_dims_size; index++) {
      if (index != (addItr->input_dims_size - 1)) {
        fprintf(weightFilePtr, "%d,", addItr->input_dims[index]);
      } else {
        fprintf(weightFilePtr, "%d", addItr->input_dims[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");

    fprintf(weightFilePtr, "#define ADD_DIMS%d {", num_of_add);
    for (int index = 0; index < addItr->add_dims_size; index++) {
      if (index != (addItr->add_dims_size - 1)) {
        fprintf(weightFilePtr, "%d,", addItr->add_dims[index]);
      } else {
        fprintf(weightFilePtr, "%d", addItr->add_dims[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

static void writeMatMulValue(FILE* weightFilePtr, CortexmBackendMeta* pMeta)
{
  unsigned int num_of_matmul = 0;
  for (auto matmulItr = pMeta->m_matmulList.begin(); matmulItr != pMeta->m_matmulList.end(); ++matmulItr) {
    num_of_matmul++;
    fprintf(weightFilePtr, "#define MATMUL_WEIGHT%d {", num_of_matmul);
    for (int index = 0; index < matmulItr->matmul_size; index++) {
      if (index != (matmulItr->matmul_size - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(matmulItr->matmul_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(matmulItr->matmul_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

static void writeShapeValue(FILE* weightFilePtr, CortexmBackendMeta* pMeta)
{
  unsigned int num_of_shape = 0;
  for (auto shapeItr = pMeta->m_shapeList.begin(); shapeItr != pMeta->m_shapeList.end(); ++shapeItr) {
    num_of_shape++;
    fprintf(weightFilePtr, "#define SHAPE%d {", num_of_shape);
    for (int index = 0; index < 2; index++) {
      if (index != 1) {
        fprintf(weightFilePtr, "%d,", shapeItr->shape_value[index]);
      } else {
        fprintf(weightFilePtr, "%d", shapeItr->shape_value[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

static void writeConvValue(FILE* weightFilePtr, CortexmBackendMeta* pMeta)
{
  unsigned int num_of_weight = 0;
  for (auto weightItr = pMeta->m_weightList.begin(); weightItr != pMeta->m_weightList.end(); ++weightItr) {
    num_of_weight++;

    // Weight
    fprintf(weightFilePtr, "#define CONV%d_WT {", num_of_weight);
    for (int index = 0; index < weightItr->weight_size; index++) {
      if (index != (weightItr->weight_size - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(weightItr->weight_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(weightItr->weight_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");

    // Bias
    fprintf(weightFilePtr, "#define CONV%d_BIAS {", num_of_weight);
    for (int index = 0; index < weightItr->bias_size; index++) {
      if (weightItr->have_bias == true) {
        if (index != (weightItr->bias_size - 1)) {
          fprintf(weightFilePtr, "%d,", (int)(weightItr->bias_value[index]));
        } else {
          fprintf(weightFilePtr, "%d", (int)(weightItr->bias_value[index]));
        }
      } else {
        if (index != (weightItr->bias_size - 1)) {
          fprintf(weightFilePtr, "0,");
        } else {
          fprintf(weightFilePtr, "0");
        }
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

CortexmWeightFileGenPass::CortexmWeightFileGenPass(TargetBackend* pBackend, CortexmBackendMeta* pMeta)
  : m_pBackend(pBackend)
  , m_pMeta(pMeta)
{}

Pass::ReturnType CortexmWeightFileGenPass::runOnModule(Module& pModule)
{
  generateWeightFile();
  return Pass::kModuleNoChanged;
}

void CortexmWeightFileGenPass::generateWeightFile()
{
  FILE* weightFilePtr = fopen("cortexm_weight.h", "w");

  writeRightShiftValue(weightFilePtr, m_pMeta);
  writeAddValue(weightFilePtr, m_pMeta);
  writeMatMulValue(weightFilePtr, m_pMeta);
  writeShapeValue(weightFilePtr, m_pMeta);
  writeConvValue(weightFilePtr, m_pMeta);
}
