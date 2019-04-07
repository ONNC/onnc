void ONNC_RUNTIME_upsample_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict mode
  ,float * restrict scales
  ,int32_t number_of_scales
) {
  // Calculate output conversion
  int32_t output_convs[output_Y_ndim];
  output_convs[output_Y_ndim - 1] = 1;
  for(int32_t i = output_Y_ndim - 2; i >= 0; --i){
    output_convs[i] = output_convs[i + 1] * output_Y_dims[i + 1];
  }
  // Calculate input conversion
  int32_t input_convs[input_X_ndim];
  input_convs[input_X_ndim - 1] = 1;
  for(int32_t i = input_X_ndim - 2; i >= 0; --i){
    input_convs[i] = input_convs[i + 1] * input_X_dims[i + 1];
  }
  // Nearest
  if(mode == NULL || !strcmp(mode, "nearest")){
    nearest_upsample(
      output_Y_ndim
      ,output_Y_dims
      ,output_convs
      ,input_convs
      ,scales
      ,input_X
      ,output_Y
    );
  }else{
    // TODO: linear
  }
}
