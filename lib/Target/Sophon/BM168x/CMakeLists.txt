if (TG_GEN_ASM_PROTO)
    include(proto)
    include(bmtarget)
    set(bm168x_asm_proto ${CMAKE_CURRENT_SOURCE_DIR}/asm/bm168x_asm.proto)

    # gen bm168x_asm.proto and bmkernel_api.h
    bm_gen_asm_proto(
        TARGET bm168x_gen_asm_proto
        MODEL BM168x
        BMKERNEL_API_H_IN ${CMAKE_CURRENT_SOURCE_DIR}/asm/bmkernel_api.h.in
        BMKERNEL_API_H ${CMAKE_BINARY_DIR}/include/onnc/Target/Sophon/BM168x/bmkernel_api.h
        PROTO ${bm168x_asm_proto}
    )

    # gen asm proto cpp/header
    gen_proto_cpp(
        TARGET bm168x_compile_asm_proto
        PROTO ${bm168x_asm_proto}
        SOURCE bm168x_asm_proto_src
        DEPEND ${bm168x_gen_asm_proto}
    )
    add_dependencies(onnc_Sophon
        bm168x_compile_asm_proto
    )

    # build asm proto cpp
    target_sources(onnc_Sophon
        ${bm168x_asm_proto_src}
    )
endif()