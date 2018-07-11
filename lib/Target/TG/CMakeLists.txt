

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BMTAP_BINARY_DIR}/Build/bmnet/src
    ${BMTAP_BINARY_DIR}/Build/bmnet
    ${BMTAP_INSTALL_DIR}/include)
include_directories(SYSTEM
    ${BMTAP_SOURCE_DIR}/bmnet/include)

add_libonnc_src(
    TargetLowering.cpp
    TGBackend.cpp
    TGCodeEmitPass.cpp
    ComputeOperator.cpp
    TGFuseOptimizer.cpp
    TGFuseOptimizerPass.cpp
    GlobalMemAllocPass
    TargetLoweringPass.cpp)

add_subdirectory(BM168x)
add_subdirectory(BM188x)
add_subdirectory(TargetInfo)
add_subdirectory(test)

install(DIRECTORY ${BMTAP_INSTALL_DIR}/lib/
    DESTINATION lib)
