#include "CortexmReadShiftPass.h"
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

extern struct shift_list* first_shift;

CortexmReadShiftPass::CortexmReadShiftPass(TargetBackend* pBackend,
                                           CortexmBackendMeta* pMeta,
                                           const Path file)
    : m_pBackend(pBackend), m_pMeta(m_pMeta), m_file(file) {}

Pass::ReturnType CortexmReadShiftPass::runOnModule(Module& pModule) {
  FILE* shift_file;
  shift_file = fopen(m_file.c_str(), "r");
  char shift_data[50] = "";
  if (!shift_file) {
    errs() << "no shift file\n";
  } else {
    fread(shift_data, 50, 1, shift_file);
    const char* spilt_char = ",";
    char* all_sub_data;
    all_sub_data = strtok(shift_data, spilt_char);
    while (all_sub_data != NULL) {
      int shift_number = atoi(all_sub_data);
      all_sub_data = strtok(NULL, spilt_char);
      errs() << shift_number << "\n";
      // save data
      if (first == 0) {
        first_shift = save_shift;
        save_shift->shift_value = shift_number;
        save_shift->next = NULL;
        first++;
      } else {
        struct shift_list* new_shift = (shift_list*)malloc(sizeof(shift_list));
        new_shift->shift_value = shift_number;
        new_shift->next = NULL;
        save_shift->next = new_shift;
        save_shift = new_shift;
      }
      // save finish
    }
    fclose(shift_file);
  }

  return Pass::kModuleNoChanged;
}
