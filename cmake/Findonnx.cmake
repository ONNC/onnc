if(NOT ONNX_LIBRARY_DIR)
    find_library(ONNX_LIBRARIES
        NAMES onnx onnx_proto
    )
    get_filename_component(ONNX_LIBRARY_DIR ${ONNX_LIBRARIES} DIRECTORY)
endif(NOT ONNX_LIBRARY_DIR)
find_path(ONNX_INCLUDE_DIR
    NAMES onnx/onnx.pb.h
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ONNX REQUIRED_VARS ONNX_INCLUDE_DIR ONNX_LIBRARY_DIR)