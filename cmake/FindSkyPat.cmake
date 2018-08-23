find_library(SKYPAT_LIBRARIES
    NAMES skypat
    HINTS ${SKYPAT_ROOT}/lib
)
get_filename_component(SKYPAT_LIBRARY_DIR ${SKYPAT_LIBRARIES} DIRECTORY)
find_path(SKYPAT_INCLUDE_DIR
    NAMES skypat/skypat.h
    HINTS ${SKYPAT_ROOT}/include
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SKYPAT REQUIRED_VARS SKYPAT_INCLUDE_DIR SKYPAT_LIBRARY_DIR)
