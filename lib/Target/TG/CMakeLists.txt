

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BMTAP_BINARY_DIR}/bmnet/src
    ${BMTAP_INSTALL_DIR}/include
    ${BMTAP_SOURCE_DIR}/bmnet/include
    ${BMTAP_SOURCE_DIR}/bmnet/include/targets
    ${PYTORCH_INCLUDE_DIRS})

add_libonnc_src(
    TargetLowering.cpp
    TGBackend.cpp
    TGCodeEmitPass.cpp
    ComputeOperator.cpp
    GlobalMemAllocPass
    TargetLoweringPass.cpp)

add_subdirectory(BM168x)
add_subdirectory(BM188x)
add_subdirectory(TargetInfo)
add_subdirectory(test)
