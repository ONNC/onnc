
include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${ONNC_ROOT_PATH}/export/runtime/include)

add_executable(onni main.cpp ONNIApp.cpp ONNIConfig.cpp
               InterpreterPass.cpp CountOperatorsPass.cpp)
target_link_libraries(onni libonnc)

install(TARGETS onni
    RUNTIME DESTINATION bin)
