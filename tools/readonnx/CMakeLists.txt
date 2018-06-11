
include_directories(${ONNC_INCLUDE_DIRS})
add_executable(readonnx main.cpp ReadONNXApp.cpp ReadONNXConfig.cpp)
target_link_libraries(readonnx libonnc)

install(TARGETS onnc
    RUNTIME DESTINATION bin)
