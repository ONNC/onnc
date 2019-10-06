
include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${SKYPAT_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIR})

add_definitions(-DTOPDIR="${onnc_SOURCE_DIR}")
add_definitions(-DBUILDDIR="${onnc_BINARY_DIR}")

if (ENABLE_UNITTEST)
    add_library(libskypat_main main.cpp)
    target_link_libraries(libskypat_main libonnc onnc-rt
        ${SKYPAT_LIBRARIES})
endif()


function(add_onnc_test name)
    if (ENABLE_UNITTEST)
        add_executable(unittest_${name} ${ARGN})
        target_link_libraries(unittest_${name} libskypat_main)
        add_test(${name} unittest_${name})
    endif()
endfunction()

add_subdirectory(Optimizations)
add_subdirectory(Runtime)

add_onnc_test(Digraph DigraphTest.cpp)
add_onnc_test(FileHandle FileHandleTest.cpp)
add_onnc_test(PassManager PassManagerTest.cpp)
add_onnc_test(Quadruple QuadrupleTest.cpp)
add_onnc_test(StringRef StringRefTest.cpp)
add_onnc_test(Any AnyTest.cpp)
add_onnc_test(BinaryTree BinaryTreeTest.cpp)
add_onnc_test(StringSwitch StringSwitchTest.cpp)
add_onnc_test(StringMap StringMapTest.cpp)
add_onnc_test(Json JsonValueTest.cpp JsonObjectTest.cpp StorageTest.cpp)
add_onnc_test(ComputeIR ComputeIRTest.cpp)
add_onnc_test(TensorSel TensorSelTest.cpp)
add_onnc_test(StatisticsTest StatisticsTest.cpp)
add_onnc_test(MemAllocTest MemAllocTest.cpp)
add_onnc_test(CounterTest CounterTest.cpp)
