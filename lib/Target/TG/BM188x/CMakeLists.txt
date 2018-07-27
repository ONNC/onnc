
include(proto)
gen_proto_cpp(PROTO proto/common_calibration2.proto SRCS proto_src)

add_libonnc_src(
    ${proto_src}
    BM188xBackend.cpp
    BM188xISelLowering.cpp
    BM188xCodeEmitter.cpp
    BM188xTargetTransformInfo.cpp
    BM188xTargetMemInfo.cpp
    BM188xVisitor.cpp
    AddDummyWeightPass.cpp
    BM188xFuseOptimizer.cpp
    PrepareCtablePass.cpp
    UpdateCtablePass.cpp
    QuantizePass.cpp
    TLLoad.cpp
    TLStore.cpp
    TGConv.cpp
    TLConv.cpp
    TGGemm.cpp
    TGLRN.cpp
    TGMaxPool.cpp
    TLPool.cpp
    TGAveragePool.cpp
    TGGlobalAveragePool.cpp
    TGRelu.cpp
    TGLeakyRelu.cpp
    TGUpsample.cpp
    TGConcat.cpp
    TGTranspose.cpp
    TGSum.cpp
    TGPRelu.cpp
    TGScale.cpp
    Compute/AveragePool.cpp
    Compute/Concat.cpp
    Compute/Conv.cpp
    Lowers/AveragePoolLower.cpp
    Lowers/ConvLower.cpp)

add_subdirectory(asm)
