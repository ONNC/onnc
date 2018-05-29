
include_directories(${ONNX_INCLUDE_DIRS})
add_definitions(-DONNX_NAMESPACE=onnx)

add_executable(onnx-dis main.cpp)
target_link_libraries(onnx-dis ${ONNX_LIBRARIES} ${PROTOBUF_LIBRARIES})
add_dependencies(onnx-dis build_external)

install(TARGETS onnx-dis
    RUNTIME DESTINATION bin)
