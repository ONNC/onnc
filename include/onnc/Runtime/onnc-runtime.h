#pragma once

#include <stdint.h>
#include <stdbool.h>

struct ONNC_RUNTIME_tensor_file {
  void* data; /* Implementation defined data */
};
    
struct ONNC_RUNTIME_tensor_offset {
  uint64_t offset; /* Tensor offset */
  uint64_t size;   /* Size of tensor in bytes */
};

#ifndef ONNC_RUNTIME_TENSOR_FILE_MAGIC
  #define ONNC_RUNTIME_TENSOR_FILE_MAGIC ".TSR"
#endif 
    
struct ONNC_RUNTIME_tensor_offset_table {
  uint8_t magic[8]; /* Tensor File magic number. */
  uint64_t number_of_tensors;
  struct ONNC_RUNTIME_tensor_offset tensor_offsets[];
};

struct ONNC_RUNTIME_tensor_view {
  void* data;
  uint64_t size; /* Size of tensor in bytes */
};

// Core Library
bool ONNC_RUNTIME_has_tensor(
  const struct ONNC_RUNTIME_tensor_offset_table* table,
  uint64_t tensor
);
struct ONNC_RUNTIME_tensor_offset ONNC_RUNTIME_get_tensor_offset(
  const struct ONNC_RUNTIME_tensor_offset_table* table,
  uint64_t tensor
);

// Client Library
const struct ONNC_RUNTIME_tensor_offset_table* ONNC_RUNTIME_read_tensor_offset_table(
  struct ONNC_RUNTIME_tensor_file* file
);

struct ONNC_RUNTIME_tensor_view ONNC_RUNTIME_read_tensor(
  struct ONNC_RUNTIME_tensor_file* file,
  uint64_t tensor
);

// Service Library
struct ONNC_RUNTIME_inference_context {
  struct ONNC_RUNTIME_tensor_file* input;
  struct ONNC_RUNTIME_tensor_file* weight;
  uint64_t id;
  void (*completed)(
    uint64_t id,
    struct ONNC_RUNTIME_tensor_view output
  );
};

/**
 * ONNC generated entry point.
 * @param context The ONNC Runtime Context.
 */
void model_main(void *context);

