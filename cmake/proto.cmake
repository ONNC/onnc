
function(gen_proto_cpp)
    cmake_parse_arguments(OPT "" "NAME;SRCS;HDRS;PROTO" "" ${ARGN})
    get_filename_component(OPT_PROTO ${OPT_PROTO} ABSOLUTE)
    get_filename_component(proto_name ${OPT_PROTO} NAME_WE)
    get_filename_component(proto_path ${OPT_PROTO} DIRECTORY)
    set(hdrs ${CMAKE_CURRENT_BINARY_DIR}/${proto_name}.pb.h)
    set(srcs ${CMAKE_CURRENT_BINARY_DIR}/${proto_name}.pb.cc)
    add_custom_command(OUTPUT ${hdrs} ${srcs}
        COMMAND protoc --proto_path=${proto_path} --cpp_out=${CMAKE_CURRENT_BINARY_DIR} ${OPT_PROTO}
        DEPENDS ${OPT_PROTO})
    add_custom_target(${OPT_NAME}
        DEPENDS ${hdrs} ${srcs})
    set(${OPT_SRCS} ${srcs} PARENT_SCOPE)
    set(${OPT_HDRS} ${hdrs} PARENT_SCOPE)
    add_dependencies(code_gen ${OPT_NAME})
endfunction()
