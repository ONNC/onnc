

set(LIB_ONNC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
function(add_libonnc_src)
    file(RELATIVE_PATH name ${LIB_ONNC_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "_" name ${name})
    set(name libonnc_${name})
    add_library(${name} OBJECT ${ARGN})
    target_sources(libonnc PRIVATE $<TARGET_OBJECTS:${name}>)
    add_dependencies(${name} build_external)
    set(cur_target ${name} PARENT_SCOPE)
endfunction()


include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${ONNX_INCLUDE_DIRS})
add_definitions(-DONNX_NAMESPACE=onnx)

add_library(libonnc "")
add_dependencies(libonnc build_external)
set_target_properties(libonnc PROPERTIES OUTPUT_NAME onnc)

add_subdirectory(ADT)
add_subdirectory(Analysis)
add_subdirectory(Config)
add_subdirectory(Core)
add_subdirectory(Diagnostic)
add_subdirectory(IR)
add_subdirectory(IRReader)
add_subdirectory(JSON)
add_subdirectory(Option)
add_subdirectory(Support)
add_subdirectory(Target)

if (HAVE_PTHREADS)
    target_link_libraries(libonnc pthread)
endif()
target_link_libraries(libonnc
    ${ONNX_LIBRARIES}
    ${PROTOBUF_LIBRARIES})
if (TARGET_TG)
    target_link_libraries(libonnc ${BMLIB})
endif()

install(TARGETS libonnc
    ARCHIVE DESTINATION lib)
