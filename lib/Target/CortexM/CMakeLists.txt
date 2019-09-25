
add_libonnc_src(
    CortexmBackend.cpp
    CodeEmitVisitor.cpp
    TargetInfo/CortexMTargetInfo.cpp
    TargetInfo/CortexMTargetMemInfo.cpp
    CortexmMeta.cpp
    weight_pack.cpp
    CortexmMainFileGenPass.cpp
    CortexmInputPreProcessingFileGenPass.cpp
    CortexmWeightFileGenPass.cpp
    loadable.cpp
    CortexmMainFileHeaderGenPass.cpp
    CortexmReadQuantizationConfigPass.cpp
)

