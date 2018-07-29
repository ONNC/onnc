set(bm_target_path ${CMAKE_CURRENT_SOURCE_DIR}/bmtarget)
set(bm_PYTHONPATH ${bm_target_path}/target)
set(tg_root ${CMAKE_CURRENT_SOURCE_DIR})

if (BMONNC_EXIST)
    add_definitions(-DBMONNC_EXIST)
    include_directories(${BMONNC_SOURCE_DIR}/src/include)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(SYSTEM ${tg_root}/include)
install(DIRECTORY include/ DESTINATION include)

add_libonnc_src(
    io.cpp
    TargetLowering.cpp
    TGBackend.cpp
    TGCodeEmitter.cpp
    TGCodeEmitPass.cpp
    ComputeOperator.cpp
    TGFuseOptimizer.cpp
    TGFuseOptimizerPass.cpp
    GlobalMemAllocPass
    TargetLoweringPass.cpp)

add_subdirectory(BM168x)
add_subdirectory(BM188x)
add_subdirectory(TargetInfo)

