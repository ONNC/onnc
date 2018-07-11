
include(proto)
gen_proto_cpp(PROTO proto/common_calibration2.proto SRCS proto_src)

add_libonnc_src(
    ${proto_src}
    BM188xBackend.cpp
    BM188xISelLowering.cpp
    BM188xCodeEmitter.cpp
    BM188xTargetTransformInfo.cpp
    BM188xTargetMemInfo.cpp
    AddDummyWeightPass.cpp
    BM188xFuseOptimizer.cpp
    PrepareCtablePass.cpp
    UpdateCtablePass.cpp
    QuantizePass.cpp
    TLLoad.cpp
    TGConv.cpp
    TLConv.cpp
    TGGemm.cpp
    TGLRN.cpp
    TGMaxPool.cpp
    TGAveragePool.cpp
    TGGlobalAveragePool.cpp
    TGRelu.cpp
    TGUpsample.cpp
    TGConcat.cpp
    TGTranspose.cpp
    TGSum.cpp
    TGPRelu.cpp)

add_subdirectory(asm)
