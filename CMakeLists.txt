cmake_minimum_required(VERSION 3.5)
enable_testing()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

####################
#  General settings
include(option)
option_enum(NAME CMAKE_BUILD_TYPE HELP "Choose the type of build" VALUE Normal Debug Release)
option(ENABLE_PTHREAD "use pthreads" ON)
set(HAVE_PTHREADS ${ENABLE_PTHREAD})
option(ENABLE_CLOCK_GETTIME "enable clock_gettime()" ON)
option(ENABLE_GETTIMEOFDAY "enable gettimeofday()" ON)
if(CMAKE_BUILD_TYPE STREQUAL Release)
    option(ENABLE_UNITTEST "enable unittest" OFF)
else()
    option(ENABLE_UNITTEST "enable unittest" ON)
endif()

####################
#  Language
set(STDC_HEADERS 1)
set(CMAKE_CXX_STANDARD 14)
set(HAVE_CXX14 TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g3"          CACHE STRING "Flags used by the CXX compiler during DEBUG builds.")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG"     CACHE STRING "Flags used by the CXX compiler during RELEASE builds.")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -DNDEBUG -g3" CACHE STRING "Flags used by the CXX compiler during RELWITHDEBINFO builds.")
option(ENABLE_WERROR "Compile with -Werror enabled (default is OFF)" OFF)
if (ENABLE_WERROR)
    add_compile_options(-Werror)
endif()

####################
#  Project
project(onnc)
set(ONNC_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR})
file(READ ${ONNC_ROOT_PATH}/VERSION PACKAGE_VERSION)
string(REPLACE "\n" " " PACKAGE_VERSION ${PACKAGE_VERSION})
set(PACKAGE "onnc")
set(PACKAGE_BUGREPORT "maillist.onnc.ai")
set(PACKAGE_NAME "ONNC")
set(PACKAGE_STRING "ONNC ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME "onnc")
set(PACKAGE_URL "onnc.ai")

####################
#  Check
include(check)
message(STATUS "Default target quadruple: ${ONNC_DEFAULT_TARGET_QUADRUPLE}")

####################
#  Scan targets from config
file(STRINGS ${ONNC_ROOT_PATH}/include/onnc/Config/Config.h.cmake.in CONFIG_IN_CONTENT)
string(REGEX MATCHALL "ENABLE_[A-Za-z0-9_]*_TARGET" CONFIG_IN_TARGETS ${CONFIG_IN_CONTENT})
file(GLOB target_dir_dump RELATIVE ${ONNC_ROOT_PATH}/lib/Target ${ONNC_ROOT_PATH}/lib/Target/* )
foreach(content IN LISTS target_dir_dump)
    string(TOUPPER ${content} content)
    set(target_dir_upper ${target_dir_upper} ${content})
endforeach(content IN LISTS target_dir_dump)
foreach(EACH_TARGET IN LISTS CONFIG_IN_TARGETS)
    # Get target name
    string(LENGTH ${EACH_TARGET} EACH_LEN)
    math(EXPR SUBSTR_LEN "${EACH_LEN} - 14")
    string(SUBSTRING ${EACH_TARGET} 7 ${SUBSTR_LEN} EACH_TARGET_NAME)
    # Set option
    string(TOUPPER ${EACH_TARGET_NAME} EACH_TARGET_NAME_upper)
    if(EACH_TARGET_NAME_upper IN_LIST target_dir_upper)
        option(${EACH_TARGET} "Enable ${EACH_TARGET_NAME}" ON)
        if(${EACH_TARGET})
            list(FIND target_dir_upper ${EACH_TARGET_NAME_upper} target_dir_index)
            list(GET target_dir_dump ${target_dir_index} target_name_real)
            set(ONNC_TARGET_PLATFORMS "${ONNC_TARGET_PLATFORMS} ONNC_PLATFORM(${target_name_real})")
            set(ONNC_TARGET_BACKENDS "${ONNC_TARGET_BACKENDS} ONNC_BACKEND(${target_name_real})")
            set(ONNC_TARGETS ${ONNC_TARGETS} ${target_name_real})
            set(ONNC_TARGET_ENTRIES ${ONNC_TARGET_ENTRIES} ${EACH_TARGET})
        endif(${EACH_TARGET})
    endif(EACH_TARGET_NAME_upper IN_LIST target_dir_upper)
endforeach(EACH_TARGET IN CONFIG_IN_TARGETS)
unset(target_dir_upper)

####################
#  Include
set(ONNC_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/lib
)

####################
#  External
#  onnx
set(ONNX_NAMESPACE "onnx" CACHE STRING "Namespace of onnx")
add_definitions(-DONNX_NAMESPACE=${ONNX_NAMESPACE})
find_package(onnx REQUIRED)
message(STATUS "[${PACKAGE}] Using onnx include at ${ONNX_INCLUDE_DIR}")
message(STATUS "[${PACKAGE}] Using onnx library at ${ONNX_LIBRARY_DIR}")
include_directories(${ONNX_INCLUDE_DIR})
link_directories(${ONNX_LIBRARY_DIR})
#  SkyPat
find_package(SkyPat REQUIRED)
message(STATUS "[${PACKAGE}] Using SkyPat include at ${SKYPAT_INCLUDE_DIR}")
include_directories(${SKYPAT_INCLUDE_DIR})
link_directories(${SKYPAT_LIBRARY_DIR})
#  llvm
OPTION(USE_LLVM "Use llvm" ON)
if(USE_LLVM)
    if(LLVM_ROOT_DIR)
        set(LLVM_DIR ${LLVM_ROOT_DIR}/lib/cmake/llvm)
    endif(LLVM_ROOT_DIR)
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "[${PACKAGE}] Using llvm include at ${LLVM_INCLUDE_DIR}")
    include_directories(${LLVM_INCLUDE_DIRS})
    add_definitions(${LLVM_DEFINITIONS})
endif(USE_LLVM)
#  Protobuf
find_package(Protobuf REQUIRED)
#  glog
find_package(Glog REQUIRED)
message(STATUS "[${PACKAGE}] Using glog include at ${GLOG_INCLUDE_DIR}")
include_directories(${GLOG_INCLUDE_DIR})
link_directories(${GLOG_LIBRARY_DIR})

####################
# Other
add_custom_target(code_gen)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(include)

####################
# OUTPUT
configure_file(include/onnc/Support/DataTypes.h.cmake.in include/onnc/Support/DataTypes.h)
configure_file(include/onnc/Config/Config.h.cmake.in include/onnc/Config/Config.h)
configure_file(include/onnc/Config/ONNX.h.in include/onnc/Config/ONNX.h @ONLY)
configure_file(include/onnc/Config/Platforms.def.in include/onnc/Config/Platforms.def @ONLY)
configure_file(include/onnc/Config/Backends.def.in include/onnc/Config/Backends.def @ONLY)
configure_file(cmake/ONNCConfig.cmake.in cmake/ONNCConfig.cmake @ONLY)
