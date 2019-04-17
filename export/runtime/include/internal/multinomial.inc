int32_t multinomial(int i, int classSize, const float* input_input)
{
  float cum_prb[classSize];
  cum_prb[0] = input_input[i * classSize];
  for (int32_t j = 1; j < classSize; j++) {
    cum_prb[j] = cum_prb[j - 1] + input_input[i * classSize + j];
  }
  for (int32_t j = 0; j < classSize - 1; j++) {
    if (rand() / (float)(RAND_MAX) < cum_prb[j]) {
      return j;
    }
  }
  return classSize - 1;
}

#include "multinomial.impl.inc"
