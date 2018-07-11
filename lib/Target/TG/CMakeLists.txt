
set(bm_PYTHONPATH ${CMAKE_CURRENT_SOURCE_DIR}/python)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_libonnc_src(
    io.cpp
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

