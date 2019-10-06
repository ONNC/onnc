void ONNC_RUNTIME_terminate()
{
  mkldnn_stream_destroy(stream);
  mkldnn_engine_destroy(engine);
}
