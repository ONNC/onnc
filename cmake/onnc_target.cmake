
function(enum_onnc_targets)
    option(TARGET_ALL "Build all targets" ON)

    macro(add_target name default_value)
        string(TOUPPER ${name} op_name)
        if (TARGET_ALL)
            set(TARGET_${op_name} ON               CACHE BOOL "Build ${name}" FORCE)
        else()
            set(TARGET_${op_name} ${default_value} CACHE BOOL "Build ${name}")
        endif()
        set(ENABLE_${op_name}_TARGET ${TARGET_${op_name}} PARENT_SCOPE)
    endmacro()
    add_target(x86 ON)
    add_target(tg ON)
    add_target(sophon ON)

endfunction()
