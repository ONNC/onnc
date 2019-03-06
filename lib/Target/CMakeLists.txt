add_libonnc_src(
    Platform.cpp
    Target.cpp 
    TargetRegistry.cpp
    TargetStandardPasses.cpp
    NPUTargetBackend.cpp)

foreach(TARGET_DIR IN LISTS ONNC_TARGETS)
    list(FIND ONNC_TARGETS ${TARGET_DIR} target_index)
    list(GET ONNC_TARGET_ENTRIES ${target_index} target_entry)
    if(${target_entry})
        add_subdirectory(${TARGET_DIR})
    endif(${target_entry})
endforeach(TARGET_DIR IN LISTS ONNC_TARGETS)

