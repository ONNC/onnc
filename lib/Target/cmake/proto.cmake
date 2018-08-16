
function(gen_proto_cpp)
    cmake_parse_arguments(OPT "" "SOURCE;PROTO;DEPEND;OBJECT" "" ${ARGN})
    # prepare proto name
    get_filename_component(OPT_PROTO ${OPT_PROTO} ABSOLUTE)
    get_filename_component(proto_name ${OPT_PROTO} NAME)
    get_filename_component(proto_path ${OPT_PROTO} DIRECTORY)
    get_filename_component(proto_name_we ${OPT_PROTO} NAME_WE)
    # set return variable
    file(RELATIVE_PATH relative_path ${ONNC_ROOT_PATH}/lib ${proto_path})
    set(out_path ${PROJECT_BINARY_DIR}/include/onnc/${relative_path})
    set(${OPT_SOURCE} "${out_path}/${proto_name_we}.pb.cc" PARENT_SCOPE)
    set(${OPT_OBJECT} "${proto_name_we}_object" PARENT_SCOPE)
    # build
    if(OPT_DEPEND)
        add_custom_command(
            OUTPUT "${out_path}/${proto_name_we}.pb.cc"
            COMMAND mkdir -p ${out_path}
            COMMAND protoc --proto_path=${proto_path} --cpp_out=${out_path} ${OPT_PROTO}
            DEPENDS ${OPT_PROTO} ${OPT_DEPEND}
        )
        add_library("${proto_name_we}_object" OBJECT
            "${out_path}/${proto_name_we}.pb.cc"
        )
    else(OPT_DEPEND)
        add_custom_command(
            OUTPUT "${out_path}/${proto_name_we}.pb.cc"
            COMMAND mkdir -p ${out_path}
            COMMAND protoc --proto_path=${proto_path} --cpp_out=${out_path} ${OPT_PROTO}
            DEPENDS ${OPT_PROTO}
        )
        add_library("${proto_name_we}_object" OBJECT
            "${out_path}/${proto_name_we}.pb.cc"
        )
    endif(OPT_DEPEND)
    
endfunction()