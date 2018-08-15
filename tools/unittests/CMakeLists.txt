add_definitions(-DTOPDIR="${ONNC_ROOT_PATH}")
add_definitions(-DBUILDDIR="${PROJECT_BINARY_DIR}")

if (ENABLE_UNITTEST)
    add_library(unittests_main main.cpp)
    target_link_libraries(unittests_main 
        libonnc
        skypat
    )
endif()


function(add_onnc_test name)
    if (ENABLE_UNITTEST)
        add_executable(unittest_${name} ${ARGN})
        target_link_libraries(unittest_${name} unittests_main)
        add_test(${name} unittest_${name})
    endif()
endfunction()
add_onnc_test(Digraph DigraphTest.cpp)
add_onnc_test(FileHandle FileHandleTest.cpp)
add_onnc_test(PassManager PassManagerTest.cpp)
add_onnc_test(Quadruple QuadrupleTest.cpp)
add_onnc_test(StringRef StringRefTest.cpp)
add_onnc_test(Any AnyTest.cpp)
add_onnc_test(BinaryTree BinaryTreeTest.cpp)
add_onnc_test(StringSwitch StringSwitchTest.cpp)
add_onnc_test(StringMap StringMapTest.cpp)
add_onnc_test(Json JsonValueTest.cpp JsonObjectTest.cpp)
add_onnc_test(ComputeIR ComputeIRTest.cpp)
add_onnc_test(TensorSel TensorSelTest.cpp)
