

set(LIB_ONNC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIB_ONNC_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
function(add_libonnc_src)
    file(RELATIVE_PATH name ${LIB_ONNC_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "_" name ${name})
    set(name libonnc_${name})
    add_library(${name} OBJECT ${ARGN})
    target_sources(libonnc PRIVATE $<TARGET_OBJECTS:${name}>)
    add_dependencies(${name} build_external)
    set(cur_target ${name} PARENT_SCOPE)
    add_dependencies(${name} code_gen)
endfunction()


find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${ONNX_INCLUDE_DIRS})
include_directories(${LIB_ONNC_BINARY_DIR}/include) # for proto
add_definitions(-DONNX_NAMESPACE=onnx)

add_library(libonnc "")
add_dependencies(libonnc build_external)
add_dependencies(libonnc code_gen)
target_include_directories(libonnc PUBLIC ${ONNX_INCLUDE_DIRS})
target_include_directories(libonnc PUBLIC ${LIB_ONNC_BINARY_DIR}/include) # for proto
target_compile_definitions(libonnc PUBLIC -DONNX_NAMESPACE=onnx)
set_target_properties(libonnc PROPERTIES OUTPUT_NAME onnc)

add_subdirectory(ADT)
add_subdirectory(Analysis)
add_subdirectory(Config)
add_subdirectory(Core)
add_subdirectory(Diagnostic)
add_subdirectory(IR)
add_subdirectory(IRReader)
add_subdirectory(JSON)
add_subdirectory(ONNXWrapper)
add_subdirectory(Option)
add_subdirectory(Support)
add_subdirectory(Target)
add_subdirectory(Transforms)

if (HAVE_PTHREADS)
    target_link_libraries(libonnc pthread)
endif()
target_link_libraries(libonnc
    ${EXTERNAL_LIBRARY}
    ${Boost_LIBRARIES}
    glog
    ${ONNX_LIBRARIES}
    ${PROTOBUF_LIBRARIES})

install(TARGETS libonnc
    ARCHIVE DESTINATION lib)
