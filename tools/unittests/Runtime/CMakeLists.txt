include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${SKYPAT_INCLUDE_DIRS})

function(add_onnc_runtime_test name)
    add_onnc_test(Runtime_${name} ${ARGN})
endfunction()

add_onnc_runtime_test(Abs AbsTest.cpp)
add_onnc_runtime_test(Transpose TransposeTest.cpp)