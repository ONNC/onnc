
target_sources(libonnc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/X86Backend.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/X86InplaceValueFusible.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TargetInfo/X86TargetInfo.cpp
)
