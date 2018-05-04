
include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${ONNX_INCLUDE_DIRS})
include_directories(${SKYPAT_INCLUDE_DIRS})

add_definitions(-DTOPDIR="${ONNC_SOURCE_DIR}")
add_definitions(-DBUILDDIR="${ONNC_BINARY_DIR}")

add_executable(onnx2tg
    main.cpp
    ONNX2TGApp.cpp
    Config.cpp)
target_link_libraries(onnx2tg
    libonnc
    ${SKYPAT_LIBRARIES})

install(TARGETS onnx2tg
    RUNTIME DESTINATION bin)
