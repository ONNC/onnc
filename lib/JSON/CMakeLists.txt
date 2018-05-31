find_package(BISON REQUIRED)
set(BisonOutput JsonParser.cpp JsonParser.h)
add_custom_command(
    OUTPUT ${BisonOutput}
    COMMAND yacc --output=JsonParser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/JsonParser.ypp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/JsonParser.ypp)
add_custom_target(gen_json_parser
    DEPENDS ${BisonOutput})

find_package(FLEX REQUIRED)
set(FlexOutput JsonScanner.cpp)
add_custom_command(OUTPUT ${FlexOutput}
    COMMAND flex --outfile=${FlexOutput} ${CMAKE_CURRENT_SOURCE_DIR}/JsonScanner.lpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/JsonScanner.lpp)
add_custom_target(gen_json_scanner
    DEPENDS ${FlexOutput})

add_libonnc_src(
    Array.cpp
    JsonParser.cpp
    JsonScanner.cpp
    Notation.cpp
    Object.cpp
    String.cpp
    Value.cpp)

add_dependencies(${cur_target} gen_json_scanner gen_json_parser)
