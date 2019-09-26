
add_libonnc_src(
    CortexmBackend.cpp
    CodeEmitVisitor.cpp
    TargetInfo/CortexMTargetInfo.cpp
    TargetInfo/CortexMTargetMemInfo.cpp
    CortexmMeta.cpp
    WeightPack.cpp
    CortexmMainFileGenPass.cpp
    CortexmInputPreProcessingFileGenPass.cpp
    CortexmWeightFileGenPass.cpp
    CortexmMainFileHeaderGenPass.cpp
    CortexmReadQuantizationConfigPass.cpp
)

