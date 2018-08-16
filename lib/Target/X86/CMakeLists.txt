
add_library(onnc_X86 OBJECT
    "X86Backend.cpp"
    "X86InplaceValueFusible.cpp"
    "TargetInfo/X86TargetInfo.cpp"
)