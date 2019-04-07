void ONNC_RUNTIME_randomnormal_float(
  void * restrict onnc_runtime_context
  
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,float mean
  ,float scale
  ,float seed
  ,int32_t * restrict shape
  ,int32_t number_of_shape
) {
  output_output_ndim = number_of_shape;

  int32_t dataSize = 1;
  for(int32_t i = 0; i < number_of_shape; i++){
    dataSize *= shape[i];
  }

  srand(seed);
  for(int32_t i = 0; i < dataSize; i++){
    output_output[i] = randomNormal(mean, scale);
  }
}
