#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_${operator_name}_float(
  void * restrict onnc_runtime_context
  ${input_parameters_float}
  ${output_parameters_float}
  ${attribute_paprameters_float}
);
