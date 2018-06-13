
include_directories(${ONNX_INCLUDE_DIRS})
add_definitions(-DONNX_NAMESPACE=onnx)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs option)

add_executable(onnx-dis main.cpp)
target_link_libraries(onnx-dis ${ONNX_LIBRARIES} ${PROTOBUF_LIBRARIES} ${llvm_libs})
add_dependencies(onnx-dis build_external)

install(TARGETS onnx-dis
    RUNTIME DESTINATION bin)
