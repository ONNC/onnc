#pragma once

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
void *ONNC_RUNTIME_init_runtime();

/**
 * Shutdown runtime.
 * @deprecated
 * @param onnc_runtime_context The ONNC Runtime Context.
 * @return True if shutdown successfully. False if something wrong.
 */
bool ONNC_RUNTIME_shutdown_runtime(void *onnc_runtime_context);

/**
 * Get tensor address from tensor table.
 * @param tensor_table The tensor table start address.
 * @param index Tensor index.
 * @return The memory address of the TensorTable[index].
 */
void *ONNC_RUNTIME_load_from_tensor_table(void *tensor_table, uint32_t index);

${include_operators}
