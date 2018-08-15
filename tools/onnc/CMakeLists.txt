add_executable(onnc
    main.cpp
    ONNCApp.cpp
    ONNCConfig.cpp
)
target_link_libraries(onnc
    libonnc
)

install(TARGETS onnc
    RUNTIME DESTINATION bin
)
