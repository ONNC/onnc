#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#ifndef MKLDNN
#define MKLDNN
#include "mkldnn.h"
#include <stdio.h>
typedef struct {
  int nargs;
  mkldnn_exec_arg_t *args;
} args_t;

static void prepare_arg_node(args_t *node, int nargs) {
  node->args = (mkldnn_exec_arg_t *)malloc(sizeof(mkldnn_exec_arg_t) * nargs);
  node->nargs = nargs;
}
static void set_arg(
    mkldnn_exec_arg_t *arg, int arg_idx, mkldnn_memory_t memory) {
  arg->arg = arg_idx;
  arg->memory = memory;
}
#endif

void ONNC_RUNTIME_conv_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_W
  ,int32_t input_W_ndim, const int32_t * restrict input_W_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict auto_pad
  ,int32_t * restrict dilations
  ,int32_t number_of_dilations
  ,int32_t group
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t * restrict strides
  ,int32_t number_of_strides
);
