
include_directories(${ONNC_INCLUDE_DIRS})

add_executable(onni main.cpp ONNIApp.cpp ONNIConfig.cpp Interpreter.cpp
               InterpreterPass.cpp CountOperatorsPass.cpp OnnxOptPass.cpp)
target_link_libraries(onni libonnc)

install(TARGETS onni
    RUNTIME DESTINATION bin)
