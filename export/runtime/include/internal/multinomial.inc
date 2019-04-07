void ONNC_RUNTIME_multinomial_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,int32_t sample_size
  ,float seed
) {
  srand(seed);
  int32_t batchSize = input_input_dims[0];
  int32_t classSize = input_input_dims[1];
  for(int32_t i = 0; i < batchSize; i++){
    for(int32_t j = 0; j < classSize; j++){
      output_output[i * classSize + j] = 0;
    }
  }
  int32_t index;
  for(int32_t i = 0; i < batchSize; i++){
    for(int32_t j = 0; j < sample_size; j++){
      index = multinomial(i, classSize, input_input);
      output_output[i * classSize + index] ++;
    }
  }
}
