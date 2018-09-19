
include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${ONNX_INCLUDE_DIRS})
include_directories(${SKYPAT_INCLUDE_DIRS})

add_definitions(-DTOPDIR="${onnc_SOURCE_DIR}")
add_definitions(-DBUILDDIR="${onnc_BINARY_DIR}")

if (BMONNC_EXIST)
    add_definitions(-DBMONNC_EXIST)
    include_directories(${BMONNC_SOURCE_DIR}/src/include)
endif()

add_executable(onnx2tg
    main.cpp
    ONNX2TGApp.cpp
    Config.cpp
)

llvm_map_components_to_libnames(llvm_libs option)
target_link_libraries(onnx2tg
    libonnc
    ${llvm_libs}
)

if (BMONNC_EXIST)
    target_link_libraries(onnx2tg foo)
    target_link_libraries(onnx2tg bmnetc)
endif()

install(TARGETS onnx2tg
    RUNTIME DESTINATION bin)

add_test(NAME onnx2tg COMMAND onnx2tg dummy_input -h)
