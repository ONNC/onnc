
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

add_subdirectory(asm)
