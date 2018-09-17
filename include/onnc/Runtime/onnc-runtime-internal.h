#pragma once

#include "onnc-runtime.h"

#include <stddef.h>

typedef struct ONNC_RUNTIME_Context {
  void *input_context;
  void *weight_context;
  void *output_context;
  void **mem; /* Deprecated */
  size_t mem_i; /* Deprecated */
} Context;


//void *ONNC_RUNTIME_internal_allocate_memory(void *onnc_runtime_context, size_t num, size_t size);

//void *ONNC_RUNTIME_initial_temporary_tensor_memory(void *onnc_runtime_context);

typedef struct ONNC_RUNTIME_Tensor_offset TensorOffset;
typedef struct ONNC_RUNTIME_Tensor_offset_table TensorOffsetTable;

