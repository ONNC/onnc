
option(TG_GEN_ASM_PROTO "generate asm.proto by bmtarget" OFF)

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
    TGBackend.cpp
    TGFuseOptimizer.cpp
    TGFuseOptimizerPass.cpp
    BuildMemOpndPass.cpp
    EncodeInstructionsPass.cpp
    LinearScanAllocPass.cpp)

add_subdirectory(BM168x)
add_subdirectory(BM188x)
add_subdirectory(TargetInfo)

