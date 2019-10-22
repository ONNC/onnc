
include_directories(${ONNC_INCLUDE_DIRS})

add_executable(onni main.cpp ONNIApp.cpp ONNIConfig.cpp
               InterpreterPass.cpp CountOperatorsPass.cpp)
target_link_libraries(onni libonnc onnc-rt)

install(TARGETS onni
    RUNTIME DESTINATION bin)
