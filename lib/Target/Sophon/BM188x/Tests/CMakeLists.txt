add_definitions(-DTOPDIR="${ONNC_ROOT_PATH}")
add_definitions(-DBUILDDIR="${PROJECT_BINARY_DIR}")

if (ENABLE_UNITTEST)
    add_library(libbm188x_test_main main.cpp)
    target_link_libraries(libbm188x_test_main
        libonnc
        skypat
    )
    function(add_onnc_test name)
        add_executable(unittest_${name} ${ARGN})
        target_link_libraries(unittest_${name} libbm188x_test_main)
        add_test(${name} unittest_${name})
    endfunction()
    add_onnc_test(BM188xOperator OperatorTest.cpp)
    add_onnc_test(BM188xBackend BackendTest.cpp)
    add_onnc_test(BM188xWeight WeightTest.cpp)
endif()
