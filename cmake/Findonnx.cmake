
if (NOT ONNX_LIBRARIES)
    foreach(libname onnx onnx_proto)
        unset(libpath CACHE)
        find_library(ONNX_LIBRARIE_${libname}
            NAMES ${libname}
            HINTS ${ONNX_ROOT}/lib)
        list(APPEND ONNX_LIBRARIES ${ONNX_LIBRARIE_${libname}})
    endforeach()
    set(ONNX_LIBRARIES ${ONNX_LIBRARIES} CACHE STRING "")
else(NOT ONNX_LIBRARIES)
    list(GET ONNX_LIBRARIES 0 ONNX_LIBRARIE_onnx)
endif(NOT ONNX_LIBRARIES)
get_filename_component(ONNX_LIBRARY_DIR ${ONNX_LIBRARIE_onnx} DIRECTORY)

find_path(ONNX_INCLUDE_DIR
    NAMES onnx/onnx.pb.h
    HINTS ${ONNX_ROOT}/include
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ONNX REQUIRED_VARS ONNX_INCLUDE_DIR ONNX_LIBRARY_DIR)
