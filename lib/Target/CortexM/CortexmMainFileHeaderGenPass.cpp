#include "CortexmMainFileHeaderGenPass.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

using namespace onnc;

Pass::ReturnType CortexmMainFileHeaderGenPass::runOnModule(Module& pModule)
{
  FILE* filePtr = fopen("cortexm_main.h", "w");

  if (!filePtr) {
    return Pass::kPassFailure;
  }

  fprintf(filePtr, "\
#ifndef CORTEXM_OUT_H\n\
#define CORTEXM_OUT_H\n\n\
#include <stdio.h>\n\
#include <stdlib.h>\n\
#include \"arm_math.h\"\n\
q7_t* cortexm_main(int* image_data);\n\n\
#endif\n\
");

  fclose(filePtr);

  return Pass::kModuleNoChanged;
}
