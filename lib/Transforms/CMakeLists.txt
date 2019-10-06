add_subdirectory(Optimizations)
add_subdirectory(TensorSel)

add_libonnc_src(
    BookONNXGraphs.cpp
    BuildInitializers.cpp
    BuildInputOperators.cpp
    BuildOutputOperators.cpp
    DeadNodeElimination.cpp
    GraphBuildingPass.cpp
    OnnxOptPass.cpp
    RemoveTrainingNodes.cpp
    TensorSel.cpp
)
