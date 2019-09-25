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

struct weight_list* first_weight;
struct add_list* first_add;
struct matmul_list* first_matmul;
struct shape_list* first_shape;
struct shift_list* first_shift;

static void writeRightShiftValue(FILE* weightFilePtr) {
  unsigned int num_of_shift = 0;
  for (struct shift_list* now_shift = first_shift; now_shift != NULL;
       now_shift = now_shift->next) {
    num_of_shift++;

    fprintf(weightFilePtr, "#define RIGHT_SHIFT%d %d\n", num_of_shift,
            now_shift->shift_value);
  }
}

static void writeAddValue(FILE* weightFilePtr) {
  unsigned int num_of_add = 0;
  for (struct add_list* now_add = first_add; now_add != NULL;
       now_add = now_add->next) {
    num_of_add++;
    fprintf(weightFilePtr, "#define ADD%d {", num_of_add);
    for (int index = 0; index < now_add->add_size; index++) {
      if (index != (now_add->add_size - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(now_add->add_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(now_add->add_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");

    fprintf(weightFilePtr, "#define INPUT_DIMS%d {", num_of_add);
    for (int index = 0; index < now_add->input_dims_size; index++) {
      if (index != (now_add->input_dims_size - 1)) {
        errs() << "output = " << now_add->input_dims[index] << "\n";
        fprintf(weightFilePtr, "%d,", now_add->input_dims[index]);
      } else {
        errs() << "output = " << now_add->input_dims[index] << "\n";
        fprintf(weightFilePtr, "%d", now_add->input_dims[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");

    fprintf(weightFilePtr, "#define ADD_DIMS%d {", num_of_add);
    for (int index = 0; index < now_add->add_dims_size; index++) {
      if (index != (now_add->add_dims_size - 1)) {
        fprintf(weightFilePtr, "%d,", now_add->add_dims[index]);
      } else {
        fprintf(weightFilePtr, "%d", now_add->add_dims[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

static void writeMatMulValue(FILE* weightFilePtr) {
  unsigned int num_of_matmul = 0;
  for (struct matmul_list* now_matmul = first_matmul; now_matmul != NULL;
       now_matmul = now_matmul->next) {
    num_of_matmul++;
    fprintf(weightFilePtr, "#define MATMUL_WEIGHT%d {", num_of_matmul);
    for (int index = 0; index < now_matmul->matmul_size; index++) {
      if (index != (now_matmul->matmul_size - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(now_matmul->matmul_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(now_matmul->matmul_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

static void writeShapeValue(FILE* weightFilePtr) {
  unsigned int num_of_shape = 0;
  for (struct shape_list* now_shape = first_shape; now_shape != NULL;
       now_shape = now_shape->next) {
    num_of_shape++;
    fprintf(weightFilePtr, "#define SHAPE%d {", num_of_shape);
    for (int index = 0; index < 2; index++) {
      if (index != 1) {
        fprintf(weightFilePtr, "%d,", now_shape->shape_value[index]);
        printf("%d,", now_shape->shape_value[index]);
      } else {
        fprintf(weightFilePtr, "%d", now_shape->shape_value[index]);
        printf("%d\n", now_shape->shape_value[index]);
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

static void writeConvValue(FILE* weightFilePtr) {
  unsigned int num_of_weight = 0;
  for (struct weight_list* now_weight = first_weight; now_weight != NULL;
       now_weight = now_weight->next) {
    num_of_weight++;

    // Weight
    fprintf(weightFilePtr, "#define CONV%d_WT {", num_of_weight);
    for (int index = 0; index < now_weight->weight_size; index++) {
      if (index != (now_weight->weight_size - 1)) {
        fprintf(weightFilePtr, "%d,", (int)(now_weight->weight_value[index]));
      } else {
        fprintf(weightFilePtr, "%d", (int)(now_weight->weight_value[index]));
      }
    }
    fprintf(weightFilePtr, "}\n");

    // Bias
    fprintf(weightFilePtr, "#define CONV%d_BIAS {", num_of_weight);
    for (int index = 0; index < now_weight->bias_size; index++) {
      if (now_weight->have_bias == true) {
        if (index != (now_weight->bias_size - 1)) {
          fprintf(weightFilePtr, "%d,", (int)(now_weight->bias_value[index]));
        } else {
          fprintf(weightFilePtr, "%d", (int)(now_weight->bias_value[index]));
        }
      } else {
        if (index != (now_weight->bias_size - 1)) {
          fprintf(weightFilePtr, "0,");
        } else {
          fprintf(weightFilePtr, "0");
        }
      }
    }
    fprintf(weightFilePtr, "}\n");
  }
}

CortexmWeightFileGenPass::CortexmWeightFileGenPass(TargetBackend* pBackend,
                                                   CortexmBackendMeta* pMeta)
    : m_pBackend(pBackend), m_pMeta(m_pMeta) {}

Pass::ReturnType CortexmWeightFileGenPass::runOnModule(Module& pModule) {
  generateWeightFile();
  return Pass::kModuleNoChanged;
}

void CortexmWeightFileGenPass::generateWeightFile() {
  FILE* weightFilePtr = fopen("cortexm_weight.h", "w");

  writeRightShiftValue(weightFilePtr);
  writeAddValue(weightFilePtr);
  writeMatMulValue(weightFilePtr);
  writeShapeValue(weightFilePtr);
  writeConvValue(weightFilePtr);
}
