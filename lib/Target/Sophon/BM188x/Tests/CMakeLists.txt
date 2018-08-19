include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${SKYPAT_INCLUDE_DIRS})

add_definitions(-DTOPDIR="${ONNC_SOURCE_DIR}")
add_definitions(-DBUILDDIR="${ONNC_BINARY_DIR}")

function(add_onnc_test name)
    if (ENABLE_UNITTEST)
       add_executable(unittest_${name} main.cpp ${ARGN})
       if (BMONNC_EXIST)
           target_link_libraries(unittest_${name} bmnetc libonnc
               ${SKYPAT_LIBRARIES})
       else()
           target_link_libraries(unittest_${name} libonnc
               ${SKYPAT_LIBRARIES})
       endif()
       add_test(${name} unittest_${name})
    endif()
endfunction()

add_onnc_test(BM188xOperator OperatorTest.cpp)
add_onnc_test(BM188xBackend BackendTest.cpp)
add_onnc_test(BM188xWeight WeightTest.cpp)
