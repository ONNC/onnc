set(NVDLA_BACKEND_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${NVDLA_BACKEND_ROOT})
include_directories(SYSTEM ${NVDLA_BACKEND_ROOT}/include)

add_libonnc_src(
    CodeEmitVisitor.cpp
    fp16.c
    Loadable.cpp
    NvDlaBackend.cpp
    NvDlaFileGenPass.cpp
    NvDlaMemInfoPass.cpp
    NvDlaMeta.cpp
    NvDlaTaskSubmitPass.cpp
    Compute/NvDlaConvRelu.cpp
    Compute/NvDlaGemmRelu.cpp
    Compute/NvDlaConvReluMaxPool.cpp
    NvDlaFuseConvReluPass.cpp
    NvDlaFuseGemmReluPass.cpp
    NvDlaLayerFusionPass.cpp
)

add_subdirectory(TargetInfo)
