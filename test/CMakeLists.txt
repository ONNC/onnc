include(check)

set(ONNC_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(ONNC_TOOL_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/../tools")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.in
    ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/onnc-lit.py.in
    ${CMAKE_CURRENT_BINARY_DIR}/onnc-lit.py)

set(LLVM_LIT_ARGS "-sv" CACHE STRING "Default options for lit")

add_lit_testsuites(LLVM ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
    onnx-as
    onnx-dis
    onnx2tg)
add_custom_target(check DEPENDS check-onnc)
