
include_directories(${ONNC_INCLUDE_DIRS})
add_executable(onnc-jit main.cpp ONNCJITApp.cpp ONNCJITConfig.cpp)
target_link_libraries(onnc-jit libonnc onnc-rt)

install(TARGETS onnc-jit
    RUNTIME DESTINATION bin)
