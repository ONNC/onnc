#pragma once

// internal
#include <stddef.h>

typedef struct ONNC_RUNTIME_Context {
  void *input_context;
  void *weight_context;
  void *output_context;
  void **mem; /* Deprecated */
  size_t mem_i; /* Deprecated */
} Context;


typedef struct ONNC_RUNTIME_Tensor_offset TensorOffset;
typedef struct ONNC_RUNTIME_Tensor_offset_table TensorOffsetTable;


#include <stdint.h>
#include <stdbool.h>

struct ONNC_RUNTIME_Tensor_offset {
  uint64_t offset; /* Tensor offset */
  uint64_t size;   /* Size of tensor in bytes */
};

#define ONNC_RUNTIME_TENSOR_FILE_MAGIC ".TSR"

struct ONNC_RUNTIME_Tensor_offset_table {
  uint8_t magic[8];                                    /* Tensor File magic number. */
  uint64_t number_of_tensors;
  struct ONNC_RUNTIME_Tensor_offset tensor_offsets[];
};

/**
 * ONNC generated entry point.
 * @param context The ONNC Runtime Context.
 */
void model_main(void *context);

/**
 * Initialize runtime.
 * @deprecated
 * @return The ONNC Runtime Context, should be passed to every ONNC Runtime functions.
 */
void *ONNC_RUNTIME_init_runtime() {
  Context *context = (Context *)calloc(1 , sizeof(Context));
  // XXX: design!
  context->mem = (void **)calloc(2048 , sizeof(void *));
  context->mem_i = 0;

  return context;
}

/**
 * Shutdown runtime.
 * @deprecated
 * @param onnc_runtime_context The ONNC Runtime Context.
 * @return True if shutdown successfully. False if something wrong.
 */
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

/**
 * Get tensor address from tensor table.
 * @param tensor_table The tensor table start address.
 * @param index Tensor index.
 * @return The memory address of the TensorTable[index].
 */
void *ONNC_RUNTIME_load_from_tensor_table(void *tensor_table, uint32_t index) {
  return NULL;
}
