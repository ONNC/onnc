
set(asm_proto ${CMAKE_CURRENT_SOURCE_DIR}/bm188x_asm.proto)

# gen bm188x_asm.proto and bmkernel_api.h
include(bmtarget)
bm_gen_asm_proto(
    TARGET gen_asm_proto
    MODEL BM188x
    BMKERNEL_API_H_IN ${CMAKE_CURRENT_SOURCE_DIR}/bmkernel_api.h.in
    BMKERNEL_API_H ${tg_root}/include/onnc/Target/TG/BM188x/bmkernel_api.h
    PROTO ${asm_proto})

# gen asm proto cpp/header
include(proto)
gen_proto_cpp(TARGET compile_asm_proto PROTO ${asm_proto} SRCS asm_proto_src)
add_dependencies(${compile_asm_proto} ${gen_asm_proto})

# build asm proto cpp
add_libonnc_src(${asm_proto_src})
