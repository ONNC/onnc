#include <onnc/Runtime/onnc-runtime-internal.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/mman.h>

void *ONNC_RUNTIME_init_runtime() {
  Context *context = (Context *)calloc(1 , sizeof(Context));
  // XXX: design!
  context->mem = (void **)calloc(2048 , sizeof(void *));
  context->mem_i = 0;

  return context;
}

bool ONNC_RUNTIME_shutdown_runtime(void *onnc_runtime_context) {
  if (onnc_runtime_context == NULL) {
    return true;
  }

  Context *context = (Context *)onnc_runtime_context;
  for (size_t i = 0; i < context->mem_i; ++i) {
    free(context->mem[i]);
  }

  free(context);
  return true;
}

/*
void *ONNC_RUNTIME_internal_allocate_memory(void *onnc_runtime_context, size_t num, size_t size) {
  Context *context = (Context *)onnc_runtime_context;
  void *mem = calloc(num , size);
  context->mem[context->mem_i] = mem;
  context->mem_i += 1;
  return mem;
}
*/

void *ONNC_RUNTIME_load_from_tensor_table(void *tensor_table,
                                          uint32_t index) {
  TensorOffsetTable *ttable = (TensorOffsetTable *)tensor_table;
  return tensor_table + ttable->tensor_offsets[index].offset;
}
