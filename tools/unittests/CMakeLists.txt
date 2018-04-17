
set(TEST_SOURCES 
    DigraphTest.cpp 
    FileHandleTest.cpp 
    PassManagerTest.cpp 
    QuadrupleTest.cpp 
    StringRefTest.cpp 
    AnyTest.cpp 
    BinaryTreeTest.cpp 
    StringSwitchTest.cpp)

include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${SKYPAT_INCLUDE_DIRS})

add_definitions(-DTOPDIR="${ONNC_SOURCE_DIR}/src")
add_definitions(-DBUILDDIR="${ONNC_BINARY_DIR}/src")

add_executable(unittests main.cpp ${TEST_SOURCES})
target_link_libraries(unittests libonnc
    ${SKYPAT_LIBRARIES})

add_test(test1 unittests)
