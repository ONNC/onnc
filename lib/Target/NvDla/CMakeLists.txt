set(NVDLA_BACKEND_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${NVDLA_BACKEND_ROOT})
include_directories(SYSTEM ${NVDLA_BACKEND_ROOT}/include)

add_libonnc_src(
    CodeEmitVisitor.cpp
    Loadable.cpp
    NvDlaBackend.cpp
    NvDlaCalibrateAveragePoolResultPass.cpp
    NvDlaCollectReshapeInfoPass.cpp
    NvDlaDefine.cpp
    NvDlaFileGenPass.cpp
    NvDlaIdentifyShufflePass.cpp
    NvDlaMemInfoPass.cpp
    NvDlaMeta.cpp
    NvDlaTaskSubmitPass.cpp
    NvDlaUtil.cpp
    SplitGroupConvPass.cpp
)

add_subdirectory(genmem)
add_subdirectory(genmemNvdla)
add_subdirectory(TargetInfo)
add_subdirectory(Compute)
add_subdirectory(Config)
