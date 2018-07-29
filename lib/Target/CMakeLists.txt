
add_compile_options(-Werror -Wall)

add_libonnc_src(
    Target.cpp 
    TargetBackend.cpp 
    TargetOptions.cpp
    TargetRegistry.cpp
    NPUTargetBackend.cpp)

if (TARGET_SOPHON)
    add_subdirectory(Sophon)
endif()
if (TARGET_TG)
    add_subdirectory(TG)
endif()
if (TARGET_X86)
    add_subdirectory(X86)
endif()
