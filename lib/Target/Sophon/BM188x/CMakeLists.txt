include(proto)
gen_proto_cpp(
    PROTO ${CMAKE_CURRENT_SOURCE_DIR}/common_calibration2.proto
    SOURCE common_calibration2_src
)
file(COPY 
    "bmkernel_api.h"
    DESTINATION ${PROJECT_BINARY_DIR}/include/onnc/Target/Sophon/BM188x
)
set(bm188x_asm_proto "${CMAKE_CURRENT_SOURCE_DIR}/asm/bm188x_asm.proto")
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
        PROTO ${bm188x_asm_proto}
        SOURCE bm188x_asm_src
        DEPEND ${bm188x_gen_asm_proto}
    )
else(TG_GEN_ASM_PROTO)
    gen_proto_cpp(
        PROTO ${bm188x_asm_proto}
        SOURCE bm188x_asm_src
    )
endif(TG_GEN_ASM_PROTO)

add_subdirectory(Tests)
