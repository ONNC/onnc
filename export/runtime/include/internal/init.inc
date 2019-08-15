void ONNC_RUNTIME_init(){
#ifdef MKLDNN
    CHECK(mkldnn_engine_create(&engine, mkldnn_cpu, 0));
    CHECK(mkldnn_stream_create(&stream, engine, mkldnn_stream_default_flags));
#endif
}
