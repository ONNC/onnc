#include "CortexmMainFileHeaderGenPass.h"

#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Timer.h>

using namespace onnc;

CortexmMainFileHeaderGenPass::CortexmMainFileHeaderGenPass(TargetBackend *pBackend, CortexMBackendMeta *pMeta)
  : ModulePass(ID),
    m_pBackend(pBackend), m_pMeta(m_pMeta){
}

Pass::ReturnType CortexmMainFileHeaderGenPass::runOnModule(Module& pModule){
  FILE *file;
  file = fopen("vanilla_out.h","w");
  fprintf(file,"#ifndef VANILLA_OUT_H\n\
#define VANILLA_OUT_H\n\n\
#include <stdio.h>\n\
#include <stdlib.h>\n\
int vanilla_main(bool input_per_proc , int* image_data);\n\n\
#endif\n\
");

  return Pass::kModuleNoChanged;
}

char CortexmMainFileHeaderGenPass::ID = 0;

CortexmMainFileHeaderGenPass *onnc::CreateCortexmMainFileHeaderGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta){
  return new CortexmMainFileHeaderGenPass(pBackend, pMeta);
}
