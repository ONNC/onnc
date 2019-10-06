set(LIB_ONNC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIB_ONNC_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

####################
#  Include
include_directories(${ONNC_INCLUDE_DIRS})
include_directories(${LIB_ONNC_BINARY_DIR}/include) # for proto
include_directories(${PROTOBUF_INCLUDE_DIR})

####################
#  libonnc-rt
SET(ONNC_RUNTIME_LIB_NAME "onnc-rt")

####################
#  libonnc
add_library(libonnc "")
add_dependencies(libonnc code_gen)
set_target_properties(libonnc PROPERTIES OUTPUT_NAME onnc)
function(add_libonnc_src) # For adding source file inside subdirectories
    file(RELATIVE_PATH name ${LIB_ONNC_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "_" name ${name})
    set(name libonnc_${name})
    add_library(${name} OBJECT ${ARGN})
    target_sources(libonnc PRIVATE $<TARGET_OBJECTS:${name}>)
    add_dependencies(${name} code_gen)
endfunction()

####################
#  Subdirectories
add_subdirectory(ADT)
add_subdirectory(Analysis)
add_subdirectory(CodeGen)
add_subdirectory(Config)
add_subdirectory(Core)
add_subdirectory(Diagnostic)
add_subdirectory(IR)
add_subdirectory(IRReader)
add_subdirectory(JSON)
add_subdirectory(ONNXWrapper)
add_subdirectory(Option)
add_subdirectory(Runtime)
add_subdirectory(Support)
add_subdirectory(Target)
add_subdirectory(Transforms)

####################
#  Linking
if (HAVE_PTHREADS)
    target_link_libraries(libonnc pthread)
endif()
target_link_libraries(libonnc
    glog
    ${ONNX_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
)

####################
#  General settings
install(TARGETS libonnc
    ARCHIVE DESTINATION lib
)
