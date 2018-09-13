cmake_minimum_required(VERSION 3.5)
enable_testing()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

####################
#  General settings
include(option)
option_enum(NAME CMAKE_BUILD_TYPE HELP "Choose the type of build" VALUE Debug Release Regression Optimized)
option(ENABLE_PTHREAD "use pthreads" ON)
set(HAVE_PTHREADS ${ENABLE_PTHREAD})
option(ENABLE_UNITTEST "enable unittest" ON)
option(ENABLE_CLOCK_GETTIME "enable clock_gettime()" OFF)
option(ENABLE_GETTIMEOFDAY "enable gettimeofday()" ON)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    option(ENABLE_UNITTEST "enable unittest" ON)
else()
    option(ENABLE_UNITTEST "enable unittest" OFF)
endif()

####################
#  Language
set(STDC_HEADERS 1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g3"          CACHE STRING "Flags used by the CXX compiler during DEBUG builds.")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG"     CACHE STRING "Flags used by the CXX compiler during RELEASE builds.")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -DNDEBUG -g3" CACHE STRING "Flags used by the CXX compiler during RELWITHDEBINFO builds.")
option(ENABLE_WERROR "Compile with -Werror enabled (default is OFF)" OFF)
if (ENABLE_WERROR)
    add_compile_options(-Werror)
endif()
#include(CheckCXXCompilerFlag)
#check_cxx_compiler_flag(-Wno-variadic-macros WNO_VARIADIC_MACROS)

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
message(STATUS "ONNC_DEFAULT_TARGET_QUADRUPAL: ${ONNC_DEFAULT_TARGET_QUADRUPAL}")

####################
#  Targets
include(onnc_target)
enum_onnc_targets()

####################
#  Include
set(ONNC_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/lib)

####################
#  External
#  onnx
set(ONNX_NAMESPACE "onnx" CACHE STRING "Namespace of onnx")
add_definitions(-DONNX_NAMESPACE=${ONNX_NAMESPACE})
find_package(onnx REQUIRED)
message(STATUS "[${PACKAGE}] Using onnx include at ${ONNX_INCLUDE_DIR}")
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
    find_package(llvm)
    if(LLVM_INCLUDE_DIR AND LLVM_LIBRARY_DIR)
        message(STATUS "[${PACKAGE}] Using llvm include at ${LLVM_INCLUDE_DIR}")
        include_directories(${LLVM_INCLUDE_DIR})
        link_directories(${LLVM_LIBRARY_DIR})
    endif(LLVM_INCLUDE_DIR AND LLVM_LIBRARY_DIR)
endif(USE_LLVM)
find_package(Protobuf REQUIRED)
    
####################
# OUTPUT
configure_file(include/onnc/Support/DataTypes.h.cmake.in include/onnc/Support/DataTypes.h)
configure_file(include/onnc/Config/Config.h.cmake.in include/onnc/Config/Config.h)
configure_file(include/onnc/Config/ONNX.h.in include/onnc/Config/ONNX.h @ONLY)
configure_file(include/onnc/Config/Platforms.def.in include/onnc/Config/Platforms.def @ONLY)
configure_file(include/onnc/Config/Backends.def.in include/onnc/Config/Backends.def @ONLY)

####################
# Other
set(ANDROID_CPPFLAGS -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address)
add_compile_options(${ANDROID_CPPFLAGS})
add_custom_target(code_gen)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(include)
