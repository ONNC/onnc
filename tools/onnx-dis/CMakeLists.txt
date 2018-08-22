
include_directories(${ONNX_INCLUDE_DIRS})
add_definitions(-DONNX_NAMESPACE=onnx)

include_directories(${ONNC_INCLUDE_DIRS})

add_executable(onnx-dis main.cpp)
target_link_libraries(onnx-dis ${ONNX_LIBRARIES} ${PROTOBUF_LIBRARIES} libonnc)

install(TARGETS onnx-dis
    RUNTIME DESTINATION bin)
