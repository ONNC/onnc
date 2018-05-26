

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BMTAP_BINARY_DIR}/bmnet/src
    ${BMTAP_INSTALL_DIR}/include
    ${BMTAP_SOURCE_DIR}/bmnet/include
    ${BMTAP_SOURCE_DIR}/bmnet/include/targets)
include_directories(${PYTORCH_INCLUDE_DIRS})

add_libonnc_src(
    TargetLowering.cpp 
    TargetInfo/TGTargetInfo.cpp 
    TGBackend.cpp 
    TGCodeEmitPass.cpp 
    ComputeOperand.cpp
    TGMemAllocInfoPass.cpp 
    TargetLoweringPass.cpp)

add_subdirectory(BM168x)
add_subdirectory(BM188x)
