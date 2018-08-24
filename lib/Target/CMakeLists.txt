
add_compile_options(-Werror -Wall)

add_libonnc_src(
    Target.cpp 
    TargetBackend.cpp 
    TargetOptions.cpp
    TargetRegistry.cpp
    NPUTargetBackend.cpp)

foreach(TARGET_DIR IN LISTS ONNC_TARGETS)
    list(FIND ONNC_TARGETS ${TARGET_DIR} target_index)
    list(GET ONNC_TARGET_ENTRIES ${target_index} target_entry)
    if(${target_entry})
        add_subdirectory(${TARGET_DIR})
    endif(${target_entry})
endforeach(TARGET_DIR IN LISTS ONNC_TARGETS)

