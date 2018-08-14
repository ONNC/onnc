include(proto)
set(bm188x_asm_proto ${CMAKE_CURRENT_SOURCE_DIR}/asm/bm188x_asm.proto)
gen_proto_cpp(
    TARGET bm188x_common_calibration2_proto
    PROTO ${CMAKE_CURRENT_SOURCE_DIR}/common_calibration2.proto
    SOURCE proto_src
)
if (TG_GEN_ASM_PROTO)
    include(bmtarget)
    # gen bm188x_asm.proto and bmkernel_api.h
    bm_gen_asm_proto(
        TARGET bm188x_gen_asm_proto
        MODEL BM188x
        BMKERNEL_API_H_IN ${CMAKE_CURRENT_SOURCE_DIR}/asm/bmkernel_api.h.in
        BMKERNEL_API_H ${CMAKE_BINARY_DIR}/include/onnc/Target/Sophon/BM188x/bmkernel_api.h
        PROTO ${bm188x_asm_proto}
    )
    # gen asm proto cpp/header
    gen_proto_cpp(
        TARGET bm188x_compile_asm_proto
        PROTO ${bm188x_asm_proto}
        SOURCE bm188x_asm_proto_src
        DEPEND ${bm188x_gen_asm_proto}
    )
else(TG_GEN_ASM_PROTO)
    # gen asm proto cpp/header
    gen_proto_cpp(
        TARGET bm188x_compile_asm_proto
        PROTO ${bm188x_asm_proto}
        SOURCE bm188x_asm_proto_src
    )
endif()
add_dependencies(onnc_Sophon
    bm188x_common_calibration2_proto
    bm188x_compile_asm_proto
)


file(COPY 
    "bmkernel_api.h"
    DESTINATION ${PROJECT_BINARY_DIR}/include/onnc/Target/Sophon/BM188x
)

add_subdirectory(Tests)
