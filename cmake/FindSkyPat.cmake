if(NOT SKYPAT_LIBRARY_DIR)
    find_library(SKYPAT_LIBRARIES
        NAMES skypat
    )
    get_filename_component(SKYPAT_LIBRARY_DIR ${SKYPAT_LIBRARIES} DIRECTORY)
endif(NOT SKYPAT_LIBRARY_DIR)
find_path(SKYPAT_INCLUDE_DIR
    NAMES skypat/skypat.h
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SKYPAT REQUIRED_VARS SKYPAT_INCLUDE_DIR SKYPAT_LIBRARY_DIR)