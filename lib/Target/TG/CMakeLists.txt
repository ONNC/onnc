

include_directories(
    ${BMTAP_BINARY_DIR}/bmnet/src
    ${BMTAP_INSTALL_DIR}/include
    ${BMTAP_SOURCE_DIR}/bmnet/include
    ${BMTAP_SOURCE_DIR}/bmnet/include/targets
    ${BMTAP_SOURCE_DIR}/bmnet/src/targets/plat-bm168x/bmkernel)
include_directories(${PYTORCH_INCLUDE_DIRS})

add_libonnc_src(
    TargetLowering.cpp 
    TargetInfo/TGTargetInfo.cpp 
    TGBackend.cpp 
    TGISelLowering.cpp 
    TGCodeEmitter.cpp 
    BM1880Backend.cpp
    BM1880ISelLowering.cpp
    BM1880CodeEmitter.cpp
    TGCodeEmitPass.cpp 
    TGMemAllocInfoPass.cpp 
    TargetLoweringPass.cpp
    TGConv.cpp 
    TGGemm.cpp 
    TGLRN.cpp  
    TGMaxPool.cpp 
    TGRelu.cpp 
    TGSoftmax.cpp)
