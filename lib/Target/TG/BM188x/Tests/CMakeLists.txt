include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${SKYPAT_INCLUDE_DIRS})

add_definitions(-DTOPDIR="${ONNC_SOURCE_DIR}")
add_definitions(-DBUILDDIR="${ONNC_BINARY_DIR}")

if (ENABLE_UNITTEST)
    add_library(libbm188x_test_main main.cpp)
    target_link_libraries(libbm188x_test_main libonnc
        ${SKYPAT_LIBRARIES})
endif()


function(add_onnc_test name)
    if (ENABLE_UNITTEST)
        add_executable(unittest_${name} ${ARGN})
        target_link_libraries(unittest_${name} libbm188x_test_main)
        add_test(${name} unittest_${name})
    endif()
endfunction()
add_onnc_test(BM188xOperator OperatorTest.cpp)
add_onnc_test(BM188xBackend BackendTest.cpp)
