
include_directories(${ONNX_INCLUDE_DIRS})
add_definitions(-DONNX_NAMESPACE=onnx)


find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs option)

add_executable(onnx-as main.cpp)
target_link_libraries(onnx-as
    ${ONNX_LIBRARIES}
    ${llvm_libs}
    ${PROTOBUF_LIBRARIES})
add_dependencies(onnx-as build_external)

install(TARGETS onnx-as
    RUNTIME DESTINATION bin)
