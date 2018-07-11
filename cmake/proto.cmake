
function(gen_proto_cpp)
    cmake_parse_arguments(OPT "" "TARGET;SRCS;HDRS;PROTO" "" ${ARGN})
    #prepare proto name
    get_filename_component(OPT_PROTO ${OPT_PROTO} ABSOLUTE)
    get_filename_component(proto_name ${OPT_PROTO} NAME)
    get_filename_component(proto_name_we ${OPT_PROTO} NAME_WE)
    get_filename_component(proto_path ${OPT_PROTO} DIRECTORY)
    #prepare target name and path
    file(RELATIVE_PATH relative_path ${LIB_ONNC_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    set(target_name libonnc_${relative_path}__${proto_name})
    string(REPLACE "/" "_" target_name ${target_name})
    string(REPLACE "." "_" target_name ${target_name})
    set(${OPT_TARGET} ${target_name} PARENT_SCOPE)
    set(out_path ${LIB_ONNC_BINARY_DIR}/include/onnc/${relative_path})
    #set return variable
    set(hdrs ${out_path}/${proto_name_we}.pb.h)
    set(srcs ${out_path}/${proto_name_we}.pb.cc)
    set(${OPT_SRCS} ${srcs} PARENT_SCOPE)
    set(${OPT_HDRS} ${hdrs} PARENT_SCOPE)
    #build
    add_custom_target(${target_name}_dir
        COMMAND ${CMAKE_COMMAND} -E make_directory ${out_path})
    add_custom_command(OUTPUT ${hdrs} ${srcs}
        COMMAND protoc --proto_path=${proto_path} --cpp_out=${out_path} ${OPT_PROTO}
        DEPENDS ${target_name}_dir ${OPT_PROTO})
    add_custom_target(${target_name}
        DEPENDS ${hdrs} ${srcs})
    add_dependencies(code_gen ${target_name})
    #install
    INSTALL(FILES ${hdrs}
        DESTINATION include/onnc/${relative_path})
endfunction()
