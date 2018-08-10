####################
#  Options
option(TG_GEN_ASM_PROTO "generate asm.proto by bmtarget" OFF)
option(BMONNC_EXIST "generate asm.proto by bmtarget" OFF)

####################
#  Generate asm proto
if (TG_GEN_ASM_PROTO)
    set(bmtarget_path ${CMAKE_CURRENT_SOURCE_DIR}/bmtarget)
    if (IS_DIRECTORY ${bmtarget_path}/target)
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${bmtarget_path}/target)
    else()
        message(FATAL_ERROR "can't find bmtarget path: ${bmtarget_path}")
    endif()
endif()

####################
#  Include
if (BMONNC_EXIST)
    add_definitions(-DBMONNC_EXIST)
    include_directories(${BMONNC_SOURCE_DIR}/src/include)
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

####################
#  Include
add_library(onnc_Sophon
    "io.cpp"
    "TargetLowering.cpp"
    "TGBackend.cpp"
    "TGCodeEmitter.cpp"
    "TGCodeEmitPass.cpp"
    "ComputeOperator.cpp"
    "TGFuseOptimizer.cpp"
    "TGFuseOptimizerPass.cpp"
    "GlobalMemAllocPass"
    "TargetLoweringPass.cpp"
    "BuildMemOpndPass.cpp"
    "EncodeInstructionsPass.cpp"
    "LinearScanAllocPass.cpp"
    "BM168x/BM168xBackend.cpp"
    "BM168x/BM168xISelLowering.cpp"
    "BM168x/BM168xCodeEmitter.cpp"
    "BM168x/TGConv.cpp"
    "BM168x/TGGemm.cpp" 
    "BM168x/TGLRN.cpp"
    "BM168x/TGMaxPool.cpp" 
    "BM168x/TGRelu.cpp"
    "BM168x/TGSoftmax.cpp"
    "TargetInfo/TGTargetInfo.cpp"
)

target_link_libraries(libonnc
    onnc_Sophon
)

add_subdirectory(BM168x)
# TODO: BM188x
#add_subdirectory(BM188x)

####################
#  Install
install()
