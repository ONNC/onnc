if(ENABLE_X86_TARGET)
    add_executable(onnc-jit main.cpp ONNCJITApp.cpp ONNCJITConfig.cpp)
    target_link_libraries(onnc-jit libonnc)
    install(TARGETS onnc-jit
        RUNTIME DESTINATION bin
    )
endif(ENABLE_X86_TARGET)
