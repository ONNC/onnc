
include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${ONNX_INCLUDE_DIRS})
include_directories(${SKYPAT_INCLUDE_DIRS})

add_definitions(-DTOPDIR="${ONNC_SOURCE_DIR}")
add_definitions(-DBUILDDIR="${ONNC_BINARY_DIR}")

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs option)

if (BMNETC_EXIST)
    add_definitions(-DBMNETC_EXIST)
    include_directories(${BMNETC_TEMP_SOURCE_DIR}/src/include)
endif()

add_executable(onnx2tg
    main.cpp
    ONNX2TGApp.cpp
    Config.cpp)
target_link_libraries(onnx2tg
    libonnc
    ${llvm_libs})

if (BMNETC_EXIST)
    target_link_libraries(onnx2tg foo)
endif()

install(TARGETS onnx2tg
    RUNTIME DESTINATION bin)

add_test(NAME onnx2tg COMMAND onnx2tg dummy_input -h)
