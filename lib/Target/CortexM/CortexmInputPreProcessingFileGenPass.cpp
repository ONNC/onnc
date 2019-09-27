#include "CortexmInputPreProcessingFileGenPass.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

using namespace onnc;

Pass::ReturnType CortexmInputPreProcessingFileGenPass::runOnModule(Module& pModule)
{
  FILE* filePtr = fopen("cortexm_input_pre_proc.h", "w");

  if (!filePtr) {
    return Pass::kPassFailure;
  }

  // include file
  fprintf(filePtr, "\
#include <stdint.h>\n\
#include <stdio.h>\n\
#include \"arm_math.h\"\n\
#include \"cortexm_weight.h\"\n\
#include \"math.h\"\n\
");

  // function declaration
  fprintf(filePtr,
          "void pre_processing(int* image_data , q7_t* img_buffer){\n"); // ongoing

  // function doing
  fprintf(filePtr, "    for(int i = 0 ; i < 28*28 ; i ++){\n\
  img_buffer[i] = (image_data[i]>>1);\n\
  }\n\n");

  // function end
  fprintf(filePtr, "}\n");
  fclose(filePtr);

  return Pass::kModuleNoChanged;
}
