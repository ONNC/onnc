
include_directories(SYSTEM ${BMTAP_SOURCE_DIR}/bmnet/src/targets/plat-bm188x/bmkernel)

include(proto)
gen_proto_cpp(PROTO proto/common_calibration2.proto SRCS proto_src)
gen_proto_cpp(PROTO proto/asm.proto SRCS proto_asm_src)

add_libonnc_src(
    ${proto_src}
    ${proto_asm_src}
    BM188xBackend.cpp
    BM188xISelLowering.cpp
    BM188xCodeEmitter.cpp
    BM188xTargetTransformInfo.cpp
    BM188xTargetMemInfo.cpp
    BM188xFuseOptimizer.cpp
    PrepareCtablePass.cpp
    UpdateCtablePass.cpp
    QuantizePass.cpp
    TGConv.cpp
    TGGemm.cpp
    TGLRN.cpp
    TGMaxPool.cpp
    TGAveragePool.cpp
    TGRelu.cpp
    TGSum.cpp)
