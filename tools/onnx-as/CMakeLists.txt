
include_directories(${ONNX_INCLUDE_DIRS})
add_definitions(-DONNX_NAMESPACE=onnx)

add_executable(onnx-as main.cpp)
target_link_libraries(onnx-as ${ONNX_LIBRARIES} ${PROTOBUF_LIBRARIES})
add_dependencies(onnx-as build_external)

install(TARGETS onnx-as
    RUNTIME DESTINATION bin)
