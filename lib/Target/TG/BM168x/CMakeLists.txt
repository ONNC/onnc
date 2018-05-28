
include_directories(
    ${BMTAP_SOURCE_DIR}/bmnet/src/targets/plat-bm168x/bmkernel)

add_libonnc_src(
    BM168xBackend.cpp
    BM168xISelLowering.cpp
    BM168xCodeEmitter.cpp
    TGConv.cpp 
    TGGemm.cpp 
    TGLRN.cpp  
    TGMaxPool.cpp 
    TGRelu.cpp 
    TGSoftmax.cpp)
