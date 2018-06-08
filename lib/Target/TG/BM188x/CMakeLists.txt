
include_directories(SYSTEM ${BMTAP_SOURCE_DIR}/bmnet/src/targets/plat-bm188x/bmkernel)

include(proto)
gen_proto_cpp(NAME gen_bm188x_proto PROTO proto/bm188x_common_calibration.proto SRCS proto_src)

add_libonnc_src(
    ${proto_src}
    BM188xBackend.cpp
    BM188xISelLowering.cpp
    BM188xCodeEmitter.cpp
    TGConv.cpp
    TGGemm.cpp 
    TGLRN.cpp  
    TGMaxPool.cpp 
    TGRelu.cpp)
