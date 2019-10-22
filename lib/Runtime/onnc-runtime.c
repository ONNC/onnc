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

  free(context->mem);
  free(context);
  return true;
}
