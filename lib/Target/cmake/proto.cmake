
function(gen_proto_cpp)
    cmake_parse_arguments(OPT "" "TARGET;SOURCE;HDRS;PROTO;DEPEND" "" ${ARGN})
    # prepare proto name
    get_filename_component(OPT_PROTO ${OPT_PROTO} ABSOLUTE)
    get_filename_component(proto_name ${OPT_PROTO} NAME)
    get_filename_component(proto_path ${OPT_PROTO} DIRECTORY)
    get_filename_component(proto_name_we ${OPT_PROTO} NAME_WE)
    # set return variable
    file(RELATIVE_PATH relative_path ${ONNC_ROOT_PATH}/lib ${proto_path})
    set(out_path ${PROJECT_BINARY_DIR}/proto/onnc/${relative_path})
    set(${OPT_SOURCE} "${out_path}/${proto_name_we}.pb.cc" PARENT_SCOPE)
    set(${OPT_HDRS} "${out_path}/${proto_name_we}.pb.h" PARENT_SCOPE)
    set(${OPT_TARGET} ${OPT_TARGET} PARENT_SCOPE)
    # build
    add_custom_target(${OPT_TARGET}
        COMMAND mkdir -p ${out_path}
        COMMAND protoc --proto_path=${proto_path} --cpp_out=${out_path} ${OPT_PROTO}
        COMMAND mkdir -p ${PROJECT_BINARY_DIR}/include/onnc/${relative_path}
        COMMAND mv ${out_path}/${proto_name_we}.pb.h ${PROJECT_BINARY_DIR}/include/onnc/${relative_path}/${proto_name_we}.pb.h
        DEPENDS ${OPT_PROTO}
    )
    # depend
    if(OPT_DEPEND)
        add_dependencies(${OPT_TARGET}
            ${OPT_DEPEND}
        )
    endif()
endfunction()