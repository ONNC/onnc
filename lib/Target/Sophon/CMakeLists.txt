####################
#  Options
option(SOPHON_GEN_ASM_PROTO "generate asm.proto by bmtarget" OFF)
option(BMONNC_EXIST "generate asm.proto by bmtarget" OFF)

####################
#  Generate asm proto
if (SOPHON_GEN_ASM_PROTO)
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
add_subdirectory(BM168x)
add_subdirectory(BM188x)
set(onnc_Sophon_obj ${onnc_Sophon_obj} PARENT_SCOPE)
add_library(onnc_Sophon OBJECT
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
    "BM188x/AddDummyWeightPass.cpp"
    "BM188x/BM188xBackend.cpp"
    "BM188x/BM188xISelLowering.cpp"
    "BM188x/BM188xCodeEmitter.cpp"
    "BM188x/BM188xTargetTransformInfo.cpp"
    "BM188x/BM188xTargetMemInfo.cpp"
    "BM188x/BM188xVisitor.cpp"
    "BM188x/BM188xFuseOptimizer.cpp"
    "BM188x/CodeEmitVisitor.cpp"
    "BM188x/FillWeightVisitor.cpp"
    "BM188x/GenRuntimeInfoPass.cpp"
    "BM188x/GenWeightPass.cpp"
    "BM188x/PrepareCtablePass.cpp"
    "BM188x/UpdateCtablePass.cpp"
    "BM188x/TLLoad.cpp"
    "BM188x/TLStore.cpp"
    "BM188x/TGConv.cpp"
    "BM188x/TLConv.cpp"
    "BM188x/TGGemm.cpp"
    "BM188x/TGLRN.cpp"
    "BM188x/TGMaxPool.cpp"
    "BM188x/TLPool.cpp"
    "BM188x/TGAveragePool.cpp"
    "BM188x/TGGlobalAveragePool.cpp"
    "BM188x/TGRelu.cpp"
    "BM188x/TGLeakyRelu.cpp"
    "BM188x/TGUpsample.cpp"
    "BM188x/TGConcat.cpp"
    "BM188x/TGTranspose.cpp"
    "BM188x/TGSum.cpp"
    "BM188x/TGPRelu.cpp"
    "BM188x/TGScale.cpp"
    "BM188x/Weight.cpp"
    "BM188x/Compute/AveragePool.cpp"
    "BM188x/Compute/Concat.cpp"
    "BM188x/Compute/Conv.cpp"
    "BM188x/Compute/Gemm.cpp"
    "BM188x/Compute/GlobalAveragePool.cpp"
    "BM188x/Compute/Load.cpp"
    "BM188x/Compute/LRN.cpp"
    "BM188x/Compute/LeakyRelu.cpp"
    "BM188x/Compute/MaxPool.cpp"
    "BM188x/Compute/Pool.cpp"
    "BM188x/Compute/PRelu.cpp"
    "BM188x/Compute/Sum.cpp"
    "BM188x/Compute/Store.cpp"
    "BM188x/Compute/Scale.cpp"
    "BM188x/Compute/SlicedConv.cpp"
    "BM188x/Compute/Relu.cpp"
    "BM188x/Compute/Transpose.cpp"
    "BM188x/Compute/Upsample.cpp"
    "BM188x/Lowers/AveragePoolLower.cpp"
    "BM188x/Lowers/ConcatLower.cpp"
    "BM188x/Lowers/ConvLower.cpp"
    "BM188x/Lowers/GemmLower.cpp"
    "BM188x/Lowers/GlobalAveragePoolLower.cpp"
    "BM188x/Lowers/LoadLower.cpp"
    "BM188x/Lowers/LRNLower.cpp"
    "BM188x/Lowers/LeakyReluLower.cpp"
    "BM188x/Lowers/MaxPoolLower.cpp"
    "BM188x/Lowers/PoolLower.cpp"
    "BM188x/Lowers/PReluLower.cpp"
    "BM188x/Lowers/SlicedConvLower.cpp"
    "BM188x/Lowers/StoreLower.cpp"
    "BM188x/Lowers/SumLower.cpp"
    "BM188x/Lowers/TransposeLower.cpp"
    "BM188x/Lowers/ScaleLower.cpp"
    "BM188x/Lowers/ReluLower.cpp"
    "BM188x/Lowers/UpsampleLower.cpp"
)

file(COPY 
    "io.hpp"
    "bmkernel_api_base.h"
    DESTINATION ${PROJECT_BINARY_DIR}/include/onnc/Target/Sophon
)