find_library(GLOG_LIBRARIES
    NAMES glog
    HINTS ${GLOG_ROOT}/lib
)

get_filename_component(GLOG_LIBRARY_DIR ${GLOG_LIBRARIES} DIRECTORY)
find_path(GLOG_INCLUDE_DIR
    NAMES glog/logging.h
    HINTS ${GLOG_ROOT}/include
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLOG REQUIRED_VARS GLOG_INCLUDE_DIR GLOG_LIBRARY_DIR)
