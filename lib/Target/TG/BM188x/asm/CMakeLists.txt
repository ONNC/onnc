
# gen asm proto
set(bmkernel_api_h ${ONNC_SOURCE_DIR}/include/onnc/Target/TG/BM188x/bmkernel_api.h)
set(asm_proto ${CMAKE_CURRENT_SOURCE_DIR}/bm188x_asm.proto)
set(asm_files ${bmkernel_api_h} ${asm_proto})
add_custom_command(OUTPUT ${asm_files}
    COMMAND PYTHONPATH=${bm_PYTHONPATH} python3 -B ${bm_target_path}/target/BM188x/asm.py
    --api-in ${CMAKE_CURRENT_SOURCE_DIR}/bmkernel_api.h.in
    --api-out ${bmkernel_api_h}
    --proto-in ${bm_target_path}/target/BM188x/asm.proto.in
    --proto-out ${asm_proto}
    DEPENDS
    ${bm_target_path}/target/BM188x/asm.py
    bmkernel_api.h.in
    ${bm_target_path}/target/BM188x/asm.proto.in
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_target(gen_188x_asm_proto
    DEPENDS ${asm_files})


# gen asm proto cpp/header
include(proto)
gen_proto_cpp(TARGET compile_asm_proto PROTO ${asm_proto} SRCS asm_proto_src)
add_dependencies(${compile_asm_proto} gen_188x_asm_proto)

# build asm proto cpp
add_libonnc_src(${asm_proto_src})
