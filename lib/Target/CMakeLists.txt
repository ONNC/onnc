
add_libonnc_src(
    Target.cpp 
    TargetBackend.cpp 
    TargetRegistry.cpp)

if (ENABLE_SOPHON_TARGET)
    add_subdirectory(Sophon)
endif()
if (ENABLE_TG_TARGET)
    add_subdirectory(TG)
endif()
if (ENABLE_X86_TARGET)
    add_subdirectory(X86)
endif()
