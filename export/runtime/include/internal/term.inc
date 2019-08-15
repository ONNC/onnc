void ONNC_RUNTIME_term(){
#ifdef MKLDNN
    mkldnn_stream_destroy(stream);
    mkldnn_engine_destroy(engine);
#endif
}
