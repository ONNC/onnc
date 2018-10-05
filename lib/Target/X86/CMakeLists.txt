
add_libonnc_src(
    Compute/X86ConvRelu.cpp
    X86Backend.cpp
    X86FuseConvRelu.cpp
    X86InplaceValueFusible.cpp
    X86RemoveWeightFromLiveIntervals.cpp
    TargetInfo/X86TargetInfo.cpp
    TargetInfo/X86TargetMemInfo.cpp)
