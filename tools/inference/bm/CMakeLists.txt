
include_directories(SYSTEM ${ONNC_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(SYSTEM ${BMTAP_SOURCE_DIR}/libbmruntime/include/)
include_directories(SYSTEM ${BMTAP_SOURCE_DIR}/bmkernel/common/include/)
include_directories(SYSTEM ${BMTAP_SOURCE_DIR}/libbmutils/include/)

# needed by libbmruntime
find_library(GLOG_LIB glog)

add_compile_options(-Werror -Wall)

add_executable(bm_runtime
    bm_runtime.cpp
    )

target_link_libraries(bm_runtime
    libonnc
    ${BMLIB}
    ${GLOG_LIB})

add_test(NAME bm_runtime COMMAND bm_runtime -h)
