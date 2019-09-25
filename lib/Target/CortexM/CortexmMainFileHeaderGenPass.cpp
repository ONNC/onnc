#include "CortexmMainFileHeaderGenPass.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

using namespace onnc;

CortexmMainFileHeaderGenPass::CortexmMainFileHeaderGenPass(
    TargetBackend* pBackend, CortexmBackendMeta* pMeta)
    : m_pBackend(pBackend), m_pMeta(m_pMeta) {}

Pass::ReturnType CortexmMainFileHeaderGenPass::runOnModule(Module& pModule) {
  FILE* file;
  file = fopen("cortexm_out.h", "w");
  fprintf(file, "#ifndef CORTEXM_OUT_H\n\
#define CORTEXM_OUT_H\n\n\
#include <stdio.h>\n\
#include <stdlib.h>\n\
#include \"arm_math.h\"\n\
q7_t* cortexm_main(int* image_data);\n\n\
#endif\n\
");

  return Pass::kModuleNoChanged;
}
