
add_libonnc_src(
    CortexmBackend.cpp
    CodeEmitVisitor.cpp
    TargetInfo/CortexMTargetInfo.cpp
    TargetInfo/CortexMTargetMemInfo.cpp
    WeightPack.cpp
    CortexmMainFileGenPass.cpp
    CortexmWeightFileGenPass.cpp
    CortexmMainFileHeaderGenPass.cpp
    CortexmReadQuantizationConfigPass.cpp
)

