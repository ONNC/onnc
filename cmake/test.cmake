
function(tg_test)
    cmake_parse_arguments(OPT "" "ASM_NAME;MARCH" "" ${ARGN})
    get_filename_component(name ${OPT_ASM_NAME} NAME_WE)
    get_filename_component(dir ${OPT_ASM_NAME} DIRECTORY)
    set(onnx_name ${CMAKE_CURRENT_BINARY_DIR}/${dir}/${name}.onnx)
    set(asm_name ${CMAKE_CURRENT_SOURCE_DIR}/${OPT_ASM_NAME})
    set(log_name ${CMAKE_CURRENT_BINARY_DIR}/${dir}/${name}.log)
    add_test(NAME test_${name}_as COMMAND onnx-as ${asm_name} -o ${onnx_name})
    add_test(NAME test_${name}_fc
        COMMAND sh -c "$<TARGET_FILE:onnx2tg> ${onnx_name} -march ${OPT_MARCH} -print-machineinstrs | tee ${log_name} | FileCheck ${asm_name}")
    set_tests_properties(test_${name}_fc PROPERTIES DEPENDS test_${name}_as)
endfunction()
