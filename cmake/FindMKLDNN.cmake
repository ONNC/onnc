find_library(MKLDNN_LIBRARIES
    NAMES mkldnn
    HINTS ${MKLDNN_ROOT}/lib
)
get_filename_component(MKLDNN_LIBRARY_DIR ${MKLDNN_LIBRARIES} DIRECTORY)
find_path(MKLDNN_INCLUDE_DIR
    NAMES mkldnn.h
    HINTS ${MKLDNN_ROOT}/include
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MKLDNN REQUIRED_VARS MKLDNN_INCLUDE_DIR MKLDNN_LIBRARY_DIR)
