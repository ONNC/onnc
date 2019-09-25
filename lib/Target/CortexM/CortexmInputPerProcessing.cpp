#include "CortexmInputPerProcessing.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

using namespace onnc;

CortexmInputPerProcessing::CortexmInputPerProcessing(TargetBackend *pBackend,
                                                     CortexmBackendMeta *pMeta)
    : m_pBackend(pBackend), m_pMeta(m_pMeta) {}

Pass::ReturnType CortexmInputPerProcessing::runOnModule(Module &pModule) {
  FILE *file;
  file = fopen("cortexm_input_pre_proc.h", "w");
  // include file
  fprintf(file, "#include <stdint.h>\n\
#include <stdio.h>\n\
#include \"arm_math.h\"\n\
#include \"cortexm_weight.h\"\n\
#include \"math.h\"\n\
");

  // function declaration
  fprintf(
      file,
      "void pre_processing(int *image_data , q7_t* img_buffer){\n"); // ongoing

  // function doing
  fprintf(file, "    for(int i = 0 ; i < 28*28 ; i ++){\n\
  img_buffer[i] = (image_data[i]>>1);\n\
  }\n\n");

  // function end
  fprintf(file, "}\n");

  return Pass::kModuleNoChanged;
}
