#include <onnc/Runtime/operator/randomnormallike.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static float randomNormal(float mean, float stddev)
{
  float x = (float)random() / (RAND_MAX + 1.0f);
  float y = (float)random() / (RAND_MAX + 1.0f);
  return mean + stddev * sqrtf(-2.0f * logf(x)) * cosf(2.0f * acosf(-1.0f) * y);
}

#include "internal/randomnormallike.inc"