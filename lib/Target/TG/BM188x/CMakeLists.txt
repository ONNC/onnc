
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CMAKE_CURRENT_BINARY_DIR}
    ${BMTAP_BINARY_DIR}/bmnet/src
    ${BMTAP_INSTALL_DIR}/include
    ${BMTAP_SOURCE_DIR}/bmnet/include
    ${BMTAP_SOURCE_DIR}/bmnet/include/targets
    ${BMTAP_SOURCE_DIR}/bmnet/src/targets/plat-bm188x/bmkernel)

include_directories(${PYTORCH_INCLUDE_DIRS})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/common_calibration.pb.cc
                   COMMAND protoc --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
                   --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
                   ${CMAKE_CURRENT_SOURCE_DIR}/common_calibration.proto
                   COMMENT "Generating common_calibration.pb.cc from common_calibration.proto"
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/common_calibration.proto)

add_libonnc_src(
    BM188xBackend.cpp
    BM188xISelLowering.cpp
    BM188xCodeEmitter.cpp
    TGConv.cpp 
    TGGemm.cpp 
    TGLRN.cpp  
    TGMaxPool.cpp 
    TGRelu.cpp 
    TGSoftmax.cpp)
