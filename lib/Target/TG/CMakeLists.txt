

include_directories(
    ${BMTAP_BINARY_DIR}/bmnet/src
    ${BMTAP_INSTALL_DIR}/include
    ${BMTAP_SOURCE_DIR}/bmnet/include
    ${BMTAP_SOURCE_DIR}/bmnet/include/targets
    ${BMTAP_SOURCE_DIR}/bmnet/src/targets/plat-bm168x/bmkernel)
include_directories(${PYTORCH_INCLUDE_DIRS})

add_libonnc_src(
    TGBackend.cpp 
    TargetInfo/TGTargetInfo.cpp 
    TGCodeEmitPass.cpp 
    TGMemAllocInfoPass.cpp 
    TGCodeEmitter.cpp 
    TGConv.cpp 
    TGGemm.cpp 
    TGISelLowering.cpp 
    TGLRN.cpp  
    TGMaxPool.cpp 
    TGRelu.cpp 
    TGSoftmax.cpp 
    TargetLowering.cpp 
    TargetLoweringPass.cpp 
    RemoveUnusedNodePass.cpp 
    UpdateOutputInfoPass.cpp)
