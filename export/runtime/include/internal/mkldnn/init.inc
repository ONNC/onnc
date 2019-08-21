void ONNC_RUNTIME_init()
{
  mkldnn_engine_create(&engine, mkldnn_cpu, 0);
  mkldnn_stream_create(&stream, engine, mkldnn_stream_default_flags);
}
