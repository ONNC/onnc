#include "CortexmReadQuantizationConfigPass.h"
#include "CodeEmitVisitor.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/Path.h>
#include <onnc/Support/Timer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace onnc;

CortexmReadQuantizationConfigPass::CortexmReadQuantizationConfigPass(TargetBackend* pBackend,
                                           CortexmBackendMeta* pMeta,
                                           const Path file)
    : m_pBackend(pBackend), m_pMeta(pMeta), m_file(file) {}

Pass::ReturnType CortexmReadQuantizationConfigPass::runOnModule(Module& pModule) {
  FILE* shift_file;
  shift_file = fopen(m_file.c_str(), "r");
  char shift_data[50] = "";
  if (!shift_file) {
    errs() << "Fail to open quantization config file\n";
  } else {
    fread(shift_data, 50, 1, shift_file);
    const char* spilt_char = ",";
    char* all_sub_data;
    all_sub_data = strtok(shift_data, spilt_char);
    while (all_sub_data != NULL) {
      int shift_number = atoi(all_sub_data);
      all_sub_data = strtok(NULL, spilt_char);

			CortexmBackendMeta::Shift shiftNode;
			shiftNode.shift_value = shift_number;
			m_pMeta->m_shiftList.emplace_back(shiftNode);
    }
    fclose(shift_file);
  }

  return Pass::kModuleNoChanged;
}
