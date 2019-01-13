find_library(PROTOBUF_LIBRARIES
    NAMES protobuf
    HINTS ${PROTOBUF_ROOT}/lib
)

get_filename_component(PROTOBUF_LIBRARY_DIR ${PROTOBUF_LIBRARIES} DIRECTORY)
find_path(PROTOBUF_INCLUDE_DIR
    NAMES google/protobuf/api.pb.h
    HINTS ${PROTOBUF_ROOT}/include
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PROTOBUF REQUIRED_VARS PROTOBUF_INCLUDE_DIR PROTOBUF_LIBRARY_DIR)
