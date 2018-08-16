include(proto)
file(COPY 
    "bmkernel_api.h"
    DESTINATION ${PROJECT_BINARY_DIR}/include/onnc/Target/Sophon/BM168x
)
set(bm168x_asm_proto "${CMAKE_CURRENT_SOURCE_DIR}/asm/bm168x_asm.proto")
if (SOPHON_GEN_ASM_PROTO)
    include(bmtarget)
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
        PROTO ${bm168x_asm_proto}
        SOURCE bm168x_asm_src
        DEPEND ${bm168x_gen_asm_proto}
        OBJECT bm168x_asm_obj
    )
else(SOPHON_GEN_ASM_PROTO)
    gen_proto_cpp(
        PROTO ${bm168x_asm_proto}
        SOURCE bm168x_asm_src
        OBJECT bm168x_asm_obj
    )
endif(SOPHON_GEN_ASM_PROTO)

set(onnc_Sophon_obj ${onnc_Sophon_obj} ${bm168x_asm_obj} PARENT_SCOPE)